#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include "graph.h"
#include <gtk/gtk.h>

//GtkGrid *gridLayout;
GtkLabel *label_2;
GtkLabel *label_3;
GtkToolButton *toolButton;
GtkComboBox *comboBox;
GtkEntry *denominatorEntry;
GtkEntry *lineEdit_2;
GtkEntry *lineEdit;
GtkLabel *label_5;
GtkLabel *label;
GtkSpinButton *spinBox_2;
typedef struct graph graph;
struct graph *widget;
GtkLabel *label_8;
GtkToolButton *toolButton_2;
GtkEntry *windowFrequencyLineEdit;
GtkEntry *windowLengthLineEdit;
GtkLabel *label_4;
GtkLabel *label_7;
GtkButton *pushButton;
GtkLabel *label_6;
GtkEntry *numberOfHarmonicsLineEdit;

void setupUi(GtkGrid  *gridLayout)
{
//    if (MainWindow->objectName().isEmpty())
//        gtkSeMainWindow->setObjectN ame(QString::fromUtf8("MainWindow"));
    gtk_widget_set_name(gridLayout, "MainWindow");
    gtk_widget_set_size_request(gridLayout, 992, 402);

//    gridLayout = gtk_grid_new();
    gtk_grid_set_column_spacing(gridLayout, 6);
    gtk_grid_set_row_spacing(gridLayout, 6);
    gtk_widget_set_margin_bottom(gridLayout, 11);
    gtk_widget_set_margin_top(gridLayout, 11);
    gtk_widget_set_margin_right(gridLayout, 11);
    gtk_widget_set_margin_left(gridLayout, 11);//gridLayout->setContentsMargins(11, 11, 11, 11);
    gtk_widget_set_name(gridLayout, "gridLayout");
    label_2 = gtk_label_new(0);//new QLabel(centralWidget);
    gtk_widget_set_name(label_2, "label_2");

    gtk_grid_attach(gridLayout, label_2, 0, 8, 1, 1);

    label_3 = gtk_label_new(0);
    gtk_widget_set_name(label_3, "label_3");

    gtk_grid_attach(gridLayout, label_3, 0, 12, 1, 1);

    toolButton = gtk_tool_button_new(0, 0);
    gtk_widget_set_name(toolButton, "toolButton");

    GdkCursor* cursor;
    cursor = gdk_cursor_new_for_display(gdk_display_get_default(),
                            GDK_SB_RIGHT_ARROW);
    gtk_widget_set_cursor(toolButton, cursor);
    g_object_unref(cursor);


    gtk_grid_attach(gridLayout, toolButton, 2, 11, 1, 1);

    comboBox = gtk_combo_box_new();
//    comboBox->addItem(QString());
//    comboBox->addItem(QString());
//    comboBox->addItem(QString());
    gtk_widget_set_name(comboBox, "comboBox");

    gtk_grid_attach(gridLayout, comboBox, 2, 1, 1, 1);

    denominatorEntry = gtk_entry_new();
    gtk_widget_set_name(denominatorEntry, "denominator");

    gtk_grid_attach(gridLayout, denominatorEntry, 2, 16, 1, 1);

    lineEdit_2 = gtk_entry_new();
    gtk_widget_set_name(lineEdit_2, "lineEdit_2");

    gtk_grid_attach(gridLayout, lineEdit_2, 2, 2, 1, 1);

    lineEdit = gtk_entry_new();
    gtk_widget_set_name(lineEdit, "lineEdit");
//    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    sizePolicy.setHorizontalStretch(0);
//    sizePolicy.setVerticalStretch(0);
//    sizePolicy.setHeightForWidth(lineEdit->sizePolicy().hasHeightForWidth());
//    lineEdit->setSizePolicy(sizePolicy);

    gtk_grid_attach(gridLayout, lineEdit, 2, 0, 1, 1);

    label_5 = gtk_label_new(0);
    gtk_widget_set_name(label_5, "label_5");

    gtk_grid_attach(gridLayout, label_5, 0, 2, 1, 1);

    label = gtk_label_new(0);
    gtk_widget_set_name(label, "label");

    gtk_grid_attach(gridLayout, label, 0, 1, 1, 1);

    spinBox_2 = gtk_spin_button_new(0,1,5);
    gtk_widget_set_name(spinBox_2, "spinBox_2");
    gtk_spin_button_set_value(spinBox_2, 44100);

    gtk_grid_attach(gridLayout, spinBox_2, 2, 5, 1, 1);

    widget = malloc(sizeof(graph));
    gtk_widget_set_name(widget, "widget");
//    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
//    sizePolicy1.setHorizontalStretch(0);
//    sizePolicy1.setVerticalStretch(0);
//    sizePolicy1.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
//    widget->setSizePolicy(sizePolicy1);

    gtk_grid_attach(gridLayout, widget, 4, 0, 1, 17);

    label_8 = gtk_label_new(0);
    gtk_widget_set_name(label_8, "label_8");

    gtk_grid_attach(gridLayout, label_8, 0, 16, 1, 1);

    toolButton_2 = gtk_menu_tool_button_new(0, 0);
    gtk_widget_set_name(toolButton_2, "toolButton_2");

    gtk_grid_attach(gridLayout, toolButton_2, 2, 10, 1, 1);

    windowFrequencyLineEdit = gtk_entry_new();
    gtk_widget_set_name(windowFrequencyLineEdit, "windowFrequencyLineEdit");
//    sizePolicy.setHeightForWidth(windowFrequencyLineEdit->sizePolicy().hasHeightForWidth());
//    windowFrequencyLineEdit->setSizePolicy(sizePolicy);

    gtk_grid_attach(gridLayout, windowFrequencyLineEdit, 2, 12, 1, 1);

    windowLengthLineEdit = gtk_entry_new();
    gtk_widget_set_name(windowLengthLineEdit, "windowLengthLineEdit");
//    sizePolicy.setHeightForWidth(windowLengthLineEdit->sizePolicy().hasHeightForWidth());
//    windowLengthLineEdit->setSizePolicy(sizePolicy);

    gtk_grid_attach(gridLayout, windowLengthLineEdit, 2, 8, 1, 1);

    label_4 = gtk_label_new(0);
    gtk_widget_set_name(label_4, "label_4");

    gtk_grid_attach(gridLayout, label_4, 0, 0, 1, 1);

    label_7 = gtk_label_new(0);
    gtk_widget_set_name(label_7, "label_7");

    gtk_grid_attach(gridLayout, label_7, 0, 5, 1, 1);

    pushButton = gtk_button_new();
    gtk_widget_set_name(pushButton, "pushButton");

    gtk_grid_attach(gridLayout, pushButton, 3, 0, 1, 1);

    label_6 = gtk_label_new(0);
    gtk_widget_set_name(label_6, "label_6");

    gtk_grid_attach(gridLayout, label_6, 0, 6, 1, 2);

    numberOfHarmonicsLineEdit = gtk_entry_new();
    gtk_widget_set_name(numberOfHarmonicsLineEdit, "numberOfHarmonicsLineEdit");

    gtk_grid_attach(gridLayout, numberOfHarmonicsLineEdit, 2, 6, 1, 2);

//    gtk_add_chwidget_MainWindow->setCentralWidget(centralWidget);
//    menuBar = new QMenuBar(MainWindow);
//    gtk_widget_set_name(menuBar, "menuBar");
//    menuBar->setGeometry(QRect(0, 0, 992, 21));
//    MainWindow->setMenuBar(menuBar);

//    retranslateUi(MainWindow);

//    QMetaObject::connectSlotsByName(MainWindow);
} // setupUi

//    void retranslateUi(QMainWindow *MainWindow)
//    {
//        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
//        label_2->setText(QApplication::translate("MainWindow", "window (sampls)", nullptr));
//        label_3->setText(QApplication::translate("MainWindow", "windowFrequency (sampls)", nullptr));
//        toolButton->setText(QApplication::translate("MainWindow", "...", nullptr));
//        comboBox->setItemText(0, QApplication::translate("MainWindow", "original", nullptr));
//        comboBox->setItemText(1, QApplication::translate("MainWindow", "multiply", nullptr));
//        comboBox->setItemText(2, QApplication::translate("MainWindow", "divide", nullptr));

//        label_5->setText(QApplication::translate("MainWindow", "treshold", nullptr));
//        label->setText(QApplication::translate("MainWindow", "method", nullptr));
//        label_8->setText(QApplication::translate("MainWindow", "denominstor", nullptr));
//        toolButton_2->setText(QApplication::translate("MainWindow", "\342\207\274", nullptr));
//#ifndef QT_NO_SHORTCUT
//        toolButton_2->setShortcut(QApplication::translate("MainWindow", "Ctrl+R", nullptr));
//#endif // QT_NO_SHORTCUT
//        label_4->setText(QApplication::translate("MainWindow", "max", nullptr));
//        label_7->setText(QApplication::translate("MainWindow", "frequency cadr", nullptr));
//        pushButton->setText(QApplication::translate("MainWindow", "audacity", nullptr));
//        label_6->setText(QApplication::translate("MainWindow", "number of harmonics", nullptr));
//        numberOfHarmonicsLineEdit->setText(QApplication::translate("MainWindow", "10", nullptr));
//    } // retranslateUi

//};

//namespace Ui {
//    class MainWindow: public Ui_MainWindow {};
//} // namespace Ui


#endif // UI_MAINWINDOW_H
