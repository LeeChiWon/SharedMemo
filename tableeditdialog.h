#ifndef TABLEEDITDIALOG_H
#define TABLEEDITDIALOG_H

#include <QDialog>

namespace Ui {
class TableEditDialog;
}

class TableEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TableEditDialog(QWidget *parent = 0);
    ~TableEditDialog();
    void Read(QString,QString,QString);

    QString idx;
signals:
    void Edit(QString,QString,QString);

private slots:
    void on_pushButton_Edit_clicked();
    void on_pushButton_Cancel_clicked();

private:
    Ui::TableEditDialog *ui;
};

#endif // TABLEEDITDIALOG_H
