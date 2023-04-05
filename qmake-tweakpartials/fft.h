#ifndef FFT_H
#define FFT_H
//#include <complex>
//#include <span>
#ifdef __cplusplus
extern "C" {
#endif
#define SAMPLE_FREQ 44100.0//48000.0//44100.0

#define WINDOW_SIZE (4096*2/**2*/)
#define STEP_SIZE (2048*2)//(1024*4)
typedef struct myComplex {
    float r;
    float i;
} myComplex;
void windowedFft(float* data, int size, myComplex* res); // data.size()=size, res.size()=size/2
//void ifft(std::complex<float> *data, float* res);
// arrlen(data)=size, arrlen(res)=size/2
//void ifft32(std::complex<float> * data,  float* res);// arrlen(data) = size/2, arrlen(res) = size
void makeHammingWindows(int size);
void makeBHWindows(int windowSize);
void makeFlatWindows(int windowSize);
void makeHannWindow(int windowSize);
float* window();
typedef struct Peak {
    float freq;
    myComplex val;
} Peak;
void alignPeaks(Peak* peaks);
void isolatePeaks(myComplex* data, Peak *peaks);
void resynth(Peak* peaks, float* res, int phase);
#ifdef __cplusplus
}
#endif
#endif // FFT_H
