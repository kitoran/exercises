#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"
#include <QPainter>
#include <QMouseEvent>
extern const double frequencyMultiplent;
MainWindow::MainWindow(/*QWidget *parent,*/
                       double *data_,
                       int size_,
                       double max_) :
//    QMainWindow(parent),
    ui(new Ui::MainWindow),
    data(data_),
    size(size_),
    max(max_)
{
    ui->setupUi(this);
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    p.drawLine(QPointF{10, height()-10},
               QPointF{width() - 10, height()-10});
//    p.drawLine(QPointF{10, height()-10},
//               QPointF{10, 10});

    double sortabase = log(20000.0/30)/(width()-20);
    for(int x = 0; x < width()-20; x += 100) {
        double freq = 30 * exp(sortabase*x);
        p.drawLine(QPointF{x+10, height()-10},
                   QPointF{x+10, 10});
        p.drawText(QPointF{x+10, height()-10},
                   QString::number(freq, ' ', 2));
    }
    QPointF last(10, height()-10);
    for(int i = 0; i < size; i++) {
        double freq = 30 * pow(frequencyMultiplent, i);
        double x = log(freq/30)/sortabase;
        double y = data[i]/max*(height()-20);
        QPointF ne{x+10, height()-10-y};
        p.drawLine(last, ne);
        last = ne;
//        p.drawText(QPointF{x+10, height()-10},
//                   QString::number(freq, ' ', 2));
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
//    double sortabase = log(20000.0/30)/(width()-20);
//    double freq = 30 * exp(sortabase*x);

    int ind = double(event->x()-10) / (width()-20) * size;
    double freq = 30 * pow(frequencyMultiplent, ind);
    double val;
    if(ind < 0 || ind >= size) {
        val = nan("hello");
    } else {
        val = data[ind];
    }
    fprintf(stderr, "ind %d freq %lf val %lf", ind, freq, val);
}

MainWindow::~MainWindow()
{
    delete ui;
}
