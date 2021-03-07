#include "graph.h"
#include "math.h"

#include <QPainter>
#include <QMouseEvent>
extern const int windowSize = 2048;
        extern const int stepSize = 1024;
extern const double freqMax=20000;
extern const double freqMin=30;
extern const double frequencyMultiplent = 1.02;//sqrt(sqrt(freqMax/freqMin));


graph::graph(QWidget *parent) : QWidget(parent){



}

void graph::setData(double* data_, int height_, int width_, double max_) {
    heights = height_;
    data = data_;
    widths = width_;
    max = max_;

}

void graph::paintEvent(QPaintEvent *event) {
    fprintf(stderr, "real %d %d need %d %d", width()-20, height()-20, widths, heights);

    QPainter p(this);
    p.drawLine(QPointF{10, height()-10},
               QPointF{width() - 10, height()-10});
//    p.drawLine(QPointF{10, height()-10},
//               QPointF{10, 10});

//    QPointF last(10, height()-10);
    p.setPen(Qt::NoPen);
    for(int i = 0; i < widths; i++) {
        for(int j = 0; j < heights; j++) {
//            double freq = freqMin * pow(frequencyMultiplent, i);
//            double x = log(freq/30)/sortabase;
//            double y = data[i]/max*(height()-20);
//            QPointF ne{x+10, height()-10-y};
//            p.drawLine(last, ne);
//            last = ne;
//        p.drawText(QPointF{x+10, height()-10},
//                   QString::number(freq, ' ', 2));
            double left = (width()-20)/double(widths)*i;
            double right = (width()-20)/double(widths)*(i+1);
            double bottom = (height()-20)/double(heights)*j;
            double top = (height()-20)/double(heights)*(j+1);
            int g = data[i*heights+j]/max*255;
            p.setBrush(QColor(g,g,g));
            p.drawRect(QRectF(left+10, height()-10-top, right-left, top-bottom));
        }
    }

    p.setPen(Qt::darkMagenta);
    double sortabase = log(freqMax/freqMin)/(height()-20);
    for(int y = 0; y < height()-20; y += 100) {
        double freq = 30 * exp(sortabase*y);
        p.drawLine(QPointF{10, height()-10-y},
                   QPointF{width()-10, height()-10-y});
        p.drawText(QPointF{10, height()-10-y},
                   QString::number(freq, ' ', 2));
    }
//    QPointF last(10, height()-10);
//    for(int i = 0; i < heights; i++) {
//        double freq = 30 * pow(frequencyMultiplent, i);
//        double x = log(freq/30)/sortabase;
//        double y = data[i]/max*(height()-20);
//        QPointF ne{x+10, height()-10-y};
//        p.drawLine(last, ne);
//        last = ne;
////        p.drawText(QPointF{x+10, height()-10},
////                   QString::number(freq, ' ', 2));
//    }
}

void graph::mouseMoveEvent(QMouseEvent *event)
{
//    double sortabase = log(20000.0/30)/(width()-20);
//    double freq = 30 * exp(sortabase*x);

    int indh = double(height()-event->y()-10) / (height()-20) * heights;
    int indw = double(event->x()-10) / (width()-20) * widths;
    double freq = freqMin * pow(frequencyMultiplent, indh);
    double val;
    int ind = indw*heights+indh;
    if(ind < 0 || ind >= heights*widths) {
        val = nan("hello");
    } else {
        val = data[ind];
    }
    fprintf(stderr, "indh %d indw %d freq %lf val %lf", indh, indw, freq, val);
}

