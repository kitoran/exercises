#ifndef GRAPH_H
#define GRAPH_H

#include <QWidget>
#include <spectrogram.h>

//#include <synthesis.h>
#include "ambiguousarray.h"
#include <complex>
class graph : public QWidget
{
    Q_OBJECT
public:
    explicit graph(QWidget *parent = nullptr);
//    void setLogarithmicData(double *data_,
//                 int size1,
//                 int size2,
//                 double max_,
//                 double freqMax_);
//    void setLinearData(double *data_, int width_, int height_,
//                 int windowSize,
//                 int samplerate,
//                 double max_);
    bool selecting{false};
    int selectStart{-1};
    int selectEnd{-1};
    int rangeStartInArray();
    int rangeEndInArray();
    QImage spectrogramDrawing;
//    QWidget
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void selectRange();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
//    void setComplexData(std::complex<double> *data_, int width_, int height_, int windowSize_, int samplerate_, double max_);
signals:

public slots:
private:
//    Spectrogram* spectrogram;
//    int widths;
//    uint heights;
//    uint windowSize;
//    double max;
//    double freqMax;
//    int samplerate;
};

#endif // GRAPH_H
