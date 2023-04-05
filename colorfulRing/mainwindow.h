#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent *event);

    void drawFactorialLogarithmic(QPainter *p);
    void drawInverse(QPainter *p);
    QPointF mapLogarithmic(QPointF a);
    QPointF mapLinear(QPointF a);
    void drawVerticalCoordinatesLogarithmic(QPainter *p);
    void drawVerticalCoordinatesIntegral(QPainter *p);
    void drawHorizontalCoordinatesIntegral(QPainter *p);
private slots:
    void on_spinBox_valueChanged(const int &arg1);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
