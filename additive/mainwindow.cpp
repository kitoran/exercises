#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"
MainWindow::MainWindow(/*QWidget *parent,*/
/*                       double *data_,
                       int size_,
                       double max_*/) :
//    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    g=ui->widget;
//    g->setData(data_, size_, max_);
}
MainWindow::~MainWindow()
{
    delete ui;
}
