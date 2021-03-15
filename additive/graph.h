#ifndef GRAPH_H
#define GRAPH_H

#include <QWidget>

#include <synthesis.h>
class graph : public QWidget
{
    Q_OBJECT
public:
    spectrogram_mode mode;
    explicit graph(QWidget *parent = nullptr);
    void setLogarithmicData(double *data_,
                 int size1,
                 int size2,
                 double max_,
                 double freqMax_);
    void setLinearData(double *data_, int width_, int height_,
                 int windowSize,
                 int samplerate,
                 double max_);

    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
signals:

public slots:
private:
    double*data;
    int heights;
    int windowSize;
    int widths;
    double max;
    double freqMax;
    int samplerate;
};

#endif // GRAPH_H
