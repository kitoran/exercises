#include "stft.h"
#include <math.h>
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

void stfft(int16_t *data, int size, int windowSize, int step, double **res, int *resW)
{
    max = 0;
    makeHammingWindow(windowSize);
    *resW = ((size) - windowSize)/step;
    *res = (double*)malloc((*resW)*(windowSize)*sizeof(double));
    double* datumStbArray = NULL; arrsetlen(datumStbArray, windowSize);
    for(int i = 0; i < *resW; i++) {
        for(int n = 0; n < windowSize; n++) {
            datumStbArray[n] = windowStbArray[n] * data[i*step+n];
        }
        fft(/*data + step*i*/datumStbArray, windowSize, (*res) + i*windowSize);
        if((i) %64 == 0) {
            fprintf(stderr, "%lf, %d of %d", max, i, *resW);
        }
    }
    fprintf(stderr, "max %lf ", max);
}

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
        complex_fft(data + step*i, windowSize, (*res) + i*windowSize);
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
            double freq = (double)(samplerate)/h*j;
            if(data[i*h+j] > data[i*h+j-1]
                 && data[i*h+j] > data[i*h+j+1]
                 && freq < 20000
                 && data[i*h+j] > max/200
                    ) {
                struct harmonic hc = {freq, data[i*h+j]};
                arrput(eStbArray, hc);
            }
        }
        int ampMore(const void * a, const void * b) {
            return ((struct harmonic*)a)->amp > ((struct harmonic*)b)->amp;
        }
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

complex double primeroot(int p) {
    static std::vector<complex double> r;
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
void fftRec(inptype *data, int logsize, int logstep, complex double *res) {
    if(logsize == 0) {
        *res = *data*ftcoef;
        return;
    }
    fftRec(data, logsize-1, logstep+1, res);
    fftRec(data+(1 << logstep), logsize-1, logstep+1, res+(1 << (logsize-1)));

    complex double proot = primeroot(logsize);
    complex double root = 1;
    for(int i = 0; i < (1 << (logsize-1)); i++) {
        complex double e = res[i];
        complex double o = res[i+(1 << (logsize-1))];
        res[i] = e + root * o;
        res[i+(1 << (logsize-1))] = e - root * o;
        root *= proot;
    }
}

void fft(int32_t *data, int size, double *res)
{
    complex double actualRes[size];
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
template void fft(complex double *data, int size, double *res);
//#pragma GCC pop_options
template<typename inptype>
void complex_fft(inptype *data, int size, complex double *res)
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
template void complex_fft(int16_t *data, int size, complex double *res);
template void complex_fft(double *data, int size, complex double *res);


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

