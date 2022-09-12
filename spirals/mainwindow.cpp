#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"
#include <QDebug>
#include <QImage>
#include <QElapsedTimer>
#include <QPainter>
//#include <QPainter>

long long int fact(int a) {
    static std::vector<long long int> factorials{1};
    if(a < factorials.size()) {
        return factorials[a];
    }
    for(int i = factorials.size(); i <= a; i++) {
        factorials.push_back(factorials[i-1]*i);
    }
    return factorials[a];
}
struct fuck {
  int res;
    bool coprime;
};
int modulus = 1 << 5;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    i(1,1,QImage::Format_Mono),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->doubleSpinBox, &QDoubleSpinBox::editingFinished,
            this, QOverload<>::of(&MainWindow::update));
    connect(ui->doubleSpinBox_2, &QDoubleSpinBox::editingFinished,
            this, QOverload<>::of(&MainWindow::update));
}
static double virtualWidth = 30;
static double max;
QPointF MainWindow::mapLogarithmic(QPointF a) {
    double sortabase = log(max)/(height()-20);
    QPointF r = {a.x()/virtualWidth*(width()-20), exp(sortabase*a.y())};
    qDebug() << r.x() << "of" << width()-20 << "," << r.y() << sortabase << a.y() << max;
    return r;
}

QPointF MainWindow::mapLinear(QPointF a)
{
    QPointF r = {a.x()/virtualWidth*(width()-20), height() - 10 - a.y()/max*(height()-20)};
    return r;

}
double tau = 6.283185307179586476;
void MainWindow::paintEvent(QPaintEvent *event) {
//    double smallSpiralStep = tau/47;
    /* Sn = n(2a1+(n-1)d)/2
     * Sn = n*(n-1)*tau/47/2
     * n*(n-1)*tau/47/2 = tau
     * n*(n-1) = 47
     * n*n-n-47=0
     * n ~= 7

    Tn = n^2*d0/2 + n(d-(3*d0/2)) + (a+d0-d)
    = n^2*tau/47/2 - n*3*tau/47/2 + tau/47

//    double numberOfStepsInHalfSpiral
//    double sst = smallSpiralStep + tau/97/7;*/

    QPainter p(this);
    p.drawImage(0,0,i);
}

void MainWindow::update()
{
    i = QImage(width(), height(), QImage::Format_RGB32);
    i.fill(Qt::white);
    QPainter p(&i);
    double x = width()/2;
    double y = height()/2;

    if(ui->doubleSpinBox->value() == 0) {
        return;
    }
    double thStepStep = ui->doubleSpinBox->value();//sst;//(tau+(tau/*+tau/57*89)/47;*
    double thStep = 0;
    double th = 0;
    double lastX = x;
    double lastY = y;
    QElapsedTimer timer; timer.start();
    int gerg = ui->spinBox->value();//.toInt();
    for(int i = 0; i < gerg; i++) { // (thStep < tau*150000/* && timer.elapsed() <  1000*/) {
        p.drawLine(QPointF(x, y), QPointF(lastX, lastY));
        lastX = x;
        lastY = y;
        x += cos (th)*ui->doubleSpinBox_2->value();
        y += sin(th)*ui->doubleSpinBox_2->value();
        th += thStep;
        thStep += thStepStep;
        if(th > tau) th-=tau;
        if(thStep > tau) thStep-=tau;
    }
    repaint();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_spinBox_valueChanged(const QString &arg1)
{
    modulus = arg1.toInt();
    update();
}
