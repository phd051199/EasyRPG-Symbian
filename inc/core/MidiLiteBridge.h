#ifndef EASYRPG_MIDI_LITE_BRIDGE_H
#define EASYRPG_MIDI_LITE_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

void* EasyRpgMidiLiteCreate(const unsigned char* data, int size, int sample_rate, int pitch);
void EasyRpgMidiLiteDestroy(void* decoder);
int EasyRpgMidiLiteRender(void* decoder, short* output, int frames);
void EasyRpgMidiLiteSetPitch(void* decoder, int pitch);
unsigned int EasyRpgMidiLiteTicks(void* decoder);
unsigned int EasyRpgMidiLiteLoopCount(void* decoder);

#ifdef __cplusplus
}
#endif

#endif
