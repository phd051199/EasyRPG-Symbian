#include "CharsetDetector.h"

#include <algorithm>

#include "nsBig5Prober.h"
#include "nsGB2312Prober.h"
#include "nsSJISProber.h"
#include "nsUTF8Prober.h"

namespace
{

void AddGuess(std::vector<EasyRpgCharsetDetector::Guess> &guesses,
              nsCharSetProber &prober,
              const char *encoding,
              const std::string &sample)
{
    const nsProbingState state = prober.HandleData(
        sample.data(),
        static_cast<PRUint32>(sample.size()));
    if (state == eNotMe)
        return;

    float confidence = prober.GetConfidence();
    if (state == eFoundIt && confidence < 0.99f)
        confidence = 0.99f;
    if (confidence <= 0.01f)
        return;

    EasyRpgCharsetDetector::Guess guess;
    guess.encoding = encoding;
    guess.confidence = confidence;
    guesses.push_back(guess);
}

bool HigherConfidence(const EasyRpgCharsetDetector::Guess &left,
                      const EasyRpgCharsetDetector::Guess &right)
{
    return left.confidence > right.confidence;
}

} // namespace

std::vector<EasyRpgCharsetDetector::Guess>
EasyRpgCharsetDetector::Detect(const std::string &sample)
{
    std::vector<Guess> guesses;
    if (sample.empty())
        return guesses;

    nsUTF8Prober utf8;
    nsSJISProber shift_jis(PR_FALSE);
    nsGB18030Prober gb18030(PR_FALSE);
    nsBig5Prober big5(PR_FALSE);

    AddGuess(guesses, utf8, "UTF-8", sample);
    AddGuess(guesses, shift_jis, "SHIFT_JIS", sample);
    AddGuess(guesses, gb18030, "GBK", sample);
    AddGuess(guesses, big5, "BIG5", sample);

    std::stable_sort(guesses.begin(), guesses.end(), HigherConfidence);
    return guesses;
}
