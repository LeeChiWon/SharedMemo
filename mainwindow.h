#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "configurationdialog.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "define.h"
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>
#include "tableprinter.h"
#include "QtXlsx"
#include "workthread.h"
#include "tableeditdialog.h"

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
    WorkThread *workThread;
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
    void commitDataRequest(QSessionManager&);
    void print(QPrinter*);
    void onSystemTryIconClicked(QSystemTrayIcon::ActivationReason);
    void MemoUpdate();
    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);
    void TableWidgetMenuShow(const QPoint &pos);
    void on_actionEdit_triggered(bool checked);
    void on_actionDelete_triggered(bool checked);
    void Edit(QString,QString,QString);

private:
    Ui::MainWindow *ui;
    QTranslator Trans;
    bool bSort;
    QSystemTrayIcon *TrayIcon;
    QMenu *TrayIconMenu;
    QSettings *Settings;
    QMenu *TableWidgetMenu;

    void closeEvent(QCloseEvent *event);
    void hideEvent(QHideEvent *event);
    void LanguageChange();
    void ComboBoxInit();
    void Search(int);
    void TableWidgetInit();
    void Input();
    void Delete();
    void Backup();
    void ExcelSave(QString);
    void TrayIconInit();
};

#endif // MAINWINDOW_H
