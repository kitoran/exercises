#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QImage>
//#include "cnl/include/cnl/all.h"
#include <QElapsedTimer>
#include <complex>
using namespace std::complex_literals;
const int w = 700, h = 700;
//const float le = -1.75-0.02, ri = -1.75+0.02,
//    dow = -0.02,
//        up = 0.02;
const float le = -2, ri = 2,
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
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    resize(w, h);
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
void Widget::paintEvent(QPaintEvent *)
{
    QImage i(w, h, QImage::Format_RGB32);
    auto saveFrame = [&i](bool b){
        static int erg = 0;
        i.save(QString("frameq")+(b?"b":"")+QString("%0.png").arg(erg, 3, 10, '0'));
        erg++;
        if(!b)
            fprintf(stderr, "%d done\n", erg);

    };
    QElapsedTimer t; t.start();

    for(float angle = 0; angle < tau*2; angle += tau/(5*25)) {
//    for(float angle = tau/4-0.1; angle < tau/4+0.01; angle += tau/30) {
        std::complex<float> roo = std::polar<float>(1, angle);
        std::complex<float> ere = std::log(roo);
        if(angle >= tau/2) ere += std::complex<float>(0, tau);
        if(angle > 3*tau/2) ere += std::complex<float>(0, tau);
        for(int xp = 0; xp < w; xp++) {
            for(int yp = 0; yp < w; yp++) {
                double x = xp*(ri-le)/w+le;
                double y = (w-1-yp)*(up-dow)/h+dow;

                std::complex<float> c(x, y);
                std::complex<float> p = c;
                int iter = 0;
                for(; iter < 200; iter++) {
    //                if(p.real().outside2by2() || p.imag().outside2by2()) {
                    if(p.real()>=2 ||p.real()<=-2||
                    p.imag()>=2||p.imag()<=-2) {
                        i.setPixelColor(xp, yp, QColor::fromHsv(float(iter)*360/200, 255, 127));
                        break;
                    }
                    p = std::exp((std::log(p/roo)+ere)*2.5f)+c;
                }
                if(iter == 200) {
                    i.setPixelColor(xp, yp, Qt::black);
                }
                p = 0;
                iter = 0;
                for(; iter < 3; iter++) {
                    p = std::exp((std::log(p/roo)+ere)*2.5f)+c;
                    std::complex<float> rf = p / roo;
                    if(rf.real() <= 0 && fabs(rf.imag()) < 1.0/100) {
                        i.setPixelColor(xp, yp, Qt::gray);
                    }
                }
            }
        }

        saveFrame(false);
//        QPainter pa2(&i);
//        pa2.setPen(QColor(255,255,255,127));
//        double xc = double(-le)*w/(ri-le);
//        double yc = double(-dow)*h/(up-dow);

//        std::complex<float> end = std::complex<float>(-3, 0) * roo;
//        double xe = double( end.real()  -le)*w/(ri-le);
//        double ye = double(-end.imag() - dow)*h/(up-dow);

//        pa2.drawLine(QPointF(xc,yc),QPointF(xe,ye));

//        saveFrame(true);
    }

    fprintf(stderr, "%d\n", t.elapsed());
    QPainter pa(this);
    pa.drawPixmap(0,0,QPixmap::fromImage(i));
}
