#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QImage>
#include <QMouseEvent>
//#include "cnl/include/cnl/all.h"
#include <QElapsedTimer>
#include <complex>
using namespace std::complex_literals;
const int w = 700, h = 700;
//const float le = -1.75-0.02, ri = -1.75+0.02,
//    dow = -0.02,
//        up = 0.02;
float le = -2, ri = 2,
    dow = -2,
        up = 2;

class Fixed {
public:
    int a;
    Fixed operator+(Fixed b) {
        Fixed res;
        res.a = a+b.a;
        return res;
    }
    Fixed operator/(Fixed b) const {
        abort();
        return (abort(), *this);
    }
    Fixed operator-(Fixed b) {
        Fixed res;
        res.a = a-b.a;
        return res;
    }
    Fixed& operator+=(Fixed b) {
        a += b.a;
        return *this;
    }
    Fixed operator*(Fixed b) const {
        Fixed res;
        res.a = (a>>14)*(b.a>>14);
        return res;
    }
    Fixed(int b) {
        a = b << 28;
    }
    Fixed(double b) {
        a = b *(1 << 28);
    }
    Fixed() {
        a = 0;
    }
    bool outside2by2() {
        return (a < ((-1) << 29)) || (a > ((1) << 29));
    }
};

//std::complex<Fixed> toOneAndHalf(std::complex<Fixed> a) {

//}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    i(w,h, QImage::Format_RGB32)
{
    ui->setupUi(this);
    resize(w, h);
    doPic();
}

Widget::~Widget()
{
    delete ui;
}

constexpr float tau = 6.283185307179586;
// Times:
// 3308
// 1919
// 1686
// 226
// 3390
// 241
QPointF toPoint(std::complex<float> end) {
    double xe = double( end.real()  -le)*w/(ri-le);
    double ye = double(-end.imag() - dow)*h/(up-dow);
    return {xe, ye};
}
std::complex<float> toNumber(QPointF end) {
    float xe = end.x()*(ri-le)/w + le;
    float ye = -(end.y()*(up-dow)/h+dow);
    return std::complex<float>(xe, ye);
}


void Widget::doPic() {
    //    QImage i(w, h, QImage::Format_RGB32);
        auto saveFrame = [this](bool b){
            static int erg = 0;
            i.save(QString("frameq")+(b?"b":"")+QString("%0.png").arg(erg, 3, 10, '0'));
            erg++;
            if(!b)
                fprintf(stderr, "%d done\n", erg);

        };
        QElapsedTimer t; t.start();

    //    for(float angle = 0; angle < tau*2; angle += tau/(/*5**/25)) {
    //    for(float angle = tau/4-0.1; angle < tau/4+0.01; angle += tau/30) {
    //    for(int branch = 0; branch < (1<<6); branch++) {
    //    float angle = 0;

    //        std::complex<float> roo = std::polar<float>(1, angle);
    //        std::complex<float> ere = std::log(roo);
    //        if(angle >= tau/2) ere += std::complex<float>(0, tau);
    //        if(angle > 3*tau/2) ere += std::complex<float>(0, tau);
            for(int xp = 0; xp < w; xp++) {
                for(int yp = 0; yp < w; yp++) {
                    double x = xp*(ri-le)/w+le;
                    double y = (w-1-yp)*(up-dow)/h+dow;

                    std::complex<float> c(x, y);
                    std::complex<float> p = 0;//c/3.f;
                    int iter = 0;

    //                bool cut = false;
    //                if(p.real() >= 0 && fabs(p.imag()) < 1.0/100) {
    //                    i.setPixelColor(xp, yp, Qt::gray);
    //                } else {
                        for(; iter < 200; iter++) {
            //                if(p.real().outside2by2() || p.imag().outside2by2()) {
                            std::complex<float> rere = p*p*p - c*p/**p*/;
                            if( rere.real()>=0.95 &&rere.real()<=1.05&&
                                rere.imag()>=-0.05&&rere.imag()<=0.05) {

                                i.setPixelColor(xp, yp, QColor::fromHsv((std::arg(p)+tau/2)/tau*360, 255, float(iter)*170/200+85));
                                break;
                            }
                            p = p - (rere-std::complex<float>(1,0))/(std::complex<float>(3,0)*p*p - c/**std::complex<float>(2,0)*p*/);
    // z -> (z - e)(z - e2)(z - 1) = z3 - 1
    // ' -> 3z2
                        }
                        if(iter == 200) {
                            i.setPixelColor(xp, yp, Qt::black);
                        }

                    }
                if(xp%64==0)
                    fprintf(stderr, "xp = %d, elapxsed %d ms\n", xp, t.elapsed());

            }
    //        }

    //        QPainter pa2(&i);
    //        pa2.setPen(QColor(255,255,255,127));
    //        for(int iters = 0; iters < 3; iters++) {
    //            QPointF prev;

    //            for(float cutpoint = 0; cutpoint > -3; cutpoint-=0.05) {
    //                std::complex<float> curc = cutpoint;
    //                for(int i = 0; i < iters; i++) {
    //                    curc = std::exp(std::log(curc - cutpoint)/2.5f - ere)*roo;
    //                }
    //                QPointF cur = toPoint(curc);
    //                pa2.drawLine(prev, cur);
    //            }
    //        }/
    //        saveFrame(false);
    //        double xc = double(-le)*w/(ri-le);
    //        double yc = double(-dow)*h/(up-dow);

    //        std::complex<float> end = std::complex<float>(-3, 0) * roo;
    //        double xe = double( end.real()  -le)*w/(ri-le);
    //        double ye = double(-end.imag() - dow)*h/(up-dow);

    //        pa2.drawLine(QPointF(xc,yc),QPointF(xe,ye));

    //        saveFrame(false);
    //    }

        fprintf(stderr, "%d\n", t.elapsed());
}


void Widget::paintEvent(QPaintEvent *)
{
    QPainter pa(this);
    pa.drawPixmap(0,0,QPixmap::fromImage(i));
    if(rect) {
        pa.drawRect(QRect(start, curm));
    }
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    rect = true;
    start = curm = event->pos();
    fprintf(stderr, "press %d %d\n", event->pos().x(), event->pos().y());
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    bool xmo = event->pos().x() > start.x();
    bool ymo = event->pos().y() > start.y();
    int xdif = abs(event->pos().x() - start.x());
    int ydif = abs(event->pos().y() - start.y());
    curm = event->pos();
    if(xdif > ydif) {
        curm.rx() = start.x() + (xmo?(1):(-1)) * ydif;
    } else {
        curm.ry() = start.y() + (ymo?(1):(-1)) * xdif;
    }
    update();
    fprintf(stderr, "move %d %d\n", event->pos().x(), event->pos().y());
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    std::complex<float> corner1 = toNumber(start);
    std::complex<float> corner2 = toNumber(curm);
    le = std::min(corner1.real(), corner2.real());
    ri = std::max(corner1.real(), corner2.real());
    up = std::max(corner1.imag(), corner2.imag());
    dow = std::min(corner1.imag(), corner2.imag());
    rect = false;
    doPic();
    update();
    fprintf(stderr, "relese %d %d\n", event->pos().x(), event->pos().y());
}
