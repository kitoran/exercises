#ifndef GRAPH_H
#define GRAPH_H

#include <QWidget>

class graph : public QWidget
{
    Q_OBJECT
public:
    explicit graph(QWidget *parent = nullptr);
    void setData(double *data_,
                 int size1,
                 int size2,
                 double max_,
                 double freqMax_);
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
signals:

public slots:
private:
    double*data;
    int heights;
    int widths;
    double max;
    double freqMax;
};

#endif // GRAPH_H
