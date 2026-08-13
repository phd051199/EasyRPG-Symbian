#include <stdint.h>
#include <stddef.h>
#include <new>
#include <cstring>

#include "MidiLiteBridge.h"
#include "midisequencer.h"

namespace
{
const int KChannelCount = 16;
const int KVoiceCount = 24;
const int KDrumChannel = 9;
const int KGainScale = 2048;
const uint32_t KInitialPhase = 0x40000000u;

const uint32_t KNoteStep[128] = {
1592507, 1687203, 1787529, 1893821, 2006434, 2125742, 2252146, 2386065,
2527948, 2678268, 2837526, 3006254, 3185015, 3374406, 3575058, 3787642,
4012867, 4251485, 4504291, 4772130, 5055896, 5356535, 5675051, 6012507,
6370030, 6748811, 7150117, 7575285, 8025735, 8502970, 9008582, 9544261,
10111792, 10713070, 11350103, 12025015, 12740059, 13497623, 14300233, 15150569,
16051469, 17005939, 18017165, 19088521, 20223584, 21426141, 22700205, 24050030,
25480119, 26995246, 28600467, 30301139, 32102938, 34011878, 36034330, 38177043,
40447168, 42852281, 45400411, 48100060, 50960238, 53990491, 57200933, 60602278,
64205876, 68023757, 72068660, 76354085, 80894335, 85704563, 90800821, 96200119,
101920476, 107980983, 114401866, 121204555, 128411753, 136047513, 144137319, 152708170,
161788671, 171409126, 181601643, 192400238, 203840952, 215961966, 228803732, 242409110,
256823506, 272095026, 288274639, 305416341, 323577341, 342818251, 363203285, 384800477,
407681904, 431923931, 457607465, 484818220, 513647012, 544190053, 576549277, 610832681,
647154683, 685636503, 726406571, 769600953, 815363807, 863847862, 915214929, 969636441,
1027294024, 1088380105, 1153098554, 1221665363, 1294309365, 1371273005, 1452813141, 1539201906,
1630727614, 1727695724, 1830429858, 1939272882, 2054588048, 2176760211, 2306197109, 2443330725
};

enum TWave
{
    EWaveTriangle,
    EWaveSquare,
    EWaveSaw,
    EWaveNoise
};

struct TMemoryReader
{
    const unsigned char* data;
    int size;
    int position;
};

int MemoryGetc(void* context)
{
    TMemoryReader* reader = static_cast<TMemoryReader*>(context);
    if (!reader || reader->position >= reader->size)
        return EOF;
    return reader->data[reader->position++];
}

struct TChannel
{
    int program;
    int volume;
    int expression;
    int pan;
    int pitch;
    bool sustain;
};

struct TVoice
{
    bool active;
    bool released;
    bool sustainRelease;
    unsigned int serial;
    int channel;
    int note;
    int velocity;
    int wave;
    uint32_t phase;
    uint32_t step;
    uint32_t noise;
    int gainBase;
    int32_t gainLeftQ16;
    int32_t gainRightQ16;
    int32_t decayLeftQ16;
    int32_t decayRightQ16;
};

class CMidiLite : public midisequencer::output
{
public:
    CMidiLite(int sampleRate, int pitch)
        : iSampleRate(sampleRate),
          iPitch(pitch),
          iTime(0.0f),
          iTotalTime(0.0f),
          iLoopCount(0),
          iSerial(0),
          iLoaded(false)
    {
        ResetState();
    }

    bool Open(const unsigned char* data, int size)
    {
        if (!data || size <= 0 || iSampleRate <= 0)
            return false;
        TMemoryReader reader;
        reader.data = data;
        reader.size = size;
        reader.position = 0;
        if (!iSequence.load(&reader, MemoryGetc))
            return false;
        iTotalTime = iSequence.get_total_time();
        if (iTotalTime <= 0.0f)
            return false;
        iSequence.rewind();
        iTime = 0.0f;
        iLoopCount = 0;
        ResetState();
        iLoaded = true;
        return true;
    }

    int Render(short* output, int frames)
    {
        if (!iLoaded || !output || frames <= 0)
            return 0;
        int rendered = 0;
        while (rendered < frames)
        {
            iSequence.play(iTime + 0.000001f, this);
            int batch = frames - rendered;
            if (batch > 64)
                batch = 64;
            Synthesize(output + rendered * 2, batch);
            rendered += batch;
            iTime += static_cast<float>(batch * iPitch) /
                     static_cast<float>(iSampleRate * 100);
            if (iTime >= iTotalTime)
            {
                iSequence.play(iTotalTime + 0.001f, this);
                while (iTime >= iTotalTime)
                {
                    iTime -= iTotalTime;
                    ++iLoopCount;
                }
                iSequence.rewind();
                ResetState();
                iSequence.play(iTime + 0.000001f, this);
            }
        }
        return rendered;
    }

    void SetPitch(int pitch)
    {
        if (pitch >= 50 && pitch <= 200)
            iPitch = pitch;
    }

    unsigned int Ticks() const
    {
        if (!iLoaded || iTime <= 0.0f)
            return 0;
        const float value = iTime * 1000.0f;
        if (value >= 4294967295.0f)
            return 0xffffffffu;
        return static_cast<unsigned int>(value);
    }

    unsigned int LoopCount() const
    {
        return iLoopCount;
    }

    void midi_message(int, uint_least32_t message)
    {
        const int status = static_cast<int>(message & 0xff);
        const int channel = status & 0x0f;
        const int first = static_cast<int>((message >> 8) & 0x7f);
        const int second = static_cast<int>((message >> 16) & 0x7f);
        if (channel < 0 || channel >= KChannelCount)
            return;
        switch (status & 0xf0)
        {
            case 0x80:
                NoteOff(channel, first);
                break;
            case 0x90:
                if (second == 0)
                    NoteOff(channel, first);
                else
                    NoteOn(channel, first, second);
                break;
            case 0xb0:
                ControlChange(channel, first, second);
                break;
            case 0xc0:
                iChannels[channel].program = first;
                break;
            case 0xe0:
                iChannels[channel].pitch = (second << 7) | first;
                RefreshPitch(channel);
                break;
            default:
                break;
        }
    }

    void sysex_message(int, const void* data, std::size_t size)
    {
        if (!data || size < 6)
            return;
        const unsigned char* bytes = static_cast<const unsigned char*>(data);
        if (bytes[0] == 0xf0 && bytes[1] == 0x7e && bytes[3] == 0x09)
            ResetControllers();
    }

    void meta_event(int, const void*, std::size_t)
    {
    }

    void reset()
    {
        ResetState();
    }

private:
    static int Clamp127(int value)
    {
        if (value < 0)
            return 0;
        if (value > 127)
            return 127;
        return value;
    }

    void ResetControllers()
    {
        for (int channel = 0; channel < KChannelCount; ++channel)
        {
            iChannels[channel].program = 0;
            iChannels[channel].volume = 100;
            iChannels[channel].expression = 127;
            iChannels[channel].pan = 64;
            iChannels[channel].pitch = 8192;
            iChannels[channel].sustain = false;
        }
    }

    void ResetState()
    {
        ResetControllers();
        for (int voice = 0; voice < KVoiceCount; ++voice)
            std::memset(&iVoices[voice], 0, sizeof(iVoices[voice]));
    }

    static int ProgramWave(int program)
    {
        if (program < 16)
            return EWaveTriangle;
        if (program < 24)
            return EWaveSquare;
        if (program < 40)
            return EWaveTriangle;
        if (program < 64)
            return EWaveSaw;
        if (program < 72)
            return EWaveSquare;
        if (program < 80)
            return EWaveTriangle;
        if (program < 88)
            return EWaveSquare;
        if (program < 96)
            return EWaveSaw;
        return EWaveTriangle;
    }

    static bool DecaysNaturally(int program)
    {
        return program < 16 || (program >= 24 && program < 40);
    }

    int FindVoice()
    {
        for (int voice = 0; voice < KVoiceCount; ++voice)
            if (!iVoices[voice].active)
                return voice;
        int selected = 0;
        for (int voice = 1; voice < KVoiceCount; ++voice)
        {
            if (iVoices[voice].released && !iVoices[selected].released)
                selected = voice;
            else if (iVoices[voice].released == iVoices[selected].released &&
                     iVoices[voice].serial < iVoices[selected].serial)
                selected = voice;
        }
        return selected;
    }

    int ComputeBaseGain(int channel, int velocity) const
    {
        const TChannel& state = iChannels[channel];
        const int64_t numerator = static_cast<int64_t>(velocity) *
                                  state.volume * state.expression * KGainScale;
        return static_cast<int>(numerator / 2048383);
    }

    void SetVoicePan(TVoice& voice, int baseGain)
    {
        const int pan = Clamp127(iChannels[voice.channel].pan);
        int left = baseGain * (127 - pan) / 64;
        int right = baseGain * pan / 64;
        if (left > KGainScale)
            left = KGainScale;
        if (right > KGainScale)
            right = KGainScale;
        voice.gainBase = baseGain;
        voice.gainLeftQ16 = left << 16;
        voice.gainRightQ16 = right << 16;
    }

    uint32_t PitchStep(int note, int pitch) const
    {
        if (note < 0)
            note = 0;
        if (note > 127)
            note = 127;
        int bend = pitch - 8192;
        if (bend == 0)
            return KNoteStep[note];
        const int direction = bend > 0 ? 1 : -1;
        int amount = bend > 0 ? bend : -bend;
        if (amount > 8192)
            amount = 8192;
        const int semitoneQ15 = amount * 2 * 32768 / 8192;
        int whole = semitoneQ15 >> 15;
        int fraction = semitoneQ15 & 0x7fff;
        int firstNote = note + direction * whole;
        int secondNote = firstNote + direction;
        if (firstNote < 0)
            firstNote = 0;
        if (firstNote > 127)
            firstNote = 127;
        if (secondNote < 0)
            secondNote = 0;
        if (secondNote > 127)
            secondNote = 127;
        const int64_t first = KNoteStep[firstNote];
        const int64_t second = KNoteStep[secondNote];
        return static_cast<uint32_t>(first + ((second - first) * fraction >> 15));
    }

    void NoteOn(int channel, int note, int velocity)
    {
        if (note < 0 || note > 127 || velocity <= 0)
            return;
        NoteOff(channel, note);
        const int index = FindVoice();
        TVoice& voice = iVoices[index];
        std::memset(&voice, 0, sizeof(voice));
        voice.active = true;
        voice.serial = ++iSerial;
        voice.channel = channel;
        voice.note = note;
        voice.velocity = velocity;
        voice.phase = KInitialPhase;
        voice.step = PitchStep(note, iChannels[channel].pitch);
        voice.noise = 0x1234567u ^ static_cast<uint32_t>(note * 1103515245u + iSerial);
        voice.wave = channel == KDrumChannel ? EWaveNoise : ProgramWave(iChannels[channel].program);
        const int baseGain = ComputeBaseGain(channel, velocity);
        SetVoicePan(voice, baseGain);
        int decayFrames = 0;
        if (channel == KDrumChannel)
            decayFrames = (note == 42 || note == 44 || note == 46) ? iSampleRate / 12 : iSampleRate / 3;
        else if (DecaysNaturally(iChannels[channel].program))
            decayFrames = iSampleRate * 4;
        if (decayFrames > 0)
        {
            voice.decayLeftQ16 = voice.gainLeftQ16 / decayFrames;
            voice.decayRightQ16 = voice.gainRightQ16 / decayFrames;
            if (voice.decayLeftQ16 <= 0 && voice.gainLeftQ16 > 0)
                voice.decayLeftQ16 = 1;
            if (voice.decayRightQ16 <= 0 && voice.gainRightQ16 > 0)
                voice.decayRightQ16 = 1;
        }
    }

    void BeginRelease(TVoice& voice)
    {
        if (!voice.active || voice.released)
            return;
        voice.released = true;
        voice.sustainRelease = false;
        const int frames = iSampleRate / 8;
        voice.decayLeftQ16 = frames > 0 ? voice.gainLeftQ16 / frames : voice.gainLeftQ16;
        voice.decayRightQ16 = frames > 0 ? voice.gainRightQ16 / frames : voice.gainRightQ16;
        if (voice.decayLeftQ16 <= 0 && voice.gainLeftQ16 > 0)
            voice.decayLeftQ16 = 1;
        if (voice.decayRightQ16 <= 0 && voice.gainRightQ16 > 0)
            voice.decayRightQ16 = 1;
    }

    void NoteOff(int channel, int note)
    {
        for (int index = 0; index < KVoiceCount; ++index)
        {
            TVoice& voice = iVoices[index];
            if (!voice.active || voice.channel != channel || voice.note != note || voice.released)
                continue;
            if (iChannels[channel].sustain)
                voice.sustainRelease = true;
            else
                BeginRelease(voice);
        }
    }

    void ReleaseSustain(int channel)
    {
        for (int index = 0; index < KVoiceCount; ++index)
        {
            TVoice& voice = iVoices[index];
            if (voice.active && voice.channel == channel && voice.sustainRelease)
                BeginRelease(voice);
        }
    }

    void AllNotesOff(int channel, bool immediate)
    {
        for (int index = 0; index < KVoiceCount; ++index)
        {
            TVoice& voice = iVoices[index];
            if (!voice.active || voice.channel != channel)
                continue;
            if (immediate)
                voice.active = false;
            else
                BeginRelease(voice);
        }
    }

    void RefreshPitch(int channel)
    {
        for (int index = 0; index < KVoiceCount; ++index)
        {
            TVoice& voice = iVoices[index];
            if (voice.active && voice.channel == channel)
                voice.step = PitchStep(voice.note, iChannels[channel].pitch);
        }
    }

    void RefreshGain(int channel)
    {
        for (int index = 0; index < KVoiceCount; ++index)
        {
            TVoice& voice = iVoices[index];
            if (!voice.active || voice.channel != channel)
                continue;
            const int oldBase = voice.gainBase;
            const int newBase = ComputeBaseGain(channel, voice.velocity);
            if (oldBase > 0)
            {
                voice.gainLeftQ16 = static_cast<int32_t>(
                    static_cast<int64_t>(voice.gainLeftQ16) * newBase / oldBase);
                voice.gainRightQ16 = static_cast<int32_t>(
                    static_cast<int64_t>(voice.gainRightQ16) * newBase / oldBase);
            }
            voice.gainBase = newBase;
        }
    }

    void RefreshPan(int channel)
    {
        for (int index = 0; index < KVoiceCount; ++index)
        {
            TVoice& voice = iVoices[index];
            if (!voice.active || voice.channel != channel)
                continue;
            int current = voice.gainLeftQ16 > voice.gainRightQ16 ?
                          voice.gainLeftQ16 : voice.gainRightQ16;
            const int pan = Clamp127(iChannels[channel].pan);
            voice.gainLeftQ16 = static_cast<int32_t>(
                static_cast<int64_t>(current) * (127 - pan) / 64);
            voice.gainRightQ16 = static_cast<int32_t>(
                static_cast<int64_t>(current) * pan / 64);
        }
    }

    void ControlChange(int channel, int control, int value)
    {
        value = Clamp127(value);
        switch (control)
        {
            case 7:
                iChannels[channel].volume = value;
                RefreshGain(channel);
                break;
            case 10:
                iChannels[channel].pan = value;
                RefreshPan(channel);
                break;
            case 11:
                iChannels[channel].expression = value;
                RefreshGain(channel);
                break;
            case 64:
            {
                const bool old = iChannels[channel].sustain;
                iChannels[channel].sustain = value >= 64;
                if (old && !iChannels[channel].sustain)
                    ReleaseSustain(channel);
                break;
            }
            case 120:
                AllNotesOff(channel, true);
                break;
            case 121:
                iChannels[channel].volume = 100;
                iChannels[channel].expression = 127;
                iChannels[channel].pan = 64;
                iChannels[channel].pitch = 8192;
                iChannels[channel].sustain = false;
                RefreshGain(channel);
                RefreshPan(channel);
                RefreshPitch(channel);
                ReleaseSustain(channel);
                break;
            case 123:
                AllNotesOff(channel, false);
                break;
            default:
                break;
        }
    }

    static int WaveSample(TVoice& voice)
    {
        const uint32_t phase = voice.phase;
        voice.phase += voice.step;
        if (voice.wave == EWaveSquare)
            return (phase & 0x80000000u) ? 32767 : -32767;
        if (voice.wave == EWaveSaw)
            return static_cast<int16_t>(phase >> 16);
        if (voice.wave == EWaveNoise)
        {
            uint32_t value = voice.noise;
            value ^= value << 13;
            value ^= value >> 17;
            value ^= value << 5;
            voice.noise = value;
            return static_cast<int16_t>(value >> 16);
        }
        const uint32_t x = phase >> 16;
        return x < 32768u ? static_cast<int>(x * 2u) - 32768
                          : 98303 - static_cast<int>(x * 2u);
    }

    void AdvanceEnvelope(TVoice& voice)
    {
        if (voice.decayLeftQ16 > 0)
        {
            voice.gainLeftQ16 -= voice.decayLeftQ16;
            if (voice.gainLeftQ16 < 0)
                voice.gainLeftQ16 = 0;
        }
        if (voice.decayRightQ16 > 0)
        {
            voice.gainRightQ16 -= voice.decayRightQ16;
            if (voice.gainRightQ16 < 0)
                voice.gainRightQ16 = 0;
        }
        if (voice.gainLeftQ16 == 0 && voice.gainRightQ16 == 0)
            voice.active = false;
    }

    void Synthesize(short* output, int frames)
    {
        for (int frame = 0; frame < frames; ++frame)
        {
            int32_t left = 0;
            int32_t right = 0;
            for (int index = 0; index < KVoiceCount; ++index)
            {
                TVoice& voice = iVoices[index];
                if (!voice.active)
                    continue;
                const int sample = WaveSample(voice);
                const int gainLeft = voice.gainLeftQ16 >> 16;
                const int gainRight = voice.gainRightQ16 >> 16;
                left += sample * gainLeft >> 15;
                right += sample * gainRight >> 15;
                AdvanceEnvelope(voice);
            }
            if (left < -32767)
                left = -32767;
            else if (left > 32767)
                left = 32767;
            if (right < -32767)
                right = -32767;
            else if (right > 32767)
                right = 32767;
            output[frame * 2] = static_cast<short>(left);
            output[frame * 2 + 1] = static_cast<short>(right);
        }
    }

private:
    int iSampleRate;
    int iPitch;
    float iTime;
    float iTotalTime;
    unsigned int iLoopCount;
    unsigned int iSerial;
    bool iLoaded;
    TChannel iChannels[KChannelCount];
    TVoice iVoices[KVoiceCount];
    midisequencer::sequencer iSequence;
};
}

extern "C" void* EasyRpgMidiLiteCreate(const unsigned char* data, int size, int sample_rate, int pitch)
{
    if (!data || size <= 0 || sample_rate <= 0 || pitch < 50 || pitch > 200)
        return NULL;
    CMidiLite* decoder = new (std::nothrow) CMidiLite(sample_rate, pitch);
    if (!decoder)
        return NULL;
    if (!decoder->Open(data, size))
    {
        delete decoder;
        return NULL;
    }
    return decoder;
}

extern "C" void EasyRpgMidiLiteDestroy(void* decoder)
{
    delete static_cast<CMidiLite*>(decoder);
}

extern "C" int EasyRpgMidiLiteRender(void* decoder, short* output, int frames)
{
    if (!decoder)
        return 0;
    return static_cast<CMidiLite*>(decoder)->Render(output, frames);
}

extern "C" void EasyRpgMidiLiteSetPitch(void* decoder, int pitch)
{
    if (decoder)
        static_cast<CMidiLite*>(decoder)->SetPitch(pitch);
}

extern "C" unsigned int EasyRpgMidiLiteTicks(void* decoder)
{
    return decoder ? static_cast<CMidiLite*>(decoder)->Ticks() : 0;
}

extern "C" unsigned int EasyRpgMidiLiteLoopCount(void* decoder)
{
    return decoder ? static_cast<CMidiLite*>(decoder)->LoopCount() : 0;
}
