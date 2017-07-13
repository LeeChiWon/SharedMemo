#ifndef CONFIGURATIONDIALOG_H
#define CONFIGURATIONDIALOG_H

#include <QDialog>
#include <QtWidgets>

namespace Ui {
class ConfigurationDialog;
}

class ConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigurationDialog(QWidget *parent = 0);
    ~ConfigurationDialog();


private slots:
    void on_pushButton_DBPath_clicked();
    void on_pushButton_Apply_clicked();
    void on_pushButton_Cancel_clicked();
    void LanguageChanged();

private:
    Ui::ConfigurationDialog *ui;
    void ConfigurationLoad();
};

#endif // CONFIGURATIONDIALOG_H
