#include "graph.h"
#include "globals.h"
#include "spectrogram.h"
#include "math.h"
#include "mathext.h"
#include "alsathread.h"

#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

template <typename T>
QDebug &operator<<(QDebug& d, std::complex<T> t) {
    QDebugStateSaver r(d);
    d.nospace() << t.real() << " + i*" << t.imag();
    return d;
}


graph::graph(QWidget *parent) : QWidget(parent){



}

//void graph::setData(Spectrogram* data_) {
//    heights = height_;
//    data = data_;
//    widths = width_;
//    max = max_;
//    freqMax = freqMax_;
//    mode = spectrogram_mode::logarithmic;
//}

//void graph::setLinearData(double *data_, int width_, int height_, int windowSize_, int samplerate_, double max_) {
//    heights = height_;
//    data = {data_, height_*width_};
//    widths = width_;
//    max = max_;
//    samplerate = samplerate_;
//    mode = spectrogram_mode::linear;
//    windowSize = windowSize_;
//}

//void graph::setComplexData(std::complex<double> *data_, int width_, int height_, int windowSize_, int samplerate_, double max_) {
//    heights = height_;
//    data = {(double*)data_, height_*width_*2};
//    widths = width_;
//    max = max_;
//    samplerate = samplerate_;
//    mode = spectrogram_mode::linear;
//    complex = true;
//    windowSize = windowSize_;
//}

void graph::paintEvent(QPaintEvent */*event*/) {
    fprintf(stderr, "real %d %d need %d", width()-20, height()-20, spectrogram->width());

    QPainter p(this);
    spectrogram->draw(&p, width(), height());
}

void graph::mouseMoveEvent(QMouseEvent *event)
{
//    double sortabase = log(20000.0/30)/(width()-20);
//    double freq = 30 * exp(sortabase*x);

//    int indh = double(height()-event->y()-10) / (height()-20) * heights;
    int indw = double(event->x()-10) / (width()-20) * spectrogram->width();
//    double freq = 0;// = freqMin * log(freqMax/freqMin)/log(double(height()-event->y()-10) / (height()-20));//pow(frequencyMultiplent, indh);
//    if(mode == spectrogram_mode::linear) {
//        double(y) / (height()-20) * 1000;
//        freq = double(height()-event->y()-10) / (height()-20)*samplerate;
//    }
    //    double val;
//    int ind = indw*heights+indh;
//    if(ind < 0 || ind >= heights*widths) {
//        val = nan("hello");
//    } else {
//        val = data[ind];
//    }
    if(indw < 0 || indw >= width()) {
        return;
    }
    message m;
    m = {indw};
//    qDebug() << "putting message:" << m.data.data << m.data.sized ;
    channel.blockAndPut(m);

    //    fprintf(stderr, "indh %d indw %d freq %lf val %lf", indh, indw, freq, val);
//    fprintf(stderr, "freq %lf", freq);
}

void graph::mousePressEvent(QMouseEvent *event)
{
    int indw = double(event->x()-10) / (width()-20) * spectrogram->width();
    message m{indw};
//    if(complex) {
//        m = message{data.slicec(indw*heights, heights), mode, true};
//    } else {
//        m = {data.sliced(indw*heights, heights), mode, true};
//    }
//    qDebug() << "putting message:" << m.data.data << m.data.sized ;
    channel.blockAndPut(m);


//    fprintf(stderr, "heights %d\n", heights);
//    for(int i = 0; i < heights; i++) {
//        if(data.c(indw*heights+i) != 0.) {
//            qDebug() << i << data.c(indw*heights+i) <<
//                    double(samplerate)/windowSize*i;

//        }
//    }
}

void graph::mouseReleaseEvent(QMouseEvent */*event*/)
{
    message m = {-1};
//    qDebug() << "putting message:" << m.data.data << m.data.sized;
    channel.blockAndPut(m);
}

