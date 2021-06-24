#ifndef ALSATHREAD_H
#define ALSATHREAD_H
#include <mutex>
#include "ambiguousarray.h"
//#include <synthesis.h>
#include <spectrogram.h>
#include "../../library/channel/channel.h"

struct message {
//    Spectrum* data;
//    uint spectrumSize{0};
//    bool complex{0};
    int pos;
};
//inline message m = {0, 0, spectrogram_mode::logarithmic, 0, false};
void startAlsathread();
inline Channel<message> channel;

#endif // ALSATHREAD_H
