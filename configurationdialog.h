#ifndef CONFIGURATIONDIALOG_H
#define CONFIGURATIONDIALOG_H

#include <QDialog>
#include <QtWidgets>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
class ConfigurationDialog;
}

class ConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigurationDialog(QWidget *parent = 0);
    ~ConfigurationDialog();
    QString DBName();
    int ListCount();
    QString User();
    bool BackupCheck();

private slots:
    void on_pushButton_DBPath_clicked();
    void on_pushButton_Apply_clicked();
    void on_pushButton_Cancel_clicked();
    void LanguageChanged();


    void on_checkBox_StartApplication_clicked(bool checked);

private:
    Ui::ConfigurationDialog *ui;
    void ConfigurationLoad();
    void ConfigurationSave();
    void DBInit();
};

#endif // CONFIGURATIONDIALOG_H
