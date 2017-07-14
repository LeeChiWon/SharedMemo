#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "configurationdialog.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "define.h"

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
    void DBInit();
    void on_pushButton_AllSearch_clicked();
    void on_pushButton_TermsSearch_clicked();
    void sectionClicked(int);
    void on_pushButton_Input_clicked();
    void on_tableWidget_itemChanged(QTableWidgetItem *item);

private:
    Ui::MainWindow *ui;
    QTranslator Trans;
    bool bSort;

    void LanguageChange();
    void ComboBoxInit();
    void Search(int);
    void TableWidgetInit();
    void Input();


};

#endif // MAINWINDOW_H
