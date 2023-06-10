#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QImage>
#include <QMouseEvent>
//#include "cnl/include/cnl/all.h"
#include <QElapsedTimer>
#include <complex>
#include <thread>
#define IterATIONnUMBER 50
//100
using namespace std::complex_literals;
const int w = 700, h = 700;
//const double le = -1.75-0.02, ri = -1.75+0.02,
//    dow = -0.02,
//        up = 0.02;
double le = -2, ri = 2,
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


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    i(w,h, QImage::Format_RGB32),
    trajectory(w,h, QImage::Format_RGBA8888)
{
    ui->setupUi(this);

    auto pc = new ModeComboBox(&mode, this);
    mode = modeIterations;
    pc->move(10, 10);
    resize(w, h);
    doPic();
    trajectory.fill(00000000);
}

Widget::~Widget()
{
    delete ui;
}

constexpr double tau = 6.283185307179586;
// Times:
// 3308
// 1919
// 1686
// 226
// 3390
// 241
// 3241
QPointF toPoint(std::complex<double> end) {
    double xe = double( end.real()  -le)*w/(ri-le);
    double ye = double(-end.imag() - dow)*h/(up-dow);
    return {xe, ye};
}
std::complex<double> toNumber(QPointF end) {
    double xe = end.x()*(ri-le)/w + le;
    double ye = ((h-1-end.y())*(up-dow)/h+dow);
    return std::complex<double>(xe, ye);
}
inline std::complex<double> func(std::complex<double> adjusted, std::complex<double> c, double branchCutArg) {
    return std::exp((std::log(adjusted)-std::complex<double>(0.0, branchCutArg)/*branchCutArg*/)*(2.5)) + c;
}
//    return std::conj(std::exp((std::log(p/**roo*/)/*+ere*/)*3.f
////                    std::complex<double>(0.f,  1)
//                    )) + c;
//    return c*p*(1.f-p);

//    return std::conj(std::exp((std::log(p))*(-2.f))) + c;
//    return std::conj(std::exp((std::log(p))*(-2.f))) + c;
#define NUM_TAMP_ITERS 3
//double angles[NUM_TAMP_ITERS] = {0};
void body(QImage& i, double angles[NUM_TAMP_ITERS], int st)
{

    double branchCutArg;
    std::complex<double> branchCut;
    for(int xp = 0; xp < w; xp++) {
        for(int yp = 0; yp < w; yp++) {
            double x = xp*(ri-le)/w+le;
            double y = (h-1-(double(yp)+0.1))*(up-dow)/h+dow;

            std::complex<double> c(x, y);
            std::complex<double> p = c;//0.5f;///3.f;
            int iter = 0;
            bool finished = false;
                for(; iter < 5; iter++) {
                    if( std::norm(p) > 4 ) {
                        i.setPixelColor(xp, yp, QColor::fromHsv(double(iter )/IterATIONnUMBER *360, 255, 127));
                        finished = true;
                        break;
                    }
                    branchCutArg = iter > NUM_TAMP_ITERS-1 ? angles[NUM_TAMP_ITERS-1] : angles[iter];
                    branchCut = std::polar<double>(1, branchCutArg);
                    std::complex<double> adjusted  = p*branchCut;
                    if( std::abs(adjusted.imag()) < 0.01 && adjusted.real()<0) {

                        //                                i.setPixelColor(xp, yp, QColor::fromHsv((std::arg(p)+tau/2)/tau*360, 255, sqrt(sqrt(double(iter)/200))*255));
                        i.setPixelColor(xp, yp, QColor::fromHsvF(0, 0, 1-iter/5.0));
                        finished = true;
                        break;
                    }

                    p = func(adjusted, c, branchCutArg);
                }
                branchCutArg = angles[NUM_TAMP_ITERS-1];
                branchCut = std::polar<double>(1, branchCutArg);
                if(!finished)  {
                    for(; !finished && iter < IterATIONnUMBER; iter++) {
                        if( std::norm(p) > 4 ) {
                            i.setPixelColor(xp, yp, QColor::fromHsv(double(iter )/IterATIONnUMBER *360, 255, 127));
                            finished = true;
                            break;
                        }
                        std::complex<double> adjusted  = p*branchCut;
                        if( std::abs(adjusted.imag()) < 0.01 && adjusted.real()<5 && iter < 5) {
                            i.setPixelColor(xp, yp, QColor::fromHsvF(0, 0, 1-iter/5.0));
                            finished = true;
                            break;
                        }
                        p = func(adjusted, c, branchCutArg);
                    }
                }
                if(!finished/*iter == IterATIONnUMBER*/) {
                    i.setPixelColor(xp, yp, Qt::black);
                }
            }
//        if(xp%64==0)
//            fprintf(stderr, "xp = %d, elapxsed %d ms\n", xp, t.elapsed());

    }
//    static int frameNumber = 0;
    i.save(QString("frame%0.png").arg(st));
//    if(frameNumber % 20 == 0)
#define steps 30
    fprintf(stderr, "thread %d made %dth frame (%d)\n", st/steps, st%steps, st);
}

void Widget::doPic() {
    //    QImage i(w, h, QImage::Format_RGB32);
        auto saveFrame = [this](bool b){
            static int erg = 0;
            i.save(QString("frameq")+(b?"b":"")+QString("%0.png").arg(erg, 3, 10, QChar('0')));
            erg++;
            if(!b)
                fprintf(stderr, "%d done\n", erg);

        };
        QElapsedTimer t; t.start();
        //100
//        int summand = 0;
        std::thread th0([](){
            QImage im(w,h, QImage::Format_RGB32);
            double angles[NUM_TAMP_ITERS] = {0};
        for(int i  = 0; i < steps; i++, angles[2] += tau/(steps)) {
            body(im, angles, i);
        }});
        std::thread th1([](){
            QImage im(w,h, QImage::Format_RGB32);
            double angles[NUM_TAMP_ITERS] = {0, 0, tau};
        for(int i  = 0; i < steps; i++, angles[1] += tau/(steps)) {
            body(im, angles, i+steps);
        }});
        std::thread th2([](){
            QImage im(w,h, QImage::Format_RGB32);
            double angles[NUM_TAMP_ITERS] = {0, tau, tau};
            for(int i  = 0; i < steps; i++, angles[2] += tau/(steps)) {
            body(im, angles,  i+steps*2);
        }});
        std::thread th3([](){
            QImage im(w,h, QImage::Format_RGB32);
            double angles[NUM_TAMP_ITERS] = {0, tau, 0};
        for(int i  = 0; i < steps; i++, angles[0] += tau/(steps)) {
            body(im, angles,  i+steps*3);
        }});
        std::thread th4([](){
            QImage im(w,h, QImage::Format_RGB32);
            double angles[NUM_TAMP_ITERS] = {tau, tau, 0};
        for(int i  = 0; i < steps; i++, angles[2] += tau/(steps)) {
            body(im, angles,  i+steps*4);
        }});
        std::thread th5([](){
            QImage im(w,h, QImage::Format_RGB32);
            double angles[NUM_TAMP_ITERS] = {tau, tau, tau};
        for(int i  = 0; i < steps; i++, angles[1] += tau/(steps)) {
            body(im, angles,  i+steps*5);
        }});
        std::thread th6([](){
            QImage im(w,h, QImage::Format_RGB32);
            double angles[NUM_TAMP_ITERS] = {tau, 0, tau};
        for(int i  = 0; i < steps; i++, angles[2] += tau/(steps)) {
            body(im, angles,  i+steps*6);
        }});
        std::thread th7([](){
            QImage im(w,h, QImage::Format_RGB32);
            double angles[NUM_TAMP_ITERS] = {tau, 0, 0};
        for(int i  = 0; i <= steps; i++, angles[0] += tau/(steps)) {
            body(im, angles,  i+steps*7);
        }});
        th0.join();
        th1.join();
        th2.join();
        th3.join();
        th4.join();
        th5.join();
        th6.join();
        th7.join();
//        for(double angle = 0; angle < tau*2; angle += tau/(25*25)) {
//        for(double angle = tau/4-0.1; angle < tau/4+0.01; angle += tau/30) {
    //    for(int branch = 0; branch < (1<<6); branch++) {
//        static double angle = 0;
//        angle += tau/(5*25);

//        }

    //        QPainter pa2(&i);
    //        pa2.setPen(QColor(255,255,255,127));
    //        for(int iters = 0; iters < 3; iters++) {
    //            QPointF prev;

    //            for(double cutpoint = 0; cutpoint > -3; cutpoint-=0.05) {
    //                std::complex<double> curc = cutpoint;
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

    //        std::complex<double> end = std::complex<double>(-3, 0) * roo;
    //        double xe = double( end.real()  -le)*w/(ri-le);
    //        double ye = double(-end.imag() - dow)*h/(up-dow);

    //        pa2.drawLine(QPointF(xc,yc),QPointF(xe,ye));

//            saveFrame(false);
    //    }

        fprintf(stderr, "%d\n", t.elapsed());
}


void Widget::paintEvent(QPaintEvent *)
{
    QPainter pa(this);
    pa.drawPixmap(0,0,QPixmap::fromImage(i));
    pa.drawPixmap(0,0,QPixmap::fromImage(trajectory));
    if(rect) {
        pa.drawRect(QRect(start, curm));
    }
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(mode == modeEnlarge) {
        rect = true;
        start = curm = event->pos();
        fprintf(stderr, "press %d %d\n", event->pos().x(), event->pos().y());
    }
    if(mode == modeIterations) {
        drawTrajectory(event->pos().x(), event->pos().y());
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(mode == modeEnlarge) {
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
    if(mode == modeIterations) {
        drawTrajectory(event->pos().x(), event->pos().y());
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if(mode == modeEnlarge) {
        std::complex<double> corner1 = toNumber(start);
        std::complex<double> corner2 = toNumber(curm);
        le = std::min(corner1.real(), corner2.real());
        ri = std::max(corner1.real(), corner2.real());
        up = std::max(corner1.imag(), corner2.imag());
        dow = std::min(corner1.imag(), corner2.imag());
        rect = false;
        doPic();
        update();
        fprintf(stderr, "relese %d %d\n", event->pos().x(), event->pos().y());
    }
}

void Widget::drawTrajectory(int x, int y)
{
    trajectory.fill(0);
    QPainter p(&trajectory);
    std::complex<double> c = toNumber(QPointF(x,y));
    std::complex<double> fff = c;//0.f;//toNumber(prev);
    QPointF prev = toPoint(fff);//(x, y);
    p.setPen(Qt::white);
    for(int i = 0; i < IterATIONnUMBER; i++) {
        fff = func(fff, c, 0);
        QPointF cur = toPoint(fff);
        p.drawLine(prev, cur);
        prev = cur;
    }
    update();
}

void Widget::on_horizontalSlider_sliderReleased()
{
//    angles[0] = tau/100*ui->horizontalSlider->value();
    doPic();
    repaint();
}


void Widget::on_horizontalSlider_2_sliderReleased()
{
//    angles[1] = tau/100*ui->horizontalSlider_2->value();
    doPic();
    repaint();
}


void Widget::on_horizontalSlider_3_sliderReleased()
{
//    angles[2] = tau/100*ui->horizontalSlider_3->value();
    doPic();
    repaint();
}


void Widget::on_horizontalSlider_4_sliderReleased()
{
//    angles[3] = tau/100*ui->horizontalSlider_4->value();
    doPic();
    repaint();

}


void Widget::on_horizontalSlider_5_sliderReleased()
{
//    angles[4] = tau/100*ui->horizontalSlider_5->value();
    doPic();
    repaint();

}

