#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}
class graph;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(/*QWidget *parent = 0,*/
                        /*double* data,
                        int size,
                        double max*/);
    ~MainWindow();

    graph* g;
private slots:
    void on_toolButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
