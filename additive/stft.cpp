#include "stft.h"
#include <math.h>
#include <algorithm>
#include "mathext.h"
#include "globals.h"
#include <stdio.h>
#include <vector>
#include <complex>

const double hammingCoef = 0.53836;

void makeHammingWindow(int windowSize)
{
    window.resize(windowSize);
    double anotherCoef = 1 - hammingCoef;
    for(int i = 0; i < windowSize; i++) {
//        int n = i-windowSize/2;
        window[i] = hammingCoef - anotherCoef*cos(tau*i/windowSize);
    }
}

void stft(int16_t *data, int size, int windowSize, int step, int samplerate, double **res, int *resH, int *resW)
{
    makeHammingWindow(windowSize);
    *resH = ceil(log(freqMax/freqMin)/log(frequencyMultiplent));

//    double frequencies[*resH] = {0};
//    for(int i = 0; i < *resH; i++) {
//        double mul = pow(frequencyMultiplent, i);
//        double prod = freqMin * mul;
//        frequencies[i] = prod;
//    }
    std::complex<double> primroots[*resH];
    for(int i = 0; i < *resH; i++) {
        double mul = pow(frequencyMultiplent, i);
        double prod = freqMin * mul;
        primroots[i] = std::polar(1.0, (double)tau*prod/samplerate);
    }
    *resW = ((size) - windowSize)/step;
    *res = (double*)malloc((*resW)*(*resH)*sizeof(double));
    fprintf(stderr, "\n%ld bytes for doubles\n", (*resW)*(*resH)*sizeof(double));

//    return;
//    exit(0);

    double maxw;
    double maxh;
    std::vector<double> datum; datum.resize(windowSize);
    for(int i = 0; i < *resW; i++) {
        for(int n = 0; n < windowSize; n++) {
            datum[n] = window[n] * data[i*step+n];
        }
        for(int ind = 0; ind < *resH; ind++) {
            std::complex<double> acc = 0;
            const std::complex<double> primroot = primroots[ind];
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
        if((i) %64 == 0) {
            fprintf(stderr, "%lf, %d of %d", max, i, *resW);
        }
    }
    fprintf(stderr, "max %lf maxw %lf maxh %lf", max, maxw, maxh);
}

void stfft(int16_t *data, int size, int windowSize, int step, double **res, int *resW)
{
    max = 0;
    makeHammingWindow(windowSize);
    *resW = ((size) - windowSize)/step;
    *res = (double*)malloc((*resW)*(windowSize)*sizeof(double));
    std::vector<double> datum; datum.resize(windowSize);
    for(int i = 0; i < *resW; i++) {
        for(int n = 0; n < windowSize; n++) {
            datum[n] = window[n] * data[i*step+n];
        }
        fft(/*data + step*i*/datum.data(), windowSize, (*res) + i*windowSize);
        if((i) %64 == 0) {
            fprintf(stderr, "%lf, %d of %d", max, i, *resW);
        }
    }
    fprintf(stderr, "max %lf ", max);
}

void complex_stfft(int16_t *data, int size, int windowSize, int step, std::complex<double> **res, int *resW)
{
    max = 0;
    makeHammingWindow(windowSize);
    *resW = ((size) - windowSize)/step;
    *res = (std::complex<double>*)malloc((*resW)*(windowSize)*sizeof(std::complex<double>));
    std::vector<double> datum; datum.resize(windowSize);
    for(int i = 0; i < *resW; i++) {
        for(int n = 0; n < windowSize; n++) {
            datum[n] = window[n] * data[i*step+n];
        }
        complex_fft(data + step*i, windowSize, (*res) + i*windowSize);
        if((i) %4 == 0) {
            fprintf(stderr, "%lf, %d of %d", max, i, *resW);
        }
    }
    fprintf(stderr, "max %lf ", max);
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

std::vector<std::vector<harmonic>> maxesLinear(double *data, int h, int w, int samplerate)
{
    std::vector<std::vector<harmonic>> r;
    r.reserve(w);
    for(int i = 0; i < w; i++) {
        std::vector<harmonic> e;
        for(int j = 1; j < h-1; j++) {
            double freq = double(samplerate)/h*j;
            if(data[i*h+j] > data[i*h+j-1]
                 && data[i*h+j] > data[i*h+j+1]
                 && freq < 20000
                 && data[i*h+j] > max/200
                    ) {
                e.push_back({freq, data[i*h+j]});
            }
        }
        std::sort(e.begin(), e.end(), [](harmonic h1, harmonic h2) {
            return h1.amp > h2.amp; });
//        if(e.size() > 15) e.resize(1);
        std::sort(e.begin(), e.end(), [](harmonic h1, harmonic h2) {
            return h1.freq < h2.freq; });
        r.push_back(e);
    }
    return r;
}

void shiftandmul(double *src, int h, int w, double **dest, int* resH)
{
    max = 0;
    constexpr int lastHarmonic = 14;
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

void shiftandmulLinear(double *src, int h, int w, double **dest, int *resH)
{
//    double lastmax = max;
    max = 0;
    constexpr int lastHarmonic =2;
    *resH = h/lastHarmonic;
    *dest = (double*)malloc(w*(*resH)*sizeof(double));
    for(int i = 0; i < w; i++) {
        for(int j = 0; j < *resH; j++) {
            double v = 1;
            for(int k = 0; k < lastHarmonic; k++) {
                if(j*(k+1) < w/2) {
                    v *= src[i*h + j*(k+1)]/500000;
                }
            }
            double treshold = 455.500326/10000;
//            if(v > treshold) {
                v = log(v/treshold)/*+100*/;
//            }else v = 0;
            (*dest)[i*(*resH)+j] = v;
            if(v > max) {
                max = v;
            }
        }
    }

    fprintf(stderr, "shmul max %lf", max);
}

std::complex<double> primeroot(int p) {
    static std::vector<std::complex<double>> r;
    if(r.size() > p) {
        return r[p];
    }
    int oldsize = r.size();
    r.resize(p+1);
    for(int i = oldsize; i <= p; i++) {
        r[i] = std::polar(1., tau/(1 << i));
    }
    return r[p];
}
//#pragma GCC push_options
static const double ftcoef = 1/sqrt(tau);
template<typename inptype>
void fftRec(inptype *data, int logsize, int logstep, std::complex<double> *res) {
    if(logsize == 0) {
        *res = *data*ftcoef;
        return;
    }
    fftRec(data, logsize-1, logstep+1, res);
    fftRec(data+(1 << logstep), logsize-1, logstep+1, res+(1 << (logsize-1)));

    std::complex<double> proot = primeroot(logsize);
    std::complex<double> root = 1;
    for(int i = 0; i < (1 << (logsize-1)); i++) {
        std::complex<double> e = res[i];
        std::complex<double> o = res[i+(1 << (logsize-1))];
        res[i] = e + root * o;
        res[i+(1 << (logsize-1))] = e - root * o;
        root *= proot;
    }
}

template<typename inptype>
void fft(inptype *data, int size, double *res)
{
    std::complex<double> actualRes[size];
    int logsize = intLog2(size);

    fftRec(data, logsize, 0, &actualRes[0]);
    for(int i = 0; i < size; i++) {
        res[i] = std::abs(actualRes[i]);
        if(max < (res[i])) {
            max = (res[i]);
//            maxh = i;
        }
    }
}

template void fft(int16_t *data, int size, double *res);
template void fft(double *data, int size, double *res);
template void fft(std::complex<double> *data, int size, double *res);
//#pragma GCC pop_options
template<typename inptype>
void complex_fft(inptype *data, int size, std::complex<double> *res)
{
    int logsize = intLog2(size);

    fftRec(data, logsize, 0, &res[0]);
    for(int i = 0; i < size; i++) {
        if(max < std::abs(res[i])) {
            max = std::abs(res[i]);
//            maxh = i;
        }
    }
}
template void complex_fft(int16_t *data, int size, std::complex<double> *res);
template void complex_fft(double *data, int size, std::complex<double> *res);


void isolateMaxima(int w, double *transform, int h,  double *data)
{
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
}

