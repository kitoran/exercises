#include "synthesis.h"
#include "stb_ds.h"
#include <memory.h>
#include <assert.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <mathext.h>
#include <soundext.h>
#include <spectrogram.h>

#include "stft.h"

#include "globals.h"
#include "intpool.h"
#include "newfile.h"



void resynthesizeOne(double *data, int w, SF_INFO inpi, int stepSize, int h)
{
    struct harmonic* maxeStbArray = maxesStbArray(data, h, w);

    SF_INFO outi = inpi;
    SNDFILE* out = sf_open("/home/n/exercises/additive/resynth.wav", SFM_WRITE, &outi);
    double phase = 0;
    for(int i = 0; i < w-1; i++) {
        double maxfreqp = maxeStbArray[i].freq;
        double maxfreqn = maxeStbArray[i+1].freq;
        double maxampp = maxeStbArray[i].amp;
        double maxampn = maxeStbArray[i+1].amp;
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

void resynthesizeAll(double *data, int w, SF_INFO inpi, int stepSize, int h)
{
    SF_INFO outi = inpi;
    SNDFILE* out = sf_open("/home/n/exercises/additive/resynthAllFlute.wav", SFM_WRITE, &outi);

    //    double phases[h] = {0};
    double frequencies[h];// = {0};
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
                v += sin(tau*frequencies[j]/inpi.samplerate*(i*stepSize+j))*amp;
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

void resynthesizeAllLinear(double *data, int w, int stepSize, SF_INFO inpi, int specrtSize, int windowSize)
{
    fprintf(stderr, "Data %p w %d stepSize %d specs %d", data, w, stepSize, specrtSize);
//    double* look = ;
    int logWindowSize = intLog2(windowSize);
    SF_INFO outi = inpi;
    SNDFILE* out = sf_open("/home/n/exercises/additive/resynthAllLinear.wav", SFM_WRITE, &outi);
    for(int i = 0; i < w-1; i++) {
        double outb[stepSize];
        for(int s = 0; s < stepSize; s++) {
            double v = 0;
            for(int j = 0; j < specrtSize; j++) {
                double amp = (data[i*specrtSize+j] * (stepSize-s) + data[(i+1)*specrtSize+j] * (s))/stepSize/max;
                v += sinLookupTable[
                        (((long long int)(i*stepSize+s)*LOOKUP_TABLE_SIZE*j+hash(j))>>logWindowSize)%LOOKUP_TABLE_SIZE]*amp/10;
            }
            outb[s] = v;
        }
        sf_write_double(out, outb, stepSize);
        if((i) %4 == 0) {
            fprintf(stderr, "hey %d of %d", i, w);
        }

    }
    sf_close(out);
}

void resynthesizeMaxima(struct ContMaximaSpectrogram* s, int start, int end)
{
//    fprintf(stderr, "Data %p w %d stepSize %d specs %d", data, w, stepSize, specrtSize);
//    double* look = ;

//    SF_INFO outi = inpi;
//    SNDFILE* out = play?0:sf_open(
//                newFile("/home/n/exercises/additive/resynthesizeMaxima", "wav").c_str(),
//                SFM_WRITE,
//                &outi);
    const struct continuousHarmonic** preparedStbArray = s->maxima;
    int hms = s->harmonics;
    if(start == -1) start = 0;
    if(end == -1) end = arrlen(preparedStbArray);
    arrsetlen(audioOutputStb, (end - start)*stepSize);
//    audioOutput = (double*)malloc(audioOutputSize*sizeof(double));
    double phases[hms];// = {0};
    for(int i = 0; i < hms; i++) phases[i] = 0;


    for(int i = start+1; i < end; i++) {
        int16_t outb[stepSize];

        for(int s = 0; s < stepSize; s++) {
            double v = 0;
            for(int j = 0; j < arrlen(preparedStbArray[i]); j++) {
                double amp;
                double freq;
//                if(prepared[i][j].prev >= 0) {
//                    int deb = prepared[i][j].prev;
//                    int d2 = prepared[i-1].size();
//                    harmonic prev = prepared[i-1][prepared[i][j].prev].h;
//                    freq = (prev.freq * (stepSize-s) + prepared[i][j].h.freq * (s))/stepSize;
//                    amp = (prev.amp * (stepSize-s) + prepared[i][j].h.amp * (s))/stepSize/max;
//                } else {
//                }
                amp = preparedStbArray[i][j].h.amp/max;// * s / stepSize;
//                if(fabs(amp) > max) {
//                    abort();
//                }
                freq = preparedStbArray[i][j].h.freq;
                int deb = preparedStbArray[i][j].continuity;
                v += sin(phases[preparedStbArray[i][j].continuity])*amp;
                double integralDummy;
                phases[preparedStbArray[i][j].continuity] +=freq/inpi.samplerate*tau;
//                phases[prepared[i][j].continuity] = modf(prepared[i][j].continuity/tau, &integralDummy)*tau;
//                v += amp*sinLookupTable[int64_t((freq
//                                                *(i*stepSize+s)/inpi.samplerate+i)*LOOKUP_TABLE_SIZE)
//                                %LOOKUP_TABLE_SIZE]
                        /*/denominator*/ ;
            }
            outb[s] = v*800;
        }

//        s->fillBuffer(outb, stepSize, i, (int64_t)i*stepSize);

        memcpy(audioOutputStb+(i-start)*stepSize, outb, stepSize
               *sizeof(outb[0]));
        if((i) %4 == 0) {
            fprintf(stderr, "hey %d of %lud", i, arrlen(preparedStbArray));
        }

    }
//    if(!play) {
//        sf_close(out);
//    }
}


double intervalInSemitones(double frq1, double freq2) {
    static const double log12RootOf2 = log(pow(2, 1.0/12));

    return fabs(log(frq1/freq2)/log12RootOf2);
}

continuousHarmonic** prepareHarmonicsStbVector(const harmonic** dataStbArray, int *continuities) {
    *continuities = 0;

    for(int i = 0; i < arrlen(dataStbArray); i++) {
        if(*continuities < arrlen(dataStbArray[i])*2) {
            *continuities = arrlen(dataStbArray[i])*2;
        }
    }
    struct IntPool pool;
    initIntPool(&pool, *continuities);
    continuousHarmonic** res = 0;
    arrsetcap(res, arrlen(dataStbArray));
    {
        continuousHarmonic* pStb = 0;
        for(int i = 0; i < arrlen(dataStbArray[0]); i++) {
            int cont = getNewContinuity(&pool);
            continuousHarmonic fawefawef = {dataStbArray[0][i], -1, cont, false};
            arrpush(pStb, fawefawef);
        }
        arrpush(res, pStb);
    }
    for(int i = 1; i < arrlen(dataStbArray); i++) {
        if(i==35) {
            fprintf(stderr, "i is %d", i);
        }
        int d1 = occupation(&pool);
        int d2 = arrlen(dataStbArray[i-1]);

        assert(d1 == d2);
        continuousHarmonic* p = 0;
        int rightInd = 0;
        int leftInd = 0;
        while(rightInd < arrlen(dataStbArray[i]) && leftInd < arrlen(dataStbArray[i-1])) {
            if(leftInd < arrlen(dataStbArray[i-1])) {
                leftInd++;
            }
            while(leftInd < arrlen(dataStbArray[i-1])
                  && dataStbArray[i-1][leftInd].freq < dataStbArray[i][rightInd].freq) {
                int finishedCont = res[i-1][leftInd-1].continuity;
                releaseContinuity( &pool, finishedCont );
                leftInd++;
            }
            if(leftInd == arrlen(dataStbArray[i-1])) {
                if(intervalInSemitones(dataStbArray[i-1][leftInd-1].freq, dataStbArray[i][rightInd].freq) < 1) {
                    res[i-1][leftInd-1].hasRightCounterpart = true;
                    int cont = res[i-1][leftInd-1].continuity;
                    continuousHarmonic vadv = {dataStbArray[i][rightInd], leftInd-1, cont, false};
                    arrpush(p, vadv);
                    rightInd++;
                    break;
                }
                leftInd--;
                int cont = getNewContinuity(&pool);
                continuousHarmonic fa = {dataStbArray[i][rightInd], -1, cont, false};
                arrpush(p, (fa));
                rightInd++;
            } else {
                double intLower = intervalInSemitones(dataStbArray[i-1][leftInd-1].freq, dataStbArray[i][rightInd].freq);
                double intHigher = intervalInSemitones(dataStbArray[i-1][leftInd].freq, dataStbArray[i][rightInd].freq);
                bool lowerCloser = intLower < intHigher;
                double smallest = lowerCloser?intLower:intHigher;
                if(smallest < 0.5) {
                    res[i-1][leftInd-lowerCloser].hasRightCounterpart = true;
                    int cont = res[i-1][leftInd-lowerCloser].continuity;
                    continuousHarmonic ch = {dataStbArray[i][rightInd], leftInd-lowerCloser
                                             /*вычитаем 1 если нижний интервал меньше*/,
                                             cont,
                                             false};
                    arrpush(p, (ch));
                    rightInd++;
                    if(!lowerCloser) {
                        int finishedCont = res[i-1][leftInd-1].continuity;
                        releaseContinuity(&pool, finishedCont);
                        leftInd++;
                    }
//                    else {
//                        pool.releaseContinuity(leftInd);
//                    }
                } else {
                    leftInd--;
                    int cont = getNewContinuity(&pool);
                    continuousHarmonic rge = {dataStbArray[i][rightInd], -1, cont, false};
                    arrpush(p, rge);
                    rightInd++;


//                    int finishedCont = res[i-1][leftInd-1].continuity;
//                    pool.releaseContinuity(finishedCont);


                }
            }
        }
        while(rightInd < arrlen(dataStbArray[i])) {
            int cont = getNewContinuity(&pool);
            continuousHarmonic f = {dataStbArray[i][rightInd], -1, cont, false};
            arrpush(p, (f));
            rightInd++;
        }
        while(leftInd < arrlen(dataStbArray[i-1])) {
            int finishedCont = res[i-1][leftInd].continuity;
            releaseContinuity(&pool, finishedCont);
            leftInd++;
        }
        arrpush(res, p);
    }
    return res;
}

