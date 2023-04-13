#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include "shittyintrospection.h"

#include <QComboBox>
//#include <QElsapsed>
#include <QSettings>
class QElapsedTimer;
INTROSPECT_ENUM_VISIBLE_NAMES_PERSISTENT_COMBOBOX_ENUMNAME(Mode,
        modeEnlarge, enlarge,
        modeIterations, showiterations)
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    Mode mode;
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void paintEvent(QPaintEvent*) override;

    QImage i;
    QImage trajectory;
    QPoint start;
    QPoint curm;
    bool rect = false;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void drawTrajectory(int x, int y);
    void doPic();
    void body(QElapsedTimer t, float angles[]);
    
private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
