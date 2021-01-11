#ifndef ALSA_H
#define ALSA_H
#include <inttypes.h>
struct stereo16 {
    int16_t l;
    int16_t r;
};
void init();
void drain();
int writeFrames(stereo16* buffer, int frames);
#endif // ALSA_H
