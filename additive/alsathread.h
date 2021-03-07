#ifndef ALSATHREAD_H
#define ALSATHREAD_H
#include <mutex>
inline std::mutex mut;
inline bool full = false;
struct message {
    double *data;
    int h;
};
inline message m = {0, -1};

void startAlsathread();
#endif // ALSATHREAD_H
