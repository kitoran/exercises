#ifndef ALSATHREAD_H
#define ALSATHREAD_H
#include <mutex>
#include <synthesis.h>
inline std::mutex mut;
inline bool full = false;
struct message {
    void *data;
    uint spectrumSize;
    spectrogram_mode mode;
    uint windowSize{0};
    bool complex{0};
};
inline message m = {0, 0, logarithmic, 0, false};

void startAlsathread();
#endif // ALSATHREAD_H
