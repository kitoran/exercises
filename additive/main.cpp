#include "cetest.h"
#include <QtWidgets/QApplication>
#include "../curveeditor/graphiceditor.h"
#include "../libspline/aaCurve.h"
#include <complex>
#include "graph.h"
#include "alsathread.h"
#include "stft.h"
#include "mainwindow.h"
#include <sndfile.h>
extern  const double tau = M_PI*2;
extern const double frequencyMultiplent;
extern const int windowSize;
extern const int stepSize;
extern const double freqMin;
double *transform;
void resynthesize(double* data, int w, SF_INFO inpi, int h)
{
    std::vector<harmonic> maxe = maxes(data, h, w);

    SF_INFO outi = inpi;
    SNDFILE* out = sf_open("/home/n/exercises/additive/resynth.wav", SFM_WRITE, &outi);

//    double phases[h] = {0};
//    double frequencies[h] = {0};
//    for(int i = 0; i < h; i++) {
//        double mul = pow(frequencyMultiplent, i);
//        double prod = freqMin * mul;
//        frequencies[i] = prod;
//    }
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
//                phases[j] += valuefrequencies[i] = freqMin * pow(frequencyMultiplent, ind);
            sf_write_double(out, &value, 1);
        }
        if((i) %4 == 0) {
            fprintf(stderr, "hey %d of %d", i, w);
        }

    }
    sf_close(out);
}
void resynthesizeAll(double* data, int w, SF_INFO inpi, int h)
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
void resynthesizeMaxima(double* olddata, int w, SF_INFO inpi, int h)
{
    SF_INFO outi = inpi;
    SNDFILE* out = sf_open("/home/n/exercises/additive/resynthMaximaFlute.wav", SFM_WRITE, &outi);

//    double phases[h] = {0};
    double frequencies[h] = {0};
    for(int i = 0; i < h; i++) {
        double mul = pow(frequencyMultiplent, i);
        double prod = freqMin * mul;
        frequencies[i] = prod;
    }
    double *data = (double*)malloc(w*h*sizeof(double));
    for(int i = 0; i < w; i++) {
        for(int j = 0; j < h; j++) {
            if(j == 0 || j == h-1 || olddata[i*h+(j-1)] > olddata[i*h+(j)] ||
                    olddata[i*h+(j+1)] > olddata[i*h+(j)] ||
                    max/255 > olddata[i*h+(j)]) {
                data[i*h+(j)] = 0;
            } else {
                data[i*h+(j)] = olddata[i*h+(j)];
            }
        }
        if((i) %4 == 0) {
            fprintf(stderr, "hey %d of %d", i, w);
        }
    }
//    double phases[h] = {0};
    for(int i = 0; i < w-1; i++) {
        for(int s = 0; s < stepSize; s++) {
            double v = 0;
            for(int j = 1; j < h-1; j++) {
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

int main(int argc, char *argv[])
{
    SF_INFO inpi;
    inpi.format = 0;
    SNDFILE* inp = sf_open("/home/n/exercises/additive/354672__mtg__flute-d4.wav", SFM_READ, &inpi);

    if(inpi.channels != 1) {
        fprintf(stderr, "%d channels ! ! !", inpi.channels);
        exit(543);
    }

    sf_count_t end = sf_seek(inp, 0, SEEK_END);
    fprintf(stderr, "%ld", end);
    sf_seek(inp, 0, SEEK_SET);
    int16_t *sampls = (int16_t*)malloc(end*sizeof(int16_t));
    sf_read_short(inp, sampls, end);
    fprintf(stderr, "%d", end);
//    int ind = 0;
//    int maxind = 0;
//    double maxfreq = 0;
//    int size = ceil(log(20000.0/30.0)/log(frequencyMultiplent));
//    transform = (double*)calloc(size, sizeof(double));
//    for(; ind < size; ind++) {
//        double freq = 30 * pow(frequencyMultiplent, ind);
//        std::complex<double> acc = 0;
//        const std::complex<double> primroot = std::polar(1.0, (double)tau*freq/samplerate);
//        std::complex<double> root = 1;
//        for(int i = 0; i < end; i++) {
//            acc += root * ((double)(sampls[i])/1000.);
//            root *= primroot;
//        }
//        transform[ind] = abs(acc);
//        if(max < transform[ind]) {
//            max = transform[ind];
//            maxind = ind;
//            maxfreq = freq;
//        }
//        if(ind %16 == 0) {
//            fprintf(stderr, "%lf %d %lf", freq, ind, max);
//        }
////        if(ind >= 3202) break;
//    }
//    fprintf(stderr, "max %lf ind %d mind %d mfreq %lf", max, ind, maxind, maxfreq);

    QApplication a(argc, argv);

    MainWindow win;
    int h, w;
    stft(sampls, end, windowSize, stepSize, inpi.samplerate, &transform, &h, &w);
    double *data = (double*)malloc(w*h*sizeof(double));
    for(int i = 0; i < w; i++) {
        for(int j = 0; j < h; j++) {
            if(j == 0 || j == h-1 || transform[i*h+(j-1)] > transform[i*h+(j)] ||
                    transform[i*h+(j+1)] > transform[i*h+(j)] ||
                    max/255 > transform[i*h+(j)]) {
                data[i*h+(j)] = 0;
            } else {
                data[i*h+(j)] = transform[i*h+(j)];
            }
        }
        if((i) %4 == 0) {
            fprintf(stderr, "hey %d of %d", i, w);
        }
    }

//    double* shmul;
//    int rh;
//    shiftandmul(transform, h, w, &shmul, &rh);
        win.g->setData(data, h, w, max, freqMin*
                                          pow(frequencyMultiplent,
                                              h));
//    win.g->setData(shmul, rh, w, max,
//                   freqMin*
//                   pow(frequencyMultiplent,
//                       rh));

//    resynthesizeMaxima(transform, w, inpi, h);
resynthesizeMaxima(transform, w, inpi, h);
resynthesizeAll(transform, w, inpi, h);
    win.show();
    startAlsathread();
//    std::vector<aaAaa::aaSpline> splines;

//    aaAaa::aaSpline spline;
//    spline.addKnots(aaAaa::aaPoint(0, 0));
//    spline.addKnots(aaAaa::aaPoint(128, 150));
//    spline.addKnots(aaAaa::aaPoint(255, 255));

//    spline.name = "R";
//    splines.push_back(spline);

//    spline.name = "G";
//    spline.setLimit(0, 255, 255, 0);
//    splines.push_back(spline);

//    graphiceditor ge;

//    ge.setSplines(splines);


//    ge.show();
     return a.exec();
}
