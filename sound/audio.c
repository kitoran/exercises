#include "audio.h"
#include <alsa/asoundlib.h>
#include <stdio.h>

snd_pcm_t *handle;
snd_pcm_t *capture_handle;
uint alsaSampleRate = 44100;
snd_pcm_uframes_t framesPerPeriod = 512;
snd_pcm_uframes_t framesPerBuffer = 512;
void initAudioPlayback(int channels, snd_pcm_format_t format)
{
    snd_pcm_hw_params_t *params;
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
                                format);

    /*  */
    snd_pcm_hw_params_set_channels(handle, params, channels);

    /* 44100 bits/second sampling rate (CD quality) */
    snd_pcm_hw_params_set_rate_near(handle, params,
                                    &alsaSampleRate, &dir);
    fprintf(stderr, "sampleRate is %d\n", alsaSampleRate);


    int err = snd_pcm_hw_params_set_buffer_size_near(handle, params, &framesPerBuffer);
    fprintf(stderr, "\n%d returned from snd_pcm_hw_params_set_buffer_size res=%lu\n", err, framesPerBuffer );
    /* Set period size to 32 frames. */
    snd_pcm_hw_params_set_period_size_near(handle,
                                params, &framesPerPeriod, &dir);
    fprintf(stderr, "\n%lu frames per period", framesPerPeriod);

    /* Write the parameters to the driver */
    rc = snd_pcm_hw_params(handle, params);
    if (rc < 0) {
      fprintf(stderr,
              "unable to set hw parameters: %s\n",
              snd_strerror(rc));
      exit(1);
    }


}

int writeFrames(const void *buffer, int frames)
{
    int rc = snd_pcm_writei(handle, buffer, frames);
    if (rc == -EPIPE) {
        /* EPIPE means underrun */
//        fprintf(stderr, "underrun occurred\n");
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

void drainAudio()
{
    snd_pcm_drain(handle);
    snd_pcm_close(handle);
}

void alsaPlayBlock(const int16_t* buffer, uint size) {
    uint index = 0;
    while(index + framesPerBuffer <size) {
//        for(int i = 0; i < framesPerBuffer; i++) {
//            assert(fabs(buffer[index + framesPerBuffer+i]) <= 1/denominator);
//        }
        writeFrames(buffer+index, framesPerBuffer);
        index+=framesPerBuffer;
    }
    if(index < size) {
        writeFrames(buffer+index, size-index);
    }
}

void initAudioCaptureS16LE()
{
    typedef int16_t frameType;
    frameType *buffer;

    if (snd_pcm_open(&capture_handle, "default",
                     SND_PCM_STREAM_CAPTURE,
                     0) < 0) {
      abort();
    }
///    if (capture_handle, "default",
//                     SND_PCM_STREAM_CAPTURE,
//                     0) < 0) {
//      abort();
//    }
    snd_pcm_hw_params_t *hw_params;
    fprintf(stdout, "audio interface opened\n");
    if (snd_pcm_hw_params_malloc (&hw_params) < 0) {
        abort();
    }
    fprintf(stdout, "hw_params allocated\n");
    if(snd_pcm_hw_params_any (capture_handle, hw_params)<0) {
        abort();
    }
    if(snd_pcm_hw_params_set_access (
                capture_handle, hw_params,
                SND_PCM_ACCESS_RW_INTERLEAVED) <0) {
        abort();
    }
    if(snd_pcm_hw_params_set_format
            (capture_handle, hw_params, SND_PCM_FORMAT_S16_LE) < 0) {
        abort();
    }
    if(snd_pcm_hw_params_set_rate_near
            (capture_handle, hw_params, &alsaSampleRate, 0) < 0) {
        abort();
    }
    if(snd_pcm_hw_params_set_channels
            (capture_handle, hw_params, 1) < 0) {
        abort();
    }
    if(snd_pcm_hw_params (capture_handle, hw_params) < 0) {
        abort();
    }
    snd_pcm_hw_params_free (hw_params);
    if(snd_pcm_prepare (capture_handle) < 0) {
        abort();
    }
    buffer = (frameType *) malloc(sizeof(frameType)*framesPerPeriod);


}

void alsaRecordBlock(int16_t *buffer, uint size)
{
    snd_pcm_readi (capture_handle, buffer, size);
}

void alsaDropCapture()
{
    snd_pcm_close(capture_handle);
}
