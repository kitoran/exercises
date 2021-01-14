#ifndef ALSA_H
#define ALSA_H
#include <alsa/asoundlib.h>
#include <inttypes.h>
struct stereo16 {
    int16_t l;
    int16_t r;
};
extern snd_pcm_uframes_t framesPerBuffer;
void init();
void drain();
int writeFrames(stereo16* buffer, int frames);
#endif // ALSA_H
