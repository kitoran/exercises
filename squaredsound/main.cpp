#include <iostream>
#include <iostream>


#include <stdio.h>

#include <math.h>
#include <sndfile.h>
using namespace std;
int main()
{
    SF_INFO inpi;
    inpi.format = 0;
    SNDFILE* inp = sf_open("/home/n/exercises/squaredsound/02-201019_1328m.wav", SFM_READ, &inpi);

    SF_INFO outi = inpi;
    outi.channels = 1;
    SNDFILE* outp = sf_open("/home/n/exercises/squaredsound/02-201019_1328-sq.wav", SFM_WRITE, &outi);
    fprintf(stderr, "%d", outi.channels);
//    double freq = 440;
//    for(int i = 0; i < outi.samplerate/2; i++) {
//        double f = sin(double(i)*freq*tau/outi.samplerate);
////        double fr[2] = {f,f};
//        sf_write_double(outp, &f, 1);
//    }
//    for(int i = outi.samplerate/2; i < outi.samplerate; i++) {
//        double f = sin(double(i)*freq*tau/outi.samplerate + M_PI);
//        sf_write_double(outp, &f, 1);
//    }
    double frame;//[2];
    double max = 0;
    while(sf_read_double(inp, &frame,inpi.channels)) {
        if(fabs(frame) > max) {
            max = fabs(frame);
            fprintf(stderr, "%lf\n", frame);

        }
        double framesq = frame*frame*1.8-0.9;//{frame[0]*frame[0], frame[1]*frame[1]};

        sf_write_double(outp, &framesq, inpi.channels);
    }

    sf_close(outp);
    return 0;
}
