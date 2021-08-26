#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <gtk/gtk.h>

struct graph;
struct MainWindow
{
    GtkWidget parent;
    struct graph* g;
};
void MainWindowon_toolButton_clicked(struct MainWindow);
void MainWindowon_denominator_textChanged(struct MainWindow, const char* arg1);
void MainWindowon_toolButton_2_clicked(struct MainWindow);
void MainWindowon_pushButton_clicked(struct MainWindow);
void MainWindowon_numberOfHarmonicsLineEdit_textChanged(struct MainWindow, const char* arg1);



#endif // MAINWINDOW_H
