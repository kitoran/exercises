#ifndef ALSATHREAD_H
#define ALSATHREAD_H
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
extern struct Channel channel;

extern bool recordingInAThread;
void startrRecordingInAThread(int16_t** data);

#endif // ALSATHREAD_H
