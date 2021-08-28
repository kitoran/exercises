#include <gtk/gtk.h>
#include "graph.h"
#include "alsathread.h"
#include "stft.h"
#include "globals.h"
#include "synthesis.h"
#include "mainwindow.h"
#include <sndfile.h>
#include <openssl/md5.h>
#include <storearray.h>
#include "mathext.h"
int main(int argc, char *argv[])
{
    initializeMathExt();
    startAlsathread();

    inpi.format = 0;
    SNDFILE* inp = sf_open("/home/n/exercises/additive/02-201019_1328part.wav", SFM_READ, &inpi);

    if(inpi.channels != 1) {
        fprintf(stderr, "%d channels ! ! !", inpi.channels);
        exit(543);
    }

    sf_count_t end = sf_seek(inp, 0, SEEK_END);
    fprintf(stderr, "%ld", end);
    sf_seek(inp, 0, SEEK_SET);
    int sizeOfSamplsInBytes = end*sizeof(int16_t);
    sampls = (int16_t*)malloc(sizeOfSamplsInBytes);
    sf_read_short(inp, sampls, end);
    fprintf(stderr, "%ld", end);

    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_show(window);

    g_signal_connect(window, "destroy",
        G_CALLBACK(gtk_main_quit), NULL);

    gtk_main();

    struct MainWindow win;
//    stft(sampls, end, windowSize, stepSize, inpi.samplerate, &transform, &h, &w);
    //    stfft(sampls, end, windowSize, stepSize, &transform, &w);
//    std::complex< double>* transform;
    unsigned char hassh[MD5_DIGEST_LENGTH];
    MD5((unsigned char*)(sampls), sizeOfSamplsInBytes, hassh);

//    complex_stfft(sampls, end, windowSize, stepSize, &transform, &w);

//    std::complex< double>* transform2;

//    load(hassh, "complex_stfft", 1, (void**)(&transform2));

//    double* maxp2;

//    load(hassh, "complex_stfft_max", 1, (void**)(&maxp2));

#define STR2(a) #a
#define STR(a) STR2(a)

#define FUNCTION stfft
//    int size;
//    if(/*true || */!load(hassh, STR(FUNCTION), 1, (void**)(&transform), &size)) {
        FUNCTION(sampls, end, windowSize, stepSize, /*inpi.samplerate, */&originalFourierTransform, /*&h, */&originalFourierTransformW);
        originalFourierTransformH = windowSize;
//        save((transform), (w)*(h)*sizeof(*transform),
//             hassh, STR(FUNCTION), 1 );
        fprintf(stderr, "%d, %d, %lu, %lu", originalFourierTransformW,
                        originalFourierTransformH,
                sizeof(*originalFourierTransform),
                (originalFourierTransformW)*(originalFourierTransformH)*sizeof(*originalFourierTransform));

        double originalMax = max;

        double* shifted;
        int shiftedH;

        shiftandmulLinear(originalFourierTransform, originalFourierTransformH, originalFourierTransformW, &shifted, &shiftedH);
        int hms;
        //    std::vector<std::vector<continuousHarmonic> >  contharms = prepareHarmonics(maxesLinear(transform, h, w, inpi.samplerate), &hms);
        struct continuousHarmonic** contharmsStbArray  = prepareHarmonicsStbArray(maxesLinear(shifted, shiftedH, originalFourierTransformW, inpi.samplerate), &hms);
//        resynthesizeMaxima(maxesLinear(shifted, shiftedH, w, inpi.samplerate), stepSize, inpi, windowSize);
//        resynthesizeMaxima(contharms, hms);
//        exit(0);
//        auto mspectrogram = new LinearSpectrogram(originalMax, transform, w, h, inpi.samplerate/windowSize);
//        auto mspectrogram = new LinearSpectrogram(max, shifted, w, shiftedH, inpi.samplerate/windowSize);
//                auto mspectrogram = new MaximaSpectrogram(max, maxesLinear(transform, h, w, inpi.samplerate));
//        auto mspectrogram = new MaximaSpectrogram(max, maxesLinear(shifted, shiftedH, w, inpi.samplerate));
        //        auto mspectrogram = new ContMaximaSpectrogram(max,
        //                prepareHarmonics(maxesLinear(shifted, shiftedH, w, inpi.samplerate), &hms));
//                auto mspectrogram = new MaximaSpectrogram(max, maxesLinear(transform, h, w, inpi.samplerate));
        struct Spectrogram* mspectrogram = newContMaximaSpectrogram(max,
                        contharmsStbArray, hms);
        spectrogram = mspectrogram;
//        save(&max, sizeof(max),
//                                              hassh,  STR(FUNCTION) "_max", 1 );
//        save(&h, sizeof(h),
//                                              hassh,  STR(FUNCTION) "_h", 1 );
//    } else {
//        double* maxp;
//        typeof(h) * hp;
//        makeHammingWindow(windowSize);
//        w = ((end) - windowSize)/stepSize;
//        loadOne(hassh, STR(FUNCTION) "_max", 1, &max);
//        loadOne(hassh, STR(FUNCTION) "_h", 1, &h);
//        qDebug() << size  << w << h << sizeof(*transform) << (w)*(h)*sizeof(*transform);
//        Q_ASSERT(size == (w)*(h)*sizeof(*transform));
//        max = *maxp;
//        h = *hp;
//       i'm not gonna free maxp
//    }

//    return 0;


//    double *data = (double*)malloc(w*windowSize*sizeof(double));
//    isolateMaxima(w, transform, windowSize, data);

//    double* shmul;
//    int rh;
//        shiftandmul(transform, h, w, &shmul, &rh);
//    multiplyFundamentalLinear(transform, windowSize, w, &shmul, &rh);
//    win.g->setLogarithmicData(transform, h, w, max, freqMin*
//                                      pow(frequencyMultiplent,
//                                            h));
//    win.g->setComplexData(transform, w, windowSize, windowSize, inpi.samplerate, max);
//    win.g->setData(transform, w, h, windowSize, inpi.samplerate, max);
//    win.g->setLinearData(transform, w, h, windowSize, inpi.samplerate, max);
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
     return 0;
}
