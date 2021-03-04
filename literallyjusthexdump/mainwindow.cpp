#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <thread>
#include <unistd.h>
#include <QTimer>
#include <QThread>
#include <fcntl.h>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QThread * ddmidi = QThread::create([=](){
        QByteArray data;
        int fd = open("/dev/dmmidi2", O_RDONLY);
        while(true) {
            char byte;
            if(read(fd, &byte, 1) == 1) {
                data.append(byte);
                QTimer::singleShot(0, this, [=](){
                    ui->ddmidi2->setText((data.toHex()));
                });
            }
        }

    });
    ddmidi->start();
    QThread*  midi= QThread::create([=](){
        QByteArray data;
        int fd = open("/dev/midi2", O_RDONLY);
        while(true) {
            char byte;
            if(read(fd, &byte, 1) == 1) {
                data.append(byte);
                QTimer::singleShot(0, this, [=](){
                    ui->midi2->setText((data.toHex()));
                });
            }
        }

    });
    midi->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}
