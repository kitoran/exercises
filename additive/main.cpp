#include <QtWidgets/QApplication>
#include <complex>
#include "graph.h"
#include "alsathread.h"
#include "stft.h"
#include "synthesis.h"
#include "mainwindow.h"
#include <sndfile.h>
extern const double frequencyMultiplent;
double *transform;

int main(int argc, char *argv[])
{
    SF_INFO inpi;
    inpi.format = 0;
    SNDFILE* inp = sf_open("/home/n/exercises/additive/02-201019_1328part.wav", SFM_READ, &inpi);

    if(inpi.channels != 1) {
        fprintf(stderr, "%d channels ! ! !", inpi.channels);
        exit(543);
    }

    sf_count_t end = sf_seek(inp, 0, SEEK_END);
    fprintf(stderr, "%ld", end);
    sf_seek(inp, 0, SEEK_SET);
    int16_t *sampls = (int16_t*)malloc(end*sizeof(int16_t));
    sf_read_short(inp, sampls, end);
    fprintf(stderr, "%ld", end);
    QApplication a(argc, argv);

    MainWindow win;
    const int windowSize = 1<<14;
    const int stepSize = 512;
    int h, w;
//    stft(sampls, end, windowSize, stepSize, inpi.samplerate, &transform, &h, &w);
    stfft(sampls, end, windowSize, stepSize, &transform, &w);
//    double *data = (double*)malloc(w*windowSize*sizeof(double));
//    isolateMaxima(w, transform, windowSize, data);

//    double* shmul;
//    int rh;
//        shiftandmul(transform, h, w, &shmul, &rh);
//    multiplyFundamentalLinear(transform, windowSize, w, &shmul, &rh);
//    win.g->setLogarithmicData(data, h, w, max, freqMin*
//                                      pow(frequencyMultiplent,
//                                            h));
    win.g->setLinearData(transform, w, h, windowSize, inpi.samplerate, max);
//    win.g->setLinearData(transform, w, windowSize, windowSize, inpi.samplerate, max);
//    win.g->setLogarithmicData(shmul, rh, w, max,
//                   freqMin*
//                   pow(frequencyMultiplent,
//                       rh));
//    resynthesizeAllLinear(shmul, w, stepSize, inpi, rh, windowSize);
//    exit(0);
    //    resynthesizeMaxima(transform, w, inpi, h);
//resynthesizeMaxima(transform, w, inpi, h);
//    resynthesizeAll(shmul, w, inpi, stepSize,rh);
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
