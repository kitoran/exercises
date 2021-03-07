#include "cetest.h"
#include <QtWidgets/QApplication>
#include "../curveeditor/graphiceditor.h"
#include "../libspline/aaCurve.h"
#include <complex>
#include "graph.h"
#include "stft.h"
#include "mainwindow.h"
#include <sndfile.h>
extern  const double tau = M_PI*2;
extern const double frequencyMultiplent;
extern const int windowSize;
extern const int stepSize;
double *transform;
int main(int argc, char *argv[])
{
    SF_INFO inpi;
    inpi.format = 0;
    SNDFILE* inp = sf_open("/home/n/exercises/additive/08-2nd voice-201025_1505-01.wav", SFM_READ, &inpi);

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
    win.g->setData(transform, h, w, max);
    win.show();

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
