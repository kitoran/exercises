#include <stdio.h>
#include <sndfile.h>
//#include <libexplain/timerfd_create.h>
#include <math.h>
#include "audio.h"

#include <signal.h>           /* Definition of SIGEV_* constants */
#include <time.h>


struct array {
    int64_t size;
    int16_t* data;
};

array readSamples() {
    int16_t* samples;
    struct SF_INFO tickInfo; // we know all the info in advance
    SNDFILE* sndTick = sf_open("Korg-N1R-Metronome-Click.wav", SFM_READ, &tickInfo);
    sf_count_t end = sf_seek(sndTick, 0, SEEK_END);
    fprintf(stderr, "%ld", end);
    sf_seek(sndTick, 0, SEEK_SET);
    int sizeOfSamplsInBytes = end*sizeof(int16_t);
    samples = (int16_t*)malloc(sizeOfSamplsInBytes);
    sf_read_short(sndTick, samples, end);
    return {end, samples};
}

void notify_function(union sigval s) {
    array* samples = (array*)(s.sival_ptr);
    alsaPlayBlock(samples->data, samples->size);
}



int main(int argc, char** argv)
{
    if(argc == 1) {
        printf("need temp\n");
        return 1;
    }
    double tempo;
    sscanf(argv[1], "%lf", &tempo);
    double wholeInterval = 60/tempo;
    array samples = readSamples();

    initAudio(2, SND_PCM_FORMAT_S16_LE);

    struct sigevent sev;
    sev.sigev_notify = SIGEV_THREAD;
    //sev.sigev_signo doesn't need initializatin if i understand correctly
    sev.sigev_value.sival_ptr = &samples;
    sev.sigev_notify_function = notify_function;
    sev.sigev_notify_attributes = NULL;
    // sev.sigev_notify_thread_id doesn't exist and probably not needed and unionized
    // with sigev_notify_function so i shouldn't set it
    timer_t timerId;
    int64_t r = timer_create(CLOCK_REALTIME, &sev, &timerId);
    printf("timer_create returned %ld timerId is %p\n", r, timerId);
    struct timespec initial = {1,1};
    struct timespec interval;
    double fractionalPart;
    double integralPart;
    fractionalPart = modf(wholeInterval, &integralPart);
    interval.tv_sec = integralPart;
    interval.tv_nsec = fractionalPart*1000000000;

    printf("tv_sec %ld tv_nsec %ld\n", interval.tv_sec, interval.tv_nsec);

    struct itimerspec t = {interval, initial};
//    struct itimerspec o;
    r = timer_settime(timerId, 0, &t, NULL);
    perror("timer_settime");
    printf("timer_settime returned %ld t.\n", r);
    sleep(INT32_MAX);

    return 0;
}
