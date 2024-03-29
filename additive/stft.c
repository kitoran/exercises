﻿#include "stft.h"
#include <math.h>
#include <assert.h>
#include "mathext.h"
#include "globals.h"
#include <stdio.h>
#include <complex.h>

const double hammingCoef = 0.53836;

double* windowStbArray = NULL;

void makeHammingWindow(int windowSize)
{
    arrsetlen(windowStbArray, windowSize);
    double anotherCoef = 1 - hammingCoef;
    for(int i = 0; i < windowSize; i++) {
//        int n = i-windowSize/2;
        windowStbArray[i] = hammingCoef - anotherCoef*cos(tau*i/windowSize);
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
    double complex primroots[*resH];
    for(int i = 0; i < *resH; i++) {
        double mul = pow(frequencyMultiplent, i);
        double prod = freqMin * mul;
        double argument = tau*prod/samplerate;
        primroots[i] = CMPLX(cos(argument), sin(argument));
    }
    *resW = ((size) - windowSize)/step;
    *res = (double*)malloc((*resW)*(*resH)*sizeof(double));
    fprintf(stderr, "\n%ld bytes for doubles\n", (*resW)*(*resH)*sizeof(double));

//    return;
//    exit(0);

    double maxw;
    double maxh;
    double * datumStbArray = NULL; arrsetlen(datumStbArray, windowSize);
    for(int i = 0; i < *resW; i++) {
        for(int n = 0; n < windowSize; n++) {
            datumStbArray[n] = windowStbArray[n] * data[i*step+n];
        }
        for(int ind = 0; ind < *resH; ind++) {
            complex double acc = 0;
            const complex double primroot = primroots[ind];
            complex double root = 1;
            for(int j = 0; j < windowSize; j++) {
                acc += root * ((double)(datumStbArray[j])/1000.);
                root *= primroot;
            }
            (*res)[i*(*resH) + ind] = cabs(acc);
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
void fftDouble(double *data, int size, double *res);

void stfft(int16_t *data, int size, int windowSize, int step, double **res, int *resW)
{

    max = 0;
    makeHammingWindow(windowSize);
    if(size == 0) {
        *resW = 0;
        *res = NULL;
        return;
    }
    *resW = ((size) - windowSize)/step;
    *res = (double*)malloc((*resW)*(windowSize)*sizeof(double));

//    return;

    double* datumStbArray = NULL; arrsetlen(datumStbArray, windowSize);
    for(int i = 0; i < *resW; i++) {
        for(int n = 0; n < windowSize; n++) {
            datumStbArray[n] = windowStbArray[n] * data[i*step+n];
        }
        fftDouble(/*data + step*i*/datumStbArray, windowSize, (*res) + i*windowSize);
        if((i) %64 == 0) {
            fprintf(stderr, "%lf, %d of %d", max, i, *resW);
        }
    }
    arrfree(datumStbArray);
    fprintf(stderr, "max %lf ", max);
}

void complex_fftInt16(int16_t *data, int size, complex double *res);
void complex_stfft(int16_t *data, int size, int windowSize, int step, complex double **res, int *resW)
{
    max = 0;
    makeHammingWindow(windowSize);
    *resW = ((size) - windowSize)/step;
    *res = (complex double*)malloc((*resW)*(windowSize)*sizeof(complex double));
    double* datumStbArray = NULL; arrsetlen(datumStbArray, windowSize);
    for(int i = 0; i < *resW; i++) {
        for(int n = 0; n < windowSize; n++) {
            datumStbArray[n] = windowStbArray[n] * data[i*step+n];
        }
        complex_fftInt16(data + step*i, windowSize, (*res) + i*windowSize);
        if((i) %4 == 0) {
            fprintf(stderr, "%lf, %d of %d", max, i, *resW);
        }
    }
    fprintf(stderr, "max %lf ", max);
}

struct harmonic *maxesStbArray(double *data, int h, int w)
{
    struct harmonic* rStbArray = NULL;
    arrsetcap(rStbArray, w);
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
        struct harmonic hc = {freq, max};
        arrput(rStbArray, hc);
    }
    return rStbArray;
}

struct harmonic** maxesLinearStbArray(double *data, int h, int w, int samplerate)
{
    struct harmonic** rStbArray = NULL;
    arrsetcap(rStbArray, w);
    for(int i = 0; i < w; i++) {
        struct harmonic* eStbArray = NULL;
        for(int j = 1; j < h-1; j++) {
            double freq = (double)(samplerate)/originalFourierTransformH*j;
            if(data[i*h+j] > data[i*h+j-1]
                 && data[i*h+j] > data[i*h+j+1]
                 && freq < 20000
                 && data[i*h+j] > max/200
                    ) {
        // здесь не надо будеит находить все коэффициенты
        // a0 =  freq1 freq2 * amp0/2   - freq0 freq2 * amp1   + freq0 freq1 * amp2/2
        // a1 = -(freq1+freq2) * amp0/2 + (freq0+freq2) * amp1 - (freq0 + freq1) * amp2/2
        // a2 = amp0/2                  - amp1                 + amp2/2
                double amp0 = data[i*h+(j-1)], amp1 = data[i*h+(j)], amp2 = data[i*h+(j+1)];
                double freq0 = (j-1), freq1 = j, freq2 = (j+1);
                double a0 =  freq1 * freq2 * amp0/2   - freq0 * freq2 * amp1   + freq0 * freq1 * amp2/2;
                double a1 = -(freq1+freq2) * amp0/2 + (freq0+freq2) * amp1 - (freq0 + freq1) * amp2/2;
                double a2 = amp0/2                  - amp1                 + amp2/2;
        fprintf(stderr, "\n %lf %lf %lf \n",
                fabs(a0 + a1*freq0 + a2*freq0*freq0 - amp0),
                fabs(a0 + a1*freq1 + a2*freq1*freq1 - amp1),
                fabs(a0 + a1*freq2 + a2*freq2*freq2 - amp2));
        assert(fabs(a0 + a1*freq0 + a2*freq0*freq0 - amp0) < 0.01);
        assert(fabs(a0 + a1*freq1 + a2*freq1*freq1 - amp1) < 0.01);
        assert(fabs(a0 + a1*freq2 + a2*freq2*freq2 - amp2) < 0.01);

// amp = ampmax - thinness * (freq - max)^2 = am? - th? fr^2 + th? fr max? - th? max?^2 = amp
// amp1 - amp2 = th fr2^2 - th fr1^2 + th fr1 max - th fr2 max
// th = (amp1-amp2)/(fr2^2 - fr1^2)
//
                double freqmax = (double)(samplerate)/originalFourierTransformH *
                                    (-a1/2/a2);
                double ampmax = a0 - (a1*a1/a2/4);

//                struct harmonic hc = {freq, data[i*h+j]};
                struct harmonic hc = {freqmax, ampmax};
                arrput(eStbArray, hc);
            }
        }
//        int ampMore(const void * a, const void * b) {
//            return ((struct harmonic*)a)->amp > ((struct harmonic*)b)->amp;
//        }
//        qsort(e, arrlen(e), sizeof(*e), ampMore);
        int freqLess(const void * a, const void * b) {
            return ((struct harmonic*)a)->freq < ((struct harmonic*)b)->freq;
        }
//        if(e.size() > 15) e.resize(1);
        qsort(eStbArray, arrlen(eStbArray), sizeof(*eStbArray), freqLess);

        arrput(rStbArray, eStbArray);
    }
    return rStbArray;
}

void shiftandmul(double *src, int h, int w, double **dest, int* resH)
{
    max = 0;
    int lastHarmonic = 14;
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
//    constexpr int lastHarmonic =2;

    *resH = h/numberOfHarmonics;
    *dest = (double*)malloc(w*(*resH)*sizeof(double));
    for(int i = 0; i < w; i++) {
        for(int j = 0; j < *resH; j++) {
            double v = 1;
            for(int k = 0; k < numberOfHarmonics; k++) {
                if(j*(k+1) < h) {
                    v *= src[i*h + j*(k+1)]/500000;
                } else {
                    abort();
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

complex double primeroot(int p) {
    static complex double* r = NULL;
    if(arrlen(r) > p) {
        return r[p];
    }
    int oldsize = arrlen(r);
    arrsetlen(r, p+1);
    for(int i = oldsize; i <= p; i++) {
        r[i] = CMPLX(cos(tau/(1 << i)), sin(tau/(1 << i)));
    }
    return r[p];
}
//#pragma GCC push_options
static const double ftcoef = 0.3989422804;//it's1/sqrt(tau);
#include "fftrec.c"
#include "fft.c"
//#pragma GCC pop_options

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

