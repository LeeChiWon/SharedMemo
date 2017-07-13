#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this,SIGNAL(LanguageChanged()),&ConfigurationDlg,SLOT(LanguageChanged()));
    LanguageChange();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionHelp_triggered()
{

}

void MainWindow::on_actionConfiguration_triggered()
{
    ConfigurationDlg.exec();
}

void MainWindow::on_actionExit_triggered()
{

}

void MainWindow::on_actionExcel_triggered()
{

}

void MainWindow::on_actionView_triggered()
{

}

void MainWindow::DBInit(QString AddDatabase)
{
    QSqlDatabase MainDB=QSqlDatabase::addDatabase("QSQLITE",AddDatabase);
    //QString DBName=QString(ConfigurationDlg.);
   // MainDB.setDatabaseName(DBName);

    try
    {
        if(!MainDB.open())
        {
            QSqlDatabase::removeDatabase("MainDB");
            return;
        }

        QSqlQuery query(MainDB);
        query.exec("create table if not exists worktime_tb(listnumber integer,name text,starttime text,endtime text)");
        query.exec(QString("insert into worktime_tb(listnumber,name,starttime,endtime) select 5,'lee','18#00','20#00' where not exists(select * from worktime_tb)"));
        qDebug()<<query.lastError();
        query.exec(QString("insert into worktime_tb(listnumber,name,starttime,endtime) values(1,'pack','16#00','21#00')"));
        qDebug()<<query.lastError();
        query.exec("select * from worktime_tb");
        while(query.next())
        {
            qDebug()<<query.value("listnumber").toString()<<query.value("name").toString()<<query.value("starttime").toString()<<query.value("endtime").toString();
        }

       // MainDB.close();

    }
    catch(QException &e)
    {

        QSqlDatabase::removeDatabase("MainDB");
    }
}

void MainWindow::LanguageChange()
{
    Trans.load(":/Lang/Lang_ko_KR.qm");
    QApplication::installTranslator(&Trans);
    ui->retranslateUi(this);
    emit LanguageChanged();
}
