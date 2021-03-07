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
                 double max_);
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
signals:

public slots:
private:
    double*data;
    int heights;
    int widths;
    double max;
};

#endif // GRAPH_H
