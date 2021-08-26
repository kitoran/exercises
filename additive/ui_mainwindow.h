#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include "graph.h"
#include <gtk/gtk.h>

GtkGrid grid;
GtkLabel *label_2;
GtkLabel *label_3;
GtkToolButton *toolButton;
GtkComboBox *comboBox;
GtkLineEdit *denominator;
GtkLineEdit *lineEdit_2;
GtkLineEdit *lineEdit;
GtkLabel *label_5;
GtkLabel *label;
GtkSpinBox *spinBox_2;
graph *widget;
GtkLabel *label_8;
GtkToolButton *toolButton_2;
GtkLineEdit *windowFrequencyLineEdit;
GtkLineEdit *windowLengthLineEdit;
GtkLabel *label_4;
GtkLabel *label_7;
GtkPushButton *pushButton;
GtkLabel *label_6;
GtkLineEdit *numberOfHarmonicsLineEdit;

void setupUi(QMainWindow *MainWindow)
{
    if (MainWindow->objectName().isEmpty())
        MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
    MainWindow->resize(992, 402);
    centralWidget = new QWidget(MainWindow);
    centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
    gridLayout = new QGridLayout(centralWidget);
    gridLayout->setSpacing(6);
    gridLayout->setContentsMargins(11, 11, 11, 11);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    label_2 = new QLabel(centralWidget);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    gridLayout->addWidget(label_2, 0, 8, 1, 1);

    label_3 = new QLabel(centralWidget);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    gridLayout->addWidget(label_3, 0, 12, 1, 1);

    toolButton = new QToolButton(centralWidget);
    toolButton->setObjectName(QString::fromUtf8("toolButton"));
    toolButton->setArrowType(Qt::RightArrow);

    gridLayout->addWidget(toolButton, 2, 11, 1, 1);

    comboBox = new QComboBox(centralWidget);
    comboBox->addItem(QString());
    comboBox->addItem(QString());
    comboBox->addItem(QString());
    comboBox->setObjectName(QString::fromUtf8("comboBox"));

    gridLayout->addWidget(comboBox, 2, 1, 1, 1);

    denominator = new QLineEdit(centralWidget);
    denominator->setObjectName(QString::fromUtf8("denominator"));

    gridLayout->addWidget(denominator, 2, 16, 1, 1);

    lineEdit_2 = new QLineEdit(centralWidget);
    lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

    gridLayout->addWidget(lineEdit_2, 2, 2, 1, 1);

    lineEdit = new QLineEdit(centralWidget);
    lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(lineEdit->sizePolicy().hasHeightForWidth());
    lineEdit->setSizePolicy(sizePolicy);

    gridLayout->addWidget(lineEdit, 2, 0, 1, 1);

    label_5 = new QLabel(centralWidget);
    label_5->setObjectName(QString::fromUtf8("label_5"));

    gridLayout->addWidget(label_5, 0, 2, 1, 1);

    label = new QLabel(centralWidget);
    label->setObjectName(QString::fromUtf8("label"));

    gridLayout->addWidget(label, 0, 1, 1, 1);

    spinBox_2 = new QSpinBox(centralWidget);
    spinBox_2->setObjectName(QString::fromUtf8("spinBox_2"));
    spinBox_2->setMaximum(48000);
    spinBox_2->setValue(44100);

    gridLayout->addWidget(spinBox_2, 2, 5, 1, 1);

    widget = new graph(centralWidget);
    widget->setObjectName(QString::fromUtf8("widget"));
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
    widget->setSizePolicy(sizePolicy1);

    gridLayout->addWidget(widget, 4, 0, 1, 17);

    label_8 = new QLabel(centralWidget);
    label_8->setObjectName(QString::fromUtf8("label_8"));

    gridLayout->addWidget(label_8, 0, 16, 1, 1);

    toolButton_2 = new QToolButton(centralWidget);
    toolButton_2->setObjectName(QString::fromUtf8("toolButton_2"));

    gridLayout->addWidget(toolButton_2, 2, 10, 1, 1);

    windowFrequencyLineEdit = new QLineEdit(centralWidget);
    windowFrequencyLineEdit->setObjectName(QString::fromUtf8("windowFrequencyLineEdit"));
    sizePolicy.setHeightForWidth(windowFrequencyLineEdit->sizePolicy().hasHeightForWidth());
    windowFrequencyLineEdit->setSizePolicy(sizePolicy);

    gridLayout->addWidget(windowFrequencyLineEdit, 2, 12, 1, 1);

    windowLengthLineEdit = new QLineEdit(centralWidget);
    windowLengthLineEdit->setObjectName(QString::fromUtf8("windowLengthLineEdit"));
    sizePolicy.setHeightForWidth(windowLengthLineEdit->sizePolicy().hasHeightForWidth());
    windowLengthLineEdit->setSizePolicy(sizePolicy);

    gridLayout->addWidget(windowLengthLineEdit, 2, 8, 1, 1);

    label_4 = new QLabel(centralWidget);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    gridLayout->addWidget(label_4, 0, 0, 1, 1);

    label_7 = new QLabel(centralWidget);
    label_7->setObjectName(QString::fromUtf8("label_7"));

    gridLayout->addWidget(label_7, 0, 5, 1, 1);

    pushButton = new QPushButton(centralWidget);
    pushButton->setObjectName(QString::fromUtf8("pushButton"));

    gridLayout->addWidget(pushButton, 3, 0, 1, 1);

    label_6 = new QLabel(centralWidget);
    label_6->setObjectName(QString::fromUtf8("label_6"));

    gridLayout->addWidget(label_6, 0, 6, 1, 2);

    numberOfHarmonicsLineEdit = new QLineEdit(centralWidget);
    numberOfHarmonicsLineEdit->setObjectName(QString::fromUtf8("numberOfHarmonicsLineEdit"));

    gridLayout->addWidget(numberOfHarmonicsLineEdit, 2, 6, 1, 2);

    MainWindow->setCentralWidget(centralWidget);
    menuBar = new QMenuBar(MainWindow);
    menuBar->setObjectName(QString::fromUtf8("menuBar"));
    menuBar->setGeometry(QRect(0, 0, 992, 21));
    MainWindow->setMenuBar(menuBar);

    retranslateUi(MainWindow);

    QMetaObject::connectSlotsByName(MainWindow);
} // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "window (sampls)", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "windowFrequency (sampls)", nullptr));
        toolButton->setText(QApplication::translate("MainWindow", "...", nullptr));
        comboBox->setItemText(0, QApplication::translate("MainWindow", "original", nullptr));
        comboBox->setItemText(1, QApplication::translate("MainWindow", "multiply", nullptr));
        comboBox->setItemText(2, QApplication::translate("MainWindow", "divide", nullptr));

        label_5->setText(QApplication::translate("MainWindow", "treshold", nullptr));
        label->setText(QApplication::translate("MainWindow", "method", nullptr));
        label_8->setText(QApplication::translate("MainWindow", "denominstor", nullptr));
        toolButton_2->setText(QApplication::translate("MainWindow", "\342\207\274", nullptr));
#ifndef QT_NO_SHORTCUT
        toolButton_2->setShortcut(QApplication::translate("MainWindow", "Ctrl+R", nullptr));
#endif // QT_NO_SHORTCUT
        label_4->setText(QApplication::translate("MainWindow", "max", nullptr));
        label_7->setText(QApplication::translate("MainWindow", "frequency cadr", nullptr));
        pushButton->setText(QApplication::translate("MainWindow", "audacity", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "number of harmonics", nullptr));
        numberOfHarmonicsLineEdit->setText(QApplication::translate("MainWindow", "10", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
