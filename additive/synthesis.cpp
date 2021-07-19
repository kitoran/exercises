#include "synthesis.h"
#include <vector>
#include <string>
#include <memory.h>
#include <assert.h>
#include <math.h>
#include <mathext.h>
#include <soundext.h>
#include <spectrogram.h>

#include "stft.h"

#include "globals.h"
#include "newfile.h"



void resynthesizeOne(double *data, int w, SF_INFO inpi, int stepSize, int h)
{
    std::vector<harmonic> maxe = maxes(data, h, w);

    SF_INFO outi = inpi;
    SNDFILE* out = sf_open("/home/n/exercises/additive/resynth.wav", SFM_WRITE, &outi);
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

void resynthesizeMaxima(ContMaximaSpectrogram* s, int start, int end)
{
//    fprintf(stderr, "Data %p w %d stepSize %d specs %d", data, w, stepSize, specrtSize);
//    double* look = ;

//    SF_INFO outi = inpi;
//    SNDFILE* out = play?0:sf_open(
//                newFile("/home/n/exercises/additive/resynthesizeMaxima", "wav").c_str(),
//                SFM_WRITE,
//                &outi);
    const std::vector<std::vector<continuousHarmonic> >& prepared= s->maxima;
    int hms = s->harmonics;
    if(start == -1) start = 0;
    if(end == -1) end = prepared.size();
    audioOutput.resize((end - start)*stepSize);
//    audioOutput = (double*)malloc(audioOutputSize*sizeof(double));
    double phases[hms] = {0};



    for(int i = start+1; i < end; i++) {
        int16_t outb[stepSize];

        for(int s = 0; s < stepSize; s++) {
            double v = 0;
            for(int j = 0; j < prepared[i].size(); j++) {
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
                amp = prepared[i][j].h.amp/max;// * s / stepSize;
//                if(fabs(amp) > max) {
//                    abort();
//                }
                freq = prepared[i][j].h.freq;
                int deb = prepared[i][j].continuity;
                v += sin(phases[prepared[i][j].continuity])*amp;
                double integralDummy;
                phases[prepared[i][j].continuity] +=freq/inpi.samplerate*tau;
//                phases[prepared[i][j].continuity] = modf(prepared[i][j].continuity/tau, &integralDummy)*tau;
//                v += amp*sinLookupTable[int64_t((freq
//                                                *(i*stepSize+s)/inpi.samplerate+i)*LOOKUP_TABLE_SIZE)
//                                %LOOKUP_TABLE_SIZE]
                        /*/denominator*/ ;
            }
            outb[s] = v*800;
        }

//        s->fillBuffer(outb, stepSize, i, (int64_t)i*stepSize);

        memcpy(audioOutput.data()+(i-start)*stepSize, outb, stepSize
               *sizeof(outb[0]));
        if((i) %4 == 0) {
            fprintf(stderr, "hey %d of %lud", i, prepared.size());
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

class IntPool {
    bool* pool;
    int size;
    int index;
    int m_occupation;
public:
    IntPool(int size_) :
        pool((bool*)malloc(sizeof(bool)*size_)),
             size(size_),
             index(0),
            m_occupation(0)
    {
         for(int i = 0; i < size; i++) {
             pool[i] = false;
         }
    }
    ~IntPool() {
        free(pool);
    }
    int getNewContinuity() {
        int oldInd = index;
        while(pool[index] && index < size) {
            index++;
        }
        if(index == size) {
            index = 0;
            while(pool[index] && index < oldInd) {
                index++;
            }
            if(index == oldInd) {
                abort();
            }
        }
        assert(!pool[index]);
        pool[index] = true;
        if(index == 5) {
            fprintf(stderr, "get  %d",  index);
        }
        m_occupation++;
        return index;
    }

    void releaseContinuity(int ind) {
        assert(pool[ind]);
        pool[ind] = false;
        if(ind == 5) {
            fprintf(stderr, "free %d",  ind);
        }
        m_occupation--;
    }

    int occupation() {
        return m_occupation;
    }
};

std::vector<std::vector<continuousHarmonic> > prepareHarmonics(const std::vector<std::vector<harmonic> > &data, int *continuities) {
    *continuities = 0;

    for(int i = 0; i < data.size(); i++) {
        if(*continuities < data[i].size()*2) {
            *continuities = data[i].size()*2;
        }
    }
    IntPool pool(*continuities);
    std::vector<std::vector<continuousHarmonic> > res;
    res.reserve(data.size());
    {
        std::vector<continuousHarmonic> p;
        for(int i = 0; i < data[0].size(); i++) {
            int cont = pool.getNewContinuity();
            p.push_back({data[0][i], -1, cont, false});
        }
        res.push_back(p);
    }
    for(int i = 1; i < data.size(); i++) {
        if(i==35) {
            fprintf(stderr, "i is %d", i);
        }
        int d1 = pool.occupation();
        int d2 = data[i-1].size();

        assert(d1 == d2);
        std::vector<continuousHarmonic> p;
        int rightInd = 0;
        int leftInd = 0;
        while(rightInd < data[i].size() && leftInd < data[i-1].size()) {
            if(leftInd < data[i-1].size()) {
                leftInd++;
            }
            while(leftInd < data[i-1].size()
                  && data[i-1][leftInd].freq < data[i][rightInd].freq) {
                int finishedCont = res[i-1][leftInd-1].continuity;
                pool.releaseContinuity( finishedCont );
                leftInd++;
            }
            if(leftInd == data[i-1].size()) {
                if(intervalInSemitones(data[i-1][leftInd-1].freq, data[i][rightInd].freq) < 1) {
                    res[i-1][leftInd-1].hasRightCounterpart = true;
                    int cont = res[i-1][leftInd-1].continuity;
                    p.push_back({data[i][rightInd], leftInd-1, cont, false});
                    rightInd++;
                    break;
                }
                leftInd--;
                int cont = pool.getNewContinuity();
                p.push_back({data[i][rightInd], -1, cont, false});
                rightInd++;
            } else {
                double intLower = intervalInSemitones(data[i-1][leftInd-1].freq, data[i][rightInd].freq);
                double intHigher = intervalInSemitones(data[i-1][leftInd].freq, data[i][rightInd].freq);
                bool lowerCloser = intLower < intHigher;
                double smallest = lowerCloser?intLower:intHigher;
                if(smallest < 0.5) {
                    res[i-1][leftInd-lowerCloser].hasRightCounterpart = true;
                    int cont = res[i-1][leftInd-lowerCloser].continuity;
                    p.push_back({data[i][rightInd], leftInd-lowerCloser
                                 /*вычитаем 1 если нижний интервал меньше*/,
                                 cont,
                                 false});
                    rightInd++;
                    if(!lowerCloser) {
                        int finishedCont = res[i-1][leftInd-1].continuity;
                        pool.releaseContinuity( finishedCont);
                        leftInd++;
                    }





//                    else {
//                        pool.releaseContinuity(leftInd);
//                    }





                } else {
                    leftInd--;
                    int cont = pool.getNewContinuity();
                    p.push_back({data[i][rightInd], -1, cont, false});
                    rightInd++;


//                    int finishedCont = res[i-1][leftInd-1].continuity;
//                    pool.releaseContinuity(finishedCont);


                }
            }
        }
        while(rightInd < data[i].size()) {
            int cont = pool.getNewContinuity();
            p.push_back({data[i][rightInd], -1, cont, false});
            rightInd++;
        }
        while(leftInd < data[i-1].size()) {
            int finishedCont = res[i-1][leftInd].continuity;
            pool.releaseContinuity(finishedCont);
            leftInd++;
        }
        res.push_back(p);
    }
    return res;
}

