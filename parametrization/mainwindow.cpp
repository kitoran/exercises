#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include "math.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}
double integX = 0;
double integY = 0;

const double theCoef = sqrt(3)/2;
void drawKoch(QPainter* p, QPointF s, QPointF e, double ratioB, double thisThing) {
    if((e - s).manhattanLength() < 1) {
//        p->setPen(QColor(ratioB*255,ratioB*255,ratioB*255));

        s.setX(s.x() - 500*ratioB);
        e.setX(e.x() - 500*ratioB);

        p->drawLine(s, e);
        p->drawPoint(500 + ratioB * 300, s.y() - 500 + 250);
        p->drawPoint(500 + ratioB * 300, -s.x()/2 + 500);

        return;
    }
    QPointF th = (e-s)/3;
    QPointF fst = s+th;
    QPointF orth = {th.y(), -th.x()};
    QPointF snd = (s+e)/2 +orth*theCoef;
    QPointF trd = e-th;

    drawKoch(p, s, fst, ratioB, thisThing/4);
    drawKoch(p, fst, snd, ratioB + thisThing/4, thisThing/4);
    drawKoch(p, snd, trd, ratioB + thisThing/4*2, thisThing/4);
    drawKoch(p, trd, e, ratioB + thisThing/4*3, thisThing/4);
}

void drawSier(QPainter* p, QPointF s, QPointF e, bool orientation, double ratioB, double thisThing) {
    if((e - s).manhattanLength() < 1) {
        p->setPen(QColor::fromHsv(ratioB*360, 255, 127));

        //        s.setX(s.x() - 500/2);
        //        e.setX(e.x() - 500/2);
        s.setX(s.x() - 500*ratioB);
        e.setX(e.x() - 500*ratioB);
        s.setY(s.y() + 500*theCoef/3);
        e.setY(e.y() + 500*theCoef/3);

        p->drawLine(s, e);

        p->drawPoint(500 + ratioB * 300, (s.y() - 500)/2 + 250);
        p->drawPoint(500 + ratioB * 300, -s.x()/2 + 500);


        integY += (500-s.y())/30000;
        integX += (s.x())/35000;
        p->drawPoint(800 + ratioB * 300, -integY+ 250);
        p->drawPoint(800 + ratioB * 300, -integX + 500);

        p->drawPoint(500 + integX, 500-integY);


        return;
    }
    QPointF hal = (e-s)/2;
//    QPointF fst = s + hal;
    QPointF orth = {hal.y(), -hal.x()};
    if(orientation) orth = -orth;
    QPointF snd = s*3/4 + e/4 +orth*theCoef;
    QPointF trd = s*1/4 + e*3/4 +orth*theCoef;

    drawSier(p, s, snd, !orientation, ratioB, thisThing/3);
    drawSier(p, snd, trd, orientation, ratioB + thisThing/3, thisThing/3);
    drawSier(p, trd, e, !orientation, ratioB + thisThing/3*2, thisThing/3);
}
void drawHilb(QPainter* p, QPointF s, QPointF e, bool orientation, double ratioB, double thisThing) {
    if((e - s).manhattanLength() < 1) {
        p->setPen(QColor::fromHsv(ratioB*360, 255, 255));

//                s.setX(s.x() - 500/2);
//                e.setX(e.x() - 500/2);
        s.setX(s.x() - 500*ratioB);
        e.setX(e.x() - 500*ratioB);
        s.setY(s.y() + 500/2);
        e.setY(e.y() + 500/2);

        p->drawLine(s, e);

        p->drawPoint(500 + ratioB * 300, (s.y() - 500)/2 + 250);
        p->drawPoint(500 + ratioB * 300, -s.x()/2 + 500);


        integY += (500-s.y())/30000;
        integX += (s.x())/35000;
        p->drawPoint(800 + ratioB * 300, -integY+ 250);
        p->drawPoint(800 + ratioB * 300, -integX + 500);

        p->drawPoint(500 + integX, 500-integY);


        return;
    }
    QPointF hal = (e-s)/2;
//    QPointF fst = s + hal;
    QPointF orth = {hal.y(), -hal.x()};
    if(orientation) orth = -orth;
    QPointF snd = s+orth;
    QPointF trd = s+hal+orth;
    QPointF frth = e+orth;

    drawHilb(p, s, snd, !orientation, ratioB, thisThing/4);
    drawHilb(p, snd, trd, orientation, ratioB + thisThing/4, thisThing/4);
    drawHilb(p, trd, frth, orientation, ratioB + thisThing/4*2, thisThing/4);
    drawHilb(p, frth, e, !orientation, ratioB + thisThing/4*3, thisThing/4);
}

void MainWindow::paintEvent(QPaintEvent *event) {
    integX = 0;
    integY = 0;

    QPainter p(this);
    QPointF start = {0, 500};
    QPointF end = {500, 500};
    p.drawLine(start, end);
    p.drawEllipse(start, 20, 30);
    p.drawEllipse(end, 40, 50);
    drawHilb(&p, start, end, false, 0, 1);
}

MainWindow::~MainWindow()
{
    delete ui;
}
