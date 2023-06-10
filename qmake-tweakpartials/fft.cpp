#include "fft.h"
//#include "mathext.h"
#include "extmath.h"
#include "stb_ds.h"
#include <complex>
#include <fstream>
#include <span>
#include <vector>
#include <assert.h>
#include <float.h>
using namespace std::literals::complex_literals;

std::complex<float> primeroot(int p) {
    static std::vector<std::complex<float>>  r;
    if(r.size() > p) {
        return r[p];
    }
    int oldsize = (int)r.size();
    r.resize(p+1);
    for(int i = oldsize; i <= p; i++) {
        r[i] = std::complex<float> (cos(tau/(1 << i)), sin(tau/(1 << i)));
    }
    return r[p];
}

//static void fftRec(std::complex<float>* data, int logsize, int logstep, std::span<std::complex<float>>res, bool inverse = false) {
////    static const float ftcoef = (float)0.3989422804; //it's 1/sqrt(tau);
//    if(logsize == 0) {
//        res[0] = data[0];//*ftcoef;
//        if(std::isnan(res[0].imag()) || std::isnan(res[0].real())) {
//                volatile int r=7;
//        }
//        return;
//    }
//    fftRec(data, logsize-1, logstep+1, res);
//    fftRec(data + (1ll << logstep), logsize-1, logstep+1, res.subspan(1ll << (logsize-1)));

//    std::complex<float>proot = primeroot(logsize);
////    if(inverse) proot = -proot;//std::conj(proot);
//    std::complex<float>root = 1;
//    for(int i = 0; i < (1 << (logsize-1)); i++) {
//        std::complex<float> e = res[i];
//        std::complex<float> o = res[i+(1 << (logsize-1))];
//        res[i] = e - (std::complex<float>(inverse?(-1):1))*root * o;
//        res[i+(1 << (logsize-1))] = e + (std::complex<float>(inverse?(-1):1))*root * o;
//        if(std::isnan(res[i].imag()) || std::isnan(res[i].real()) ||
//            std::isnan(res[i+(1 << (logsize-1))].imag()) ||
//                std::isnan(res[i+(1 << (logsize-1))].real())) {
//                volatile int r=7;
//        }
//        root *= proot;
//    }
//}
static void fftRec(std::complex<float>* data, int logsize, int logstep, const std::span<std::complex<float>>& res, bool inverse = false) {
    if(logsize == 1) {
        res[0] = data[0] + data[(1ll << (logstep))];
        res[1] = data[0] - data[(1ll << (logstep))];
        return;
    }
    fftRec(data, logsize-1, logstep+1, res, inverse);
    fftRec(data + (1ll << (logstep)), logsize-1, logstep+1, res.subspan(1ll << (logsize-1)), inverse);

//    std::complex<float> pr = primeroot(logsize);
//    std::complex<float> root = 1;
    float factor = (inverse?(1):(-1))*tau/(1 << (logsize));
//    if(inverse) pr = std::conj(pr);
    for(int i = 0; i < (1 << (logsize-1)); i++) {
        std::complex<float>root = std::polar<float>(1, factor*i);
        std::complex<float> e = res[i];
        std::complex<float> o = res[i+(1 << (logsize-1))];
        res[i] = e + root * o;
        res[i+(1 << (logsize-1))] = e - root * o;
//        root *= pr;
    }
}


void fft(std::span<float > data, int size, std::span<std::complex<float> >  res)
{
//    data.
    std::vector<std::complex<float>>  actualParam; actualParam.resize(size);
    for(int i = 0; i < size; i++) {
        actualParam[i]=data[i];//actualParam
    }
    std::vector<std::complex<float>>  actualRes; actualRes.resize(size);
    int logsize = intLog2(size);
    std::span<std::complex<float>> ar = actualRes;
    fftRec(actualParam.data(), logsize, 0, ar);
    for(int i = 0; i < size/2; i++) {
        res[i] = actualRes[i];
    }
}
static std::vector<float> windowSA;
std::complex<float> cast(const myComplex& r){
    return {r.r, r.i};
}
void ifft(myComplex *data, float* res)// data.size() = size/2+1, res.size() = size
{
    std::vector<std::complex<float>>  actualRes;    actualRes.resize(WINDOW_SIZE);
    std::vector<std::complex<float>>  actualParam;  actualParam.resize(WINDOW_SIZE);
    actualParam[0] = cast(data[0])*windowSA[0];
    for(int i = 1; i < WINDOW_SIZE/2+1; i++) {
        actualParam[WINDOW_SIZE-i] = std::conj(cast(data[i])) * windowSA[WINDOW_SIZE-i];
        actualParam[i] = cast(data[i])*windowSA[i];
    }
//    actualParam[1]=1;
    int logsize = intLog2(WINDOW_SIZE);
    std::span<std::complex<float>> ar = actualRes;
    std::span<std::complex<float>> ap = actualParam;
    fftRec(actualParam.data(), logsize, 0, ar, true);
    for(int i = 0; i < WINDOW_SIZE; i++) {
//        assert(fabs(actualRes[i].imag())<0.001);
        res[i] = actualRes[i].real()/WINDOW_SIZE;
    }
}

void makeHammingWindow(int windowSize)
{
    const float hammingCoef = (float)0.53836;
    windowSA.resize(windowSize);
    float anotherCoef = 1 - hammingCoef;
    for(int i = 0; i < windowSize; i++) {
        windowSA[i] = hammingCoef - anotherCoef*cos((float)tau*i/windowSize);
    }
}
void makeFlatWindows(int windowSize)
{
    windowSA.resize(windowSize);
    for(int i = 0; i < windowSize; i++) {
        windowSA[i] = 1;
    }
}

void makeHammingWindows(int windowSize)
{
    makeHammingWindow(windowSize);
//    makeHammingWindow<double>(windowSize);
}
void makeBHWindows(int windowSize)
{
    windowSA.resize( windowSize);

    double a0 = 0.35875, a1 = 0.48829, a2=0.14128, a3=0.01168;
    for(int i = 0; i < windowSize; i++) {
        windowSA[i] = a0 - a1*cos(tau*i/windowSize)+a2*cos(2*tau*i/windowSize)-a3*cos(3*tau*i/windowSize);
    }
}
void makeHannWindow(int windowSize)
{
    windowSA.resize( windowSize);

    for(int i = 0; i < windowSize; i++) {
        double s = sin(tau/2*i/windowSize);
        windowSA[i] = s*s;
    }
}

void windowedFft(float* data, int size, myComplex* res) // data.size()=size, res.size()=size/2
{
    static std::vector<float> preparedSA; preparedSA.resize(size);
    static std::vector<std::complex<float>> cres; cres.resize(size/2);
    for(int n = 0; n < size; n++) {
        preparedSA[n] = windowSA[n] *  data[n];
    }
    std::span<float> prep = preparedSA;
    std::span<std::complex<float>> rs(cres);
    fft(prep, size, rs);
    for(int i = 0; i < size/2; i++) {
        res[i] = {cres[i].real(), cres[i].imag()};
    }
}

//const DWORD MS_VC_EXCEPTION = 0x406D1388;
//#pragma pack(push,8)
//typedef struct tagTHREADNAME_INFO
//{
//    DWORD dwType; // Must be 0x1000.
//    LPCSTR szName; // Pointer to name (in user addr space).
//    DWORD dwThreadID; // Thread ID (-1=caller thread).
//    DWORD dwFlags; // Reserved for future use, must be zero.
// } THREADNAME_INFO;
//#pragma pack(pop)
//extern "C" void SetThreadName(DWORD dwThreadID, const char* threadName) {
//    THREADNAME_INFO info;
//    info.dwType = 0x1000;
//    info.szName = threadName;
//    info.dwThreadID = dwThreadID;
//    info.dwFlags = 0;
//#pragma warning(push)
//#pragma warning(disable: 6320 6322)
//    __try{
//        RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
//    }
//    __except (EXCEPTION_EXECUTE_HANDLER){
//    }
//#pragma warning(pop)
//}
//void windowedFft32(float *data, int size, float *res)
//{
//    /*unsigned int fp_control_state = */_controlfp((_EM_INEXACT | _EM_UNDERFLOW)  , _MCW_EM);
//    windowed_fft(std::span(data,size),size,std::span(res, size));
//}

//void ifft32(float *data, float *res)
//{
//    ifft(data, res);
//}
//float std::norm(std::complex<float> a) {
//    return std::norm(a);//a.r*a.r+a.i*a.i;
//}
//std::complex<float> z
//std::complex<float>& operator +=(std::complex<float>& a, std::complex<float> b) {

//}
void isolatePeaks(myComplex *datsa, Peak* peaks)
{
    std::complex<float> data[WINDOW_SIZE/2];
    memcpy(data, datsa, sizeof(data));
//    float prevprev = 2;
//    float prev = 1;
    int peaksF = 0;
#define NUM 1
//    for(int i = 0; i < (size-1)/NUM; i++) {
//        data[i] *= data[i*2] * data[i*3] * data[i*4]* data[i*5]* data[i*6]* data[i*7]* data[i*8];
//    }
//    FILE* ff = fopen("C:/src/exercises/build-qmake-tweakpartials-Desktop_Qt_6_4_1_MSVC2019_64bit-Debug/debug/hi.txt", "w");
    for(int i = 1; i < WINDOW_SIZE/2-1; i++) {
        if(std::norm(data[i]) > std::norm(data[i-1]) && std::norm(data[i]) > std::norm(data[i+1])) {
            peaks[peaksF].freq = SAMPLE_FREQ / WINDOW_SIZE * i;

            peaks[peaksF].val = {data[i].real(), data[i].imag()};//{ std::abs(data[i])*440/peaks[peaksF].freq*440/peaks[peaksF].freq,0};//
//            fprintf(ff, "%lf, %lf\n",SAMPLE_FREQ / WINDOW_SIZE * i, data[i]) ;
            peaksF++;
          //  if(peaks[peaksF].freq > 300) break;
        } else {data[i] = {0,0};}
    }
//    fclose(ff);
//    f << "\n\n\n";
//    f.flush();
//    volatile std::string sdfsf = f.str();
//    f.c
//    float data2[WINDOW_SIZE/2];
//    memset(data2, 0, sizeof(data2));
//    for(int i = 0; i < WINDOW_SIZE/2; i++) {
//        int index = round(peaks[i].freq / SAMPLE_FREQ * WINDOW_SIZE);
//        if(index >= 0 && index < WINDOW_SIZE/2) {
//            data2[index] += peaks[i].val;
//        }
//    }

    volatile int t = 56;
//    data[0]=data[size-1]=0;
}

void alignPeaks(Peak* peaks) {
    float step = 1.04427378243;
    float startFreq = 11.5623257126;

    for(int i = 0; i < WINDOW_SIZE/2; i++) {
        if(peaks[i].freq < 0.1 || peaks[i].freq >= 80000.0) break;
        float key = round(log(peaks[i].freq/startFreq)/log(step));
        peaks[i].freq = pow(step, key)*startFreq;
    }
}
float norm(const myComplex& r){
    return r.r*r.r + r.i*r.i;
}
//void resynth(Peak* peaks, float* res, int phase) {
//    myComplex data[WINDOW_SIZE/2+1];
//    memset(data, 0, sizeof(data));
////    data[0] = 1;
//    for(int i = 0; i < WINDOW_SIZE/2; i++) {
////        if(peaks[i].freq > 2000) break;
//        int index = round(peaks[i].freq / SAMPLE_FREQ * WINDOW_SIZE);
//        if(index >= 0 && index < WINDOW_SIZE/2 + 1 && norm(data[index]) < norm(peaks[i].val)) {
////            std::complex<float> mul = std::complex<float>(std::pow<float>(primeroot(intLog2(WINDOW_SIZE/STEP_SIZE)), index*phase));

//            float mul = ((index)*phase)%2?(1):(-1);
//            float abs = std::abs(cast(peaks[i].val));
//            data[index] = {abs*mul, 0};

////            data[index] = cast(peaks[i].val);
//        }
//    }
//    ifft(data, res);
//}

void resynth(Peak* peaks, float* res, int phase) {
    Peak* p = peaks;
    for(int i = 0; i < WINDOW_SIZE; i++) {
        res[i]=0;
        p = peaks;
//        while(p->freq > 0) {
//            res[i] += sin[
//        }
    }
}

float *window()
{
    return windowSA.data();
}

