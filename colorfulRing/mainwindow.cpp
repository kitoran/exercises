#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"
#include <QDebug>
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
fuck inverse(int a) {
//    Q_ASSERT(a & 1 == 1);
    long long int rold = a;
    long long int rnew = modulus;
    long long int sold = 1;
    long long int snew = 0;
    long long int told = 0;
    long long int tnew = 1;

    while(rnew != 0) {
        long long int q = rold/rnew;
        long long int r = rold - q*rnew;
        long long int s = sold - q*snew;
        long long int t = told - q*tnew;

        rold = rnew; rnew = r;
        sold = snew; snew = s;
        told = tnew; tnew = t;
    }
    int gcd = rold;
    int res = sold;

    if(res < 0) {
        res += modulus;
    }
    return {res, gcd == 1};
}
int intLog2(long long int l) {
    int res = 0;
    while(l >>= 1) {
        res++;
    }
    return res;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
//    connect(ui->spinBox, &QSpinBox::valueChanged
    ui->setupUi(this);
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

void MainWindow::drawFactorialLogarithmic(QPainter* p) {
    for(int i = 0; i < virtualWidth; i++) {
        p->drawEllipse(
                    mapLogarithmic(
                        {
                            qreal(i),
                            qreal(fact(
                                i
                            ))
                        }),
                    2,
                    2);
    }
}

void MainWindow::drawInverse(QPainter* p) {
    for(int i = 0; i < virtualWidth; i++) {
        fuck r = inverse(i);
        p->setPen(r.coprime? Qt::black : Qt::gray);

        p->drawLine(mapLinear({qreal(i), 0}), mapLinear({qreal(i), qreal(r.res)}));
    }
}

void MainWindow::drawVerticalCoordinatesLogarithmic(QPainter* p) {
    double sortabase = log(max)/(height()-20);
    for(int y = 0; y < height()-20; y += 100) {
        double val = exp(sortabase*y);
//        p->drawLine(QPointF{10., height()-10.-y},
//                   QPointF{width()-10., height()-10.-y});
//        p->drawText(QPointF{10., height()-10.-y},
//                   QString::number(val, ' ', 2));
    }
}

void MainWindow::drawVerticalCoordinatesIntegral(QPainter* p) {
    for(int y = 0; y < height()-20; y += 100) {
        double val = (height()-20)/max*y;
//        p->drawLine(QPointF{10., height()-10.-y},
//                   QPointF{width()-10., height()-10.-y});
//        p->drawText(QPointF{10., height()-10.-y},
//                   QString::number(val, ' ', 2));
    }
}

void MainWindow::drawHorizontalCoordinatesIntegral(QPainter* p) {
    for(int x = 0; x < virtualWidth; x++) {
        double val = (width()-20)/virtualWidth*x;
//        p->drawLine(QPointF{x, 10.},
//                   QPointF{x, height()-10.});
//        p->drawText(QPointF{x, height()-10.},
//                   QString::number(val, ' ', 2));
    }
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    max = virtualWidth = modulus;//fact(virtualWidth);
//    drawVerticalCoordinatesIntegral(&p);
//    drawHorizontalCoordinatesIntegral(&p);
//    drawInverse(&p);
    int width = 200;
    for(int i = 0; i < modulus; i++) {
        for(int j = 0; j < modulus; j++) {
            QColor c = QColor::fromHsvF(((i*j)%modulus)*1.0/modulus, 1, 1);
            QRect rf = {i*width/modulus, j*width/modulus, (i+1)*width/modulus,
                        (j+1)*width/modulus};

            p.setBrush(c);
            p.setPen(Qt::NoPen);
            int uu = ui->spinBox->y() + ui->spinBox->height()*3;
            p.drawRect(rf.adjusted(0,uu, 0, uu));
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_spinBox_valueChanged(const int &arg1)
{
    modulus = arg1;
    update();
}
