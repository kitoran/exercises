#include "audio.h"
#include <alsa/asoundlib.h>
#include <stdio.h>

snd_pcm_t *handle;
uint sampleRate = 44100;
snd_pcm_uframes_t framesPerPeriod = 32;
snd_pcm_uframes_t framesPerBuffer = 256;
void init()
{
    snd_pcm_hw_params_t *params;
    unsigned int val;
    int dir;
    int rc;
    /* Open PCM device for playback. */
    rc = snd_pcm_open(&handle, "default",
                      SND_PCM_STREAM_PLAYBACK
                      ,  0/*SND_PCM_NONBLOCK*/);
    if (rc < 0) {
      fprintf(stderr,
              "unable to open pcm device: %s\n",
              snd_strerror(rc));
      exit(1);
    }

    /* Allocate a hardware parameters object. */
    snd_pcm_hw_params_alloca(&params);

    /* Fill it in with default values. */
    snd_pcm_hw_params_any(handle, params);

    /* Set the desired hardware parameters. */

    /* Interleaved mode */
    snd_pcm_hw_params_set_access(handle, params,
                        SND_PCM_ACCESS_RW_INTERLEAVED);

    /* Signed 16-bit little-endian format */
    snd_pcm_hw_params_set_format(handle, params,
                                SND_PCM_FORMAT_S16_LE);

    /* Two channels (stereo) */
    snd_pcm_hw_params_set_channels(handle, params, 2);

    /* 44100 bits/second sampling rate (CD quality) */
    snd_pcm_hw_params_set_rate_near(handle, params,
                                    &sampleRate, &dir);
    fprintf(stderr, "sampleRate is %d\n", sampleRate);


    /* Set period size to 32 frames. */
    snd_pcm_hw_params_set_period_size_near(handle,
                                params, &framesPerPeriod, &dir);
    fprintf(stderr, "\n%d frames per period", framesPerPeriod);
    int err = snd_pcm_hw_params_set_buffer_size_near(handle, params, &framesPerBuffer);
    fprintf(stderr, "\n%d returned from snd_pcm_hw_params_set_buffer_size res=%d\n", err, framesPerBuffer );

    /* Write the parameters to the driver */
    rc = snd_pcm_hw_params(handle, params);
    if (rc < 0) {
      fprintf(stderr,
              "unable to set hw parameters: %s\n",
              snd_strerror(rc));
      exit(1);
    }


}

int writeFrames(stereo16 *buffer, int frames)
{
    int rc = snd_pcm_writei(handle, buffer, frames);
    if (rc == -EPIPE) {
        /* EPIPE means underrun */
        fprintf(stderr, "underrun occurred\n");
        snd_pcm_prepare(handle);
    } else if (rc < 0) {
        fprintf(stderr,
                "error from writei: %s\n",
                snd_strerror(rc));
    }  else if (rc != (int)frames) {
        fprintf(stderr,
                "short write, write %d frames\n", rc);
    }
    return 0;
}

void drain()
{
    snd_pcm_drain(handle);
    snd_pcm_close(handle);
}
