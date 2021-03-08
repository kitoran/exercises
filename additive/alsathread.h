#ifndef ALSATHREAD_H
#define ALSATHREAD_H
#include <mutex>
#include <synthesis.h>
inline std::mutex mut;
inline bool full = false;
struct message {
    double *data;
    uint h;
    spectrogram_mode mode;
};
inline message m = {0, 0, logarithmic};

void startAlsathread();
#endif // ALSATHREAD_H
