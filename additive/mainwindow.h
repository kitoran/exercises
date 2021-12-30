#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <gtk/gtk.h>

//struct graph;
//struct MainWindow
//{
//    GtkWidget parent;
//    struct graph* g;
//};
void MainWindowon_toolButton_clicked(GtkWidget*, void*);
void MainWindowon_denominator_textChanged(GtkWidget*, void*);//struct MainWindow, const char* arg1);
void MainWindowon_toolButton_2_clicked(GtkWidget*, void*);//struct MainWindow);
void MainWindowon_pushButton_clicked(GtkWidget*, void*);//struct MainWindow);
void MainWindowon_numberOfHarmonicsLineEdit_textChanged(GtkWidget*, void*);//struct MainWindow, const char* arg1);



#endif // MAINWINDOW_H
