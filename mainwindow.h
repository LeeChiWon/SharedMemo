#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "configurationdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    ConfigurationDialog ConfigurationDlg;

private slots:
    void on_actionHelp_triggered();
    void on_actionConfiguration_triggered();
    void on_actionExit_triggered();
    void on_actionExcel_triggered();
    void on_actionView_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
