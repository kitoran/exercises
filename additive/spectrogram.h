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
    Spectrogram(double max_) : max(max_) {}
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
    MaximaSpectrogram(double max__, const std::vector<std::vector<harmonic>>& maxima_)
        : Spectrogram(max__), maxima(maxima_)
    { }
    void draw(QPainter* p, int w, int h) override ;
    void fillBuffer(int16_t *, int bufferSize, int pos, unsigned int phase);
    double frequencyAtProportion(double proportion) override;
};
class ContMaximaSpectrogram : public Spectrogram
{ public:
    std::vector<std::vector<continuousHarmonic>> maxima;
    int harmonics;
//    double phases[
    int width() override {
        return maxima.size();
    }
    ContMaximaSpectrogram(double max__, const std::vector<std::vector<continuousHarmonic>>& maxima_, int harmonics_)
        : Spectrogram(max__), maxima(maxima_), harmonics(harmonics_)
    { }
    void draw(QPainter* p, int w, int h) override ;
    void fillBuffer(int16_t *, int bufferSize, int pos, unsigned int phase);
    double frequencyAtProportion(double proportion) override;
};
class LinearSpectrogram : public Spectrogram
{ public:
    double* data;
    int width_;
    int height;
    double freqStep;
    int width() override {
        return width_;
    }
    LinearSpectrogram(double max_, double* data_, int width__,
        int height_, double freqStep_)
        : Spectrogram(max_), data(data_), width_(width__),
    height(height_), freqStep(freqStep_) {}
    void draw(QPainter* p, int w, int h) override ;
    void fillBuffer(int16_t *, int bufferSize, int pos, unsigned int phase);
    double frequencyAtProportion(double proportion) override;
};
#endif // SPECTRUM_H
