#include "stft.h"
#include <math.h>
#include <stdio.h>
#include <vector>
#include <complex>
std::vector<double> window;
const double hammingCoef = 0.53836;
extern const double tau;
extern const double frequencyMultiplent;
extern const double freqMax;
extern const double freqMin;

void stft(int16_t *data, int size, int windowSize, int step, int samplerate, double **res, int *resH, int *resW)
{
    window.resize(windowSize);
    double anotherCoef = 1 - hammingCoef;
    for(int i = 0; i < windowSize; i++) {
//        int n = i-windowSize/2;
        window[i] = hammingCoef - anotherCoef*cos(tau*i/windowSize);
    }
    *resH = ceil(log(freqMax/freqMin)/log(frequencyMultiplent));
    *resW = ((size) - windowSize)/step;
    *res = (double*)malloc((*resW)*(*resH)*sizeof(double));
    fprintf(stderr, "\n%d bytes for doubles\n", (*resW)*(*resH)*sizeof(double));
//    exit(0);
    double maxw;
    double maxh;
    std::vector<double> datum; datum.resize(windowSize);
    for(int i = 0; i < *resW; i++) {
        for(int n = 0; n < windowSize; n++) {
            datum[n] = window[n] * data[i*step+n];
        }
        for(int ind = 0; ind < *resH; ind++) {
            double freq = 30 * pow(frequencyMultiplent, ind);
            std::complex<double> acc = 0;
            const std::complex<double> primroot = std::polar(1.0, (double)tau*freq/samplerate);
            std::complex<double> root = 1;
            for(int j = 0; j < windowSize; j++) {
                acc += root * ((double)(datum[j])/1000.);
                root *= primroot;
            }
            (*res)[i*(*resH) + ind] = abs(acc);
            if(max < (*res)[i*(*resH) + ind]) {
                max = (*res)[i*(*resH) + ind];
                maxw = i;
                maxh = ind;
            }
            if((i*(*resH) + ind) %16 == 0) {
                fprintf(stderr, "%lf %d %lf", freq, ind, max);
            }
    //        if(ind >= 3202) break;
        }
    }
    fprintf(stderr, "max %lf maxw %lf maxh %lf", max, maxw, maxh);

}
