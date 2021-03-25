#include "graph.h"
#include "math.h"
#include "mathext.h"
#include "alsathread.h"

#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
extern const double freqMin=30;
extern const double frequencyMultiplent = 1.01;//sqrt(sqrt(freqMax/freqMin));

template <typename T>
QDebug &operator<<(QDebug& d, std::complex<T> t) {
    QDebugStateSaver r(d);
    d.nospace() << t.real() << " + i*" << t.imag();
    return d;
}


graph::graph(QWidget *parent) : QWidget(parent){



}

void graph::setLogarithmicData(double* data_, int height_, int width_, double max_, double freqMax_) {
    heights = height_;
    ddata = data_;
    widths = width_;
    max = max_;
    freqMax = freqMax_;
    mode = logarithmic;
}

void graph::setLinearData(double *data_, int width_, int height_, int windowSize_, int samplerate_, double max_) {
    heights = height_;
    ddata = data_;
    widths = width_;
    max = max_;
    samplerate = samplerate_;
    mode = linear;
    windowSize = windowSize_;
}

void graph::setComplexData(std::complex<double> *data_, int width_, int height_, int windowSize_, int samplerate_, double max_) {
    heights = height_;
    cdata = data_;
    widths = width_;
    max = max_;
    samplerate = samplerate_;
    mode = linear;
    complex = true;
    windowSize = windowSize_;
}

void graph::paintEvent(QPaintEvent */*event*/) {
    fprintf(stderr, "real %d %d need %d %d", width()-20, height()-20, widths, heights);

    QPainter p(this);
    p.drawLine(QPointF{10., height()-10.},
               QPointF{width() - 10., height()-10.});
//    p.drawLine(QPointF{10, height()-10},
//               QPointF{10, 10});

//    QPointF last(10, height()-10);
    p.setPen(Qt::NoPen);
    double cutoff = 10000;//samplerate;//10000.0;//*heights/samplerate;
    double cutoffind = cutoff*heights/samplerate;
    for(int i = 0; i < widths; i++) {
        for(int j = 0; j < cutoffind; j++) {
            double left = (width()-20)/double(widths)*i;
            double right = (width()-20)/double(widths)*(i+1);
            double bottom = (height()-20)/double(cutoffind)*j;
            double top = (height()-20)/double(cutoffind)*(j+1);
            int g = std::abs(cdata[i*heights+j])/max*255;
            p.setBrush(QColor(g,g,g));
//            qDebug() << "amplitude" << data[i*heights+j]
//                     << "max" << max <<
//                        "color" << g;
            p.drawRect(QRectF(left+10, height()-10-top, right-left, top-bottom));
        }
    }

    p.setPen(Qt::darkMagenta);
    double sortabase = log(freqMax/freqMin)/(height()-20);
    for(int y = 0; y < height()-20; y += 100) {
        double freq;
        if(mode == logarithmic) {
             freq = 30 * exp(sortabase*y);
        } else {
             freq = double(y) / (height()-20) * cutoff;
        }
        p.drawLine(QPointF{10., height()-10.-y},
                   QPointF{width()-10., height()-10.-y});
        p.drawText(QPointF{10., height()-10.-y},
                   QString::number(freq, ' ', 2));
    }

}

void graph::mouseMoveEvent(QMouseEvent *event)
{
//    double sortabase = log(20000.0/30)/(width()-20);
//    double freq = 30 * exp(sortabase*x);

//    int indh = double(height()-event->y()-10) / (height()-20) * heights;
    int indw = double(event->x()-10) / (width()-20) * widths;
//    double freq = 0;// = freqMin * log(freqMax/freqMin)/log(double(height()-event->y()-10) / (height()-20));//pow(frequencyMultiplent, indh);
    if(mode == linear) {
//        double(y) / (height()-20) * 1000;
//        freq = double(height()-event->y()-10) / (height()-20)*samplerate;
    }
    //    double val;
//    int ind = indw*heights+indh;
//    if(ind < 0 || ind >= heights*widths) {
//        val = nan("hello");
//    } else {
//        val = data[ind];
//    }


    mut.lock();
    m = {cdata+indw*heights, heights, mode, windowSize, true};
    full = true;
    mut.unlock();
    //    fprintf(stderr, "indh %d indw %d freq %lf val %lf", indh, indw, freq, val);
//    fprintf(stderr, "freq %lf", freq);
}

void graph::mousePressEvent(QMouseEvent *event)
{
    int indw = double(event->x()-10) / (width()-20) * widths;

    mut.lock();
    m = {cdata+indw*heights, heights, mode, windowSize, true};
    fprintf(stderr, "heights %d\n", heights);
    for(int i = 0; i < heights; i++) {
        if(cdata[indw*heights+i] != 0.) {
            qDebug() << i << cdata[indw*heights+i] <<
                    double(samplerate)/windowSize*i;

        }
    }
    full = true;
    mut.unlock();
}

void graph::mouseReleaseEvent(QMouseEvent */*event*/)
{
    mut.lock();
    m = {0, 0, mode};
    full = true;
    mut.unlock();
}

