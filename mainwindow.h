#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "configurationdialog.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

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
signals:
    void LanguageChanged();

private slots:
    void on_actionHelp_triggered();
    void on_actionConfiguration_triggered();
    void on_actionExit_triggered();
    void on_actionExcel_triggered();
    void on_actionView_triggered();
    void DBInit(QString);

private:
    Ui::MainWindow *ui;
    QTranslator Trans;

    void LanguageChange();
};

#endif // MAINWINDOW_H
