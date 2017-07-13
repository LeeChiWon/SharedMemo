#include "configurationdialog.h"
#include "ui_configurationdialog.h"

ConfigurationDialog::ConfigurationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigurationDialog)
{
    ui->setupUi(this);
}

ConfigurationDialog::~ConfigurationDialog()
{
    delete ui;
}

void ConfigurationDialog::on_pushButton_DBPath_clicked()
{

}

void ConfigurationDialog::on_pushButton_Apply_clicked()
{

}

void ConfigurationDialog::on_pushButton_Cancel_clicked()
{

}
