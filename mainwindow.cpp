#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this,SIGNAL(LanguageChanged()),&ConfigurationDlg,SLOT(LanguageChanged()));
    connect(ui->tableWidget->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(sectionClicked(int)));
    LanguageChange();
    ui->dateEdit_StartDate->setDate(QDate::currentDate());
    ui->dateEdit_EndDate->setDate(QDate::currentDate());
    ComboBoxInit();
    TableWidgetInit();
    DBInit();
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

void MainWindow::DBInit()
{
    QSqlDatabase DB=QSqlDatabase::addDatabase("QSQLITE","MainDB");
    DB.setDatabaseName(ConfigurationDlg.DBName());
    ui->tableWidget->setRowCount(0);
    try
    {
        if(!DB.open())
        {
            QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database open error!"),DB.lastError().text()),QMessageBox::Ok);
            QSqlDatabase::removeDatabase("MainDB");
            return;
        }

        // CREATE TABLE "main_tb" ( `idx` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, `date` TEXT, `content` TEXT, `separation` TEXT, `writer` TEXT )

        QSqlQuery query(DB);
        query.exec(QString("select * from main_tb order by date desc limit %1").arg(ConfigurationDlg.ListCount()));

        while(query.next())
        {
            ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_DATE,new QTableWidgetItem(query.value("date").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_CONTENT,new QTableWidgetItem(query.value("content").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_SEPARATION,new QTableWidgetItem(query.value("separation").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_WRITER,new QTableWidgetItem(query.value("writer").toString()));
        }
        ui->tableWidget->resizeRowsToContents();
        ui->tableWidget->resizeColumnsToContents();
        DB.close();
    }
    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Config Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("ConfigDB");
    }
}

void MainWindow::LanguageChange()
{
    Trans.load(":/Lang/Lang_ko_KR.qm");
    QApplication::installTranslator(&Trans);
    ui->retranslateUi(this);
    emit LanguageChanged();
}

void MainWindow::ComboBoxInit()
{
    QStringList ComboTexts=QStringList()<<tr("ERP")<<tr("Drawing")<<tr("Program")<<tr("Etc")<<tr("All");
    ui->comboBox_Input_Separation->clear();
    ui->comboBox_Search_Separation->clear();

    ui->comboBox_Input_Separation->addItems(ComboTexts);
    ui->comboBox_Search_Separation->addItems(ComboTexts);
}

void MainWindow::Search(int Select)
{
    ui->tableWidget->setRowCount(0);

    QSqlDatabase DB=QSqlDatabase::database("MainDB");

    try
    {
        if(!DB.isOpen())
        {
            QSqlDatabase::removeDatabase("MainDB");
            DBInit();
        }

        QSqlQuery query(DB);

        switch(Select)
        {
        case SEARCH_A:
            query.exec(QString("select * from main_tb order by date desc limit %1").arg(ConfigurationDlg.ListCount()));
            break;
        case SEARCH_C:
            QString QueryText=QString("select * from main_tb where date between '%1' and '%2'")
                    .arg(ui->dateEdit_StartDate->date().toString("yyyy-MM-dd"),ui->dateEdit_EndDate->date().toString("yyyy-MM-dd"));

            switch(ui->comboBox_Search_Separation->currentIndex())
            {
            case SEPARATION_ERP:
            case SEPARATION_DRAWING:
            case SEPARATION_PROGRAM:
            case SEPARATION_ETC:
                QueryText.append(QString(" and separation='%1'").arg(ui->comboBox_Search_Separation->currentText()));
                break;
            }

            if(ui->lineEdit_Search_Content->text().isEmpty() ||ui->lineEdit_Search_Content->text().isNull() || ui->lineEdit_Search_Content->text()=="")
            {

            }
            else
            {
                QueryText.append(QString(" and content like '\%%1\%'").arg(ui->lineEdit_Search_Content->text()));
            }
            query.exec(QueryText);
            break;
        }

        while(query.next())
        {
            ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_DATE,new QTableWidgetItem(query.value("date").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_CONTENT,new QTableWidgetItem(query.value("content").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_SEPARATION,new QTableWidgetItem(query.value("separation").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_WRITER,new QTableWidgetItem(query.value("writer").toString()));
        }
        ui->tableWidget->resizeColumnsToContents();
        ui->tableWidget->resizeRowsToContents();
        DB.close();
    }

    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("MainDB");
    }
}

void MainWindow::on_pushButton_AllSearch_clicked()
{
    Search(SEARCH_A);
}

void MainWindow::on_pushButton_TermsSearch_clicked()
{
    Search(SEARCH_C);
}

void MainWindow::sectionClicked(int column)
{
    ui->tableWidget->setSortingEnabled(true);

    if(!bSort) //ASC
    {
        ui->tableWidget->sortByColumn(column,Qt::AscendingOrder);
        bSort=true;
    }

    else //DESC
    {
        ui->tableWidget->sortByColumn(column,Qt::DescendingOrder);
        bSort=false;
    }
}

void MainWindow::TableWidgetInit()
{
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<tr("Date")<<tr("Content")<<tr("Separation")<<tr("Writer"));
}

void MainWindow::Input()
{
    QSqlDatabase DB=QSqlDatabase::database("MainDB");

    try
    {
        if(!DB.isOpen())
        {
            QSqlDatabase::removeDatabase("MainDB");
            DBInit();
        }

        QSqlQuery query(DB);

        query.exec(QString("insert into main_tb(date,content,separation,writer) values('%1','%2','%3','%4')")
                   .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd"),ui->textEdit_Input_Content->toPlainText())
                   .arg(ui->comboBox_Input_Separation->currentText(),ConfigurationDlg.User()));

        query.exec(QString("select * from main_tb order by date desc limit %1").arg(ConfigurationDlg.ListCount()));
        ui->tableWidget->setRowCount(0);

        while(query.next())
        {
            ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_DATE,new QTableWidgetItem(query.value("date").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_CONTENT,new QTableWidgetItem(query.value("content").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_SEPARATION,new QTableWidgetItem(query.value("separation").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_WRITER,new QTableWidgetItem(query.value("writer").toString()));
        }
        ui->tableWidget->resizeColumnsToContents();
        ui->tableWidget->resizeRowsToContents();
        DB.close();
    }

    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("MainDB");
    }
}

void MainWindow::on_pushButton_Input_clicked()
{
    if(ui->textEdit_Input_Content->toPlainText().isEmpty() ||ui->textEdit_Input_Content->toPlainText().isNull())
    {
        QMessageBox::warning(this,tr("Warning"),tr("Content is NULL."),QMessageBox::Ok);
        return;
    }

    Input();
    ui->textEdit_Input_Content->clear();
    ui->comboBox_Input_Separation->setCurrentIndex(4);
}

void MainWindow::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    QFont Font;
    Font.setBold(true);
    Font.setPointSize(10);

    switch(item->column())
    {
    case COLUMN_CONTENT:
        item->setFont(Font);
        break;
    }
}
