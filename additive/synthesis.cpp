#include "synthesis.h"


void resynthesizeOne(double *data, int w, SF_INFO inpi, int h)
{
    std::vector<harmonic> maxe = maxes(data, h, w);

    SF_INFO outi = inpi;
    SNDFILE* out = sf_open("/home/n/exercises/additive/resynth.wav", SFM_WRITE, &outi);
    double phase = 0;
    for(int i = 0; i < w-1; i++) {
        double maxfreqp = maxe[i].freq;
        double maxfreqn = maxe[i+1].freq;
        double maxampp = maxe[i].amp;
        double maxampn = maxe[i+1].amp;
        for(int s = 0; s < stepSize; s++) {
            double freq = (maxfreqp * (stepSize-s) + maxfreqn * (s))/stepSize;
            double amp = (maxampp * (stepSize-s) + maxampn * (s))/stepSize/max;
            double value = sin(phase)*amp;
            phase += tau*freq/outi.samplerate;
            sf_write_double(out, &value, 1);
        }
        if((i) %4 == 0) {
            fprintf(stderr, "hey %d of %d", i, w);
        }

    }
    sf_close(out);
}

void resynthesizeAll(double *data, int w, SF_INFO inpi, int h)
{
    SF_INFO outi = inpi;
    SNDFILE* out = sf_open("/home/n/exercises/additive/resynthAllFlute.wav", SFM_WRITE, &outi);

    //    double phases[h] = {0};
    double frequencies[h] = {0};
    for(int i = 0; i < h; i++) {
        double mul = pow(frequencyMultiplent, i);
        double prod = freqMin * mul;
        frequencies[i] = prod;
    }
    //    double phases[h] = {0};
    for(int i = 0; i < w-1; i++) {
        for(int s = 0; s < stepSize; s++) {
            double v = 0;
            for(int j = 0; j < h; j++) {
                //                double freq = frequencies[j];
                double amp = (data[i*h+j] * (stepSize-s) + data[(i+1)*h+j] * (s))/stepSize/max/100;
                v += sin(tau*frequencies[j]/inpi.samplerate*(i*stepSize+s))*amp;
                //                phase += tau*freq/outi.samplerate;
                //                phases[j] += frequencies[i] = freqMin * pow(frequencyMultiplent, ind);
            }
            sf_write_double(out, &v, 1);
        }
        if((i) %4 == 0) {
            fprintf(stderr, "hey %d of %d", i, w);
        }

    }
    sf_close(out);
}

void resynthesizeMaxima(double *data, int w, SF_INFO inpi, int h)
{
    isolateMaxima();
    resynthesizeAll();
}
