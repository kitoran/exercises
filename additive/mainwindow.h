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
    explicit MainWindow(/*QWidget *parent = 0,*/
                        double* data,
                        int size,
                        double max);
    ~MainWindow();

    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    Ui::MainWindow *ui;
    double*data;
    int size;
    double max;
};

#endif // MAINWINDOW_H
