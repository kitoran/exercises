#ifndef ALSATHREAD_H
#define ALSATHREAD_H
#include <mutex>
#include "ambiguousarray.h"
#include <synthesis.h>
#include "../../library/channel/channel.h"

struct message {
    AmbiguousArray data;
    spectrogram_mode mode;
//    uint spectrumSize{0};
    bool complex{0};
};
//inline message m = {0, 0, spectrogram_mode::logarithmic, 0, false};
void startAlsathread();
inline Channel<message> channel;

#endif // ALSATHREAD_H
