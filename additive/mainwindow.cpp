#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"
#include "soundext.h"
#include "spectrogram.h"
#include "synthesis.h"
#include "globals.h"
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

void MainWindow::on_toolButton_clicked()
{
    ContMaximaSpectrogram* s = dynamic_cast<ContMaximaSpectrogram*>(spectrogram);
    resynthesizeMaxima(s, g->rangeStartInArray(), g->rangeEndInArray());
//    alsaPlayBlock(audioOutput.data(), audioOutput.size());
    alsaPlayBlock(audioOutput.data(), audioOutput.size());
 }

void MainWindow::on_lineEdit_3_textChanged(const QString &arg1)
{
    denominator = arg1.toDouble();
}

void MainWindow::on_toolButton_2_clicked()
{
    g->selectRange();
}
