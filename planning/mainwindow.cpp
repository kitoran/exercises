#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "month.h"
#include <QGridLayout>
#include <QTextStream>
#include <QDateTime>
#include <QFile>
#include <QSaveFile>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Month* m = new Month;

    QDateTime dt = QDateTime::currentDateTime();
    m->setMonth(dt.date().year(), dt.date().month());
    ui->gridLayout->addWidget(m, 1, 0);

    QDir d = QDir::home();
    d.mkdir(".planning");
    QFile file(d.filePath(".planning/planned"));
    file.open(QFile::ReadOnly);
    QString line;
    QStringList dds;
    while(line = file.readLine(), !line.isEmpty()) {
        dds << line;
    }
    ddsm.setStringList(dds);
    ui->listView->setModel(&ddsm);
}

MainWindow::~MainWindow()
{
    QDir home = QDir::home();
    QSaveFile file(home.filePath(".planning/planned"));
    file.open(QFile::WriteOnly);
    QStringList dds = ddsm.stringList();
    QTextStream ts(&file);
    for(const QString& line : dds) {
        ts << line;
        ts << "\n";
    }
    file.commit();

    delete ui;
}
