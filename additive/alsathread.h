#ifndef ALSATHREAD_H
#define ALSATHREAD_H
#include <mutex>
#include <synthesis.h>
inline std::mutex mut;
inline bool full = false;
struct message {
    double *data;
    uint spectrumSize;
    spectrogram_mode mode;
    uint windowSize;
};
inline message m = {0, 0, logarithmic, 0};

void startAlsathread();
#endif // ALSATHREAD_H
