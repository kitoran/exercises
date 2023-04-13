#include "playback.h"
#include <SDL_audio.h>
#include <SDL_events.h>
#include <SDL.h>
#include "stb_ds.h"
#include "extmath.h"
#include "misc.h"
#include <stdbool.h>
#include <stdio.h>

double freqs[2]={0};
bool playing = false;
char DebugBuffer[1000];
SDL_AudioSpec  have = {.freq = 44100};

int countSamples(double time) {
    return (int)round(time * have.freq);
}
SDL_AudioDeviceID audioDevice;
void buffer(void* userdata,
            Uint8* stream8,
            int wrongLen)
{
    int trueLen = wrongLen/2;

    i16* stream = (i16*)stream8;
    while(trueLen > 0) {
        int samplesToFillNow = trueLen;

        static u32 phase[2] = {0};
        for(int i = 0; i < samplesToFillNow; i++) {
            stream[i] = 0;
            FOR(j, 2) {
                FOR(t, 5) {
                    stream[i] += (i16)round(sin(phase[j]*(t+1)/4.0*tau/(SDL_MAX_UINT32/4))*1000/(t+1)/(t+1));
                }
//                if(state.framesLeftLastFreq[j] == 0) {
                phase[j] += (u32)round(SDL_MAX_UINT32/((double)have.freq)*(freqs[j]));
//                } else {
//                    phase[j] +=
//                             ((SDL_MAX_UINT32/((double)have.freq)*(state.lastFreq[j])))*
//                                    (state.framesLeftLastFreq[j]/(double)EASE_FRAMES)
//                            + (SDL_MAX_UINT32/((double)have.freq)*(state.freq[j]))*
//                                    (1-state.framesLeftLastFreq[j]/(double)EASE_FRAMES);
//                    state.framesLeftLastFreq[j]--;
//                }
            }
        }
        stream += samplesToFillNow;
        trueLen -= samplesToFillNow;
    }
    SDL_UserEvent userevent = {PlaybackEvent, SDL_GetTicks(), 0, 0, 0, 0};
    SDL_Event event; event.user = userevent;
    SDL_PushEvent(&event);
}

int PlaybackEvent;
void openAudio() {
    SDL_Init(SDL_INIT_AUDIO);
    PlaybackEvent = SDL_RegisterEvents(1);
    SDL_AudioSpec  want;
    memset(&want,0,sizeof(want));

    want.freq = 44100;
    want.format = AUDIO_S16;
    want.channels = 1;
    want.samples = 1024;
    want.callback = buffer;

    audioDevice =  SDL_OpenAudioDevice(
                NULL,
                SDL_FALSE,
                &want, &have,
                0);
    fprintf(stderr,"%s, returend %d %d %d %d %d %d \n",
            SDL_GetError(), want.format, want.channels, want.samples,have.format, have.channels, have.samples);

    SDL_PauseAudioDevice(audioDevice, 0);
}

double samplesToTime(int samples)
{
    return samples*1.0/have.freq;
}

int timeToSamples(double time)
{
    return (int)round(time*have.freq);
}
