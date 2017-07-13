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
    ui->lineEdit_DBPath->setText(QFileDialog::getOpenFileName(this,tr("Open File"),"c://",tr("SQLITE (*.db)")));

}

void ConfigurationDialog::on_pushButton_Apply_clicked()
{

    this->close();
}

void ConfigurationDialog::on_pushButton_Cancel_clicked()
{
    this->close();
}

void ConfigurationDialog::LanguageChanged()
{
    ui->retranslateUi(this);
}

void ConfigurationDialog::ConfigurationLoad()
{

}
