#include "configurationdialog.h"
#include "ui_configurationdialog.h"

ConfigurationDialog::ConfigurationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigurationDialog)
{
    ui->setupUi(this);

    DBInit();
    ConfigurationLoad();
}

ConfigurationDialog::~ConfigurationDialog()
{
    delete ui;
}

QString ConfigurationDialog::DBName()
{
    return ui->lineEdit_DBPath->text();
}

int ConfigurationDialog::ListCount()
{
    return ui->lineEdit_ListOutpuCount->text().toInt();
}

QString ConfigurationDialog::User()
{
    return ui->lineEdit_User->text();
}

void ConfigurationDialog::on_pushButton_DBPath_clicked()
{
    ui->lineEdit_DBPath->setText(QFileDialog::getOpenFileName(this,tr("Open File"),"c://",tr("SQLITE (*.db)")));
}

void ConfigurationDialog::on_pushButton_Apply_clicked()
{
    ConfigurationSave();
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
    QSqlDatabase DB=QSqlDatabase::database("ConfigDB");

    try
    {
        if(!DB.isOpen())
        {
            QSqlDatabase::removeDatabase("ConfigDB");
            DBInit();
        }

        QSqlQuery query(DB);
        query.exec("select * from config_tb");
        query.next();

        ui->lineEdit_DBPath->setText(query.value("databasepath").toString());
        ui->lineEdit_User->setText(query.value("user").toString());
        ui->lineEdit_ListOutpuCount->setText(query.value("listcount").toString());
        ui->checkBox_Backup->setChecked(query.value("backup").toBool());
        DB.close();

      //  QMessageBox::information(this,tr("information"),tr("Configuration Load Success!"),QMessageBox::Ok);
    }

    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Config Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("ConfigDB");
    }
}

void ConfigurationDialog::ConfigurationSave()
{
    QSqlDatabase DB=QSqlDatabase::database("ConfigDB");

    try
    {
        if(!DB.isOpen())
        {
            QSqlDatabase::removeDatabase("ConfigDB");
            DBInit();
        }

        QSqlQuery query(DB);
        query.exec(QString("update config_tb set user='%1', databasepath='%2', listcount=%3, backup=%4")
                   .arg(ui->lineEdit_User->text(),ui->lineEdit_DBPath->text()).arg(ui->lineEdit_ListOutpuCount->text().toInt()).arg(ui->checkBox_Backup->isChecked()));
        query.next();
        DB.close();

        QMessageBox::information(this,tr("information"),tr("Configuration Saved!"),QMessageBox::Ok);
    }

    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Config Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("ConfigDB");
    }
}

void ConfigurationDialog::DBInit()
{
    QSqlDatabase DB=QSqlDatabase::addDatabase("QSQLITE","ConfigDB");
    DB.setDatabaseName(QApplication::applicationDirPath()+"/Config.db");

    try
    {
        if(!DB.open())
        {
            QSqlDatabase::removeDatabase("ConfigDB");
            return;
        }

        QSqlQuery query(DB);
        query.exec("create table if not exists config_tb(user text,databasepath text,listcount int, backup int)");
        query.exec(QString("insert into config_tb(user,databasepath,listcount) select 'test','test', 100, 0 where not exists(select * from config_tb)"));
        DB.close();
    }
    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Config Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("ConfigDB");
    }
}
