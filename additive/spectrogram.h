#ifndef SPECTRUM_H
#define SPECTRUM_H
#include <inttypes.h>
#include <stft.h>
#include <vector>
//enum class spectrogram_mode {linear, logarithmic};
class QPainter;
class Spectrogram
{
public:
    virtual int width() = 0;
    double max;
    virtual void draw(QPainter*, int, int) = 0;
    virtual void fillBuffer(int16_t*, int bufferSize, int pos, unsigned int phase) = 0;
    void drawAxes(QPainter*, int, int);
    virtual double frequencyAtProportion(double p) = 0;

};
class MaximaSpectrogram : public Spectrogram
{ public:
    std::vector<std::vector<harmonic>> maxima;
    int width() override {
        return maxima.size();
    }
    static int* lookup;
    void draw(QPainter* p, int w, int h) override ;
    void fillBuffer(int16_t *, int bufferSize, int pos, unsigned int phase);
    double frequencyAtProportion(double proportion) override;
};

#endif // SPECTRUM_H
