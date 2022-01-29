#ifndef ALSA_H
#define ALSA_H
#include <alsa/asoundlib.h>
#include <inttypes.h>
#ifdef __cplusplus
extern "C" {
#endif
extern snd_pcm_uframes_t framesPerBuffer;
extern uint alsaSampleRate;
void initAudioPlayback(int channels, snd_pcm_format_t format);
void initAudioCaptureS16LE();
void drainAudio();
int writeFrames(const void *buffer, int frames);
void alsaPlayBlock(const int16_t* buffer, uint size);
void alsaRecordBlock(int16_t* buffer, uint size);
#ifdef __cplusplus
}
#endif
#endif // ALSA_H
