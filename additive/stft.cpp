#include "stft.h"
#include <math.h>
#include <stdio.h>
#include <vector>
#include <complex>
std::vector<double> window;
const double hammingCoef = 0.53836;
extern const double tau;
extern const double frequencyMultiplent;
extern const double freqMax = 20000;
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
            double freq = freqMin * pow(frequencyMultiplent, ind);
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
    //        if(ind >= 3202) break;
        }
        if((i) %4 == 0) {
            fprintf(stderr, "%lf, %d of %d", max, i, *resW);
        }
    }
    fprintf(stderr, "max %lf maxw %lf maxh %lf", max, maxw, maxh);

}

std::vector<harmonic> maxes(double *data, int h, int w)
{
    std::vector<harmonic> r;
    r.reserve(w);
    for(int i = 0; i < w; i++) {
        double max = 0;
        int maxind = 0;
        for(int j = 0; j < h; j++) {
            if(data[i*h+j] > max) {
                max = data[i*h+j];
                maxind = j;
            }
        }
        double freq = freqMin * pow(frequencyMultiplent, maxind);
        r.push_back({freq, max});
    }
    return r;
}

void shiftandmul(double *src, int h, int w, double **dest, int* resH)
{
    max = 0;
    constexpr int lastHarmonic = 10;
    int shifts[lastHarmonic];
    for(int i = 0; i < lastHarmonic; i++) {
        shifts[i] = log(i+1)/log(frequencyMultiplent);
    }
    *resH = h - shifts[lastHarmonic-1];
    *dest = (double*)malloc(w*(*resH)*sizeof(double));
    for(int i = 0; i < w; i++) {
        for(int j = 0; j < *resH; j++) {
            double v = 1;
            for(int k = 0; k < lastHarmonic; k++) {
                v *= src[i*h + j + shifts[k]]/1000;
            }
            double treshold = 5.238856/100000;
            if(v > treshold) {
                v = log(v/treshold)+100;
            }else v = 0;
            (*dest)[i*(*resH)+j] = v;
            if(v > max) {
                max = v;
            }
        }
    }
    fprintf(stderr, "shmul max %lf", max);
}
