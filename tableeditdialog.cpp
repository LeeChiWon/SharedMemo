#include "tableeditdialog.h"
#include "ui_tableeditdialog.h"

TableEditDialog::TableEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableEditDialog)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());
}

TableEditDialog::~TableEditDialog()
{
    delete ui;
}

void TableEditDialog::Read(QString Index,QString Date, QString Content)
{
    idx=Index;
    ui->dateEdit->setDate(QDate::fromString(Date,"yyyy-MM-dd"));
    ui->textEdit->setText(Content);
}

void TableEditDialog::on_pushButton_Edit_clicked()
{
    emit Edit(idx,ui->dateEdit->date().toString("yyyy-MM-dd"),ui->textEdit->toPlainText());
    this->close();
}

void TableEditDialog::on_pushButton_Cancel_clicked()
{
    this->close();
}
