#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    ui->setupUi(this);
    connect(this,SIGNAL(LanguageChanged()),&ConfigurationDlg,SLOT(LanguageChanged()));
    connect(ui->tableWidget->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(sectionClicked(int)));
    connect(qApp,SIGNAL(commitDataRequest(QSessionManager&)),this,SLOT(commitDataRequest(QSessionManager&)),Qt::DirectConnection);

    LanguageChange();
    ui->dateEdit_StartDate->setDate(QDate::currentDate());
    ui->dateEdit_EndDate->setDate(QDate::currentDate());
    TrayIconInit();
    ComboBoxInit();
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(TableWidgetMenuShow(QPoint)));
    QList <QAction*> TableWidgetMenuActions;
    TableWidgetMenuActions.append(ui->actionEdit);
    TableWidgetMenuActions.append(ui->actionDelete);
    TableWidgetMenu=new QMenu();
    TableWidgetMenu->addActions(TableWidgetMenuActions);
    TableWidgetInit();
    DBInit();

    workThread=new WorkThread();
    workThread->FilePath=ConfigurationDlg.DBName();
    connect(workThread,SIGNAL(MemoUpdate()),this,SLOT(MemoUpdate()));
    workThread->start();

    Settings=new QSettings("SharedMemoOrg","SharedMemo",this);
    restoreGeometry(Settings->value("geometry/SharedMemoGeometry").toByteArray());
}

MainWindow::~MainWindow()
{
    if(workThread->isRunning())
    {
        workThread->quit();
    }

    delete ui;
}

void MainWindow::on_actionHelp_triggered()
{
    QString newfilename = QString("%1/help.pdf").arg(QApplication::applicationDirPath());
    QDesktopServices::openUrl(QUrl(newfilename.prepend( "file:///" )));
}

void MainWindow::on_actionConfiguration_triggered()
{
    ConfigurationDlg.exec();
}

void MainWindow::on_actionExit_triggered()
{
    this->hide();
    this->close();
}

void MainWindow::on_actionExcel_triggered()
{
    QFileDialog FileDialog;
    QString FileName=FileDialog.getSaveFileName(this,tr("Save File"),"c://",tr("Excel (*.xlsx)"));

    if(!FileName.isEmpty() || !FileName.isNull())
    {
        ExcelSave(FileName);
    }

}

void MainWindow::on_actionView_triggered()
{
    /*  QPrinter Printer;
    QPrintDialog PrinterDlg(&Printer);

    if(PrinterDlg.exec()==QDialog::Accepted)
    {
        QPainter Painter(&Printer);
        ui->tableWidget->render(&Painter);
    }*/

    QPrintPreviewDialog dialog;
    connect(&dialog, SIGNAL(paintRequested(QPrinter*)), this, SLOT(print(QPrinter*)));
    dialog.exec();
}

void MainWindow::DBInit()
{
    QSqlDatabase DB=QSqlDatabase::addDatabase("QSQLITE","MainDB");
    DB.setDatabaseName(ConfigurationDlg.DBName());

    try
    {
        if(!DB.open())
        {
            QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database open error!"),DB.lastError().text()),QMessageBox::Ok);
            QSqlDatabase::removeDatabase("MainDB");
            return;
        }

        ui->tableWidget->clear();
        ui->tableWidget->setRowCount(0);
        TableWidgetInit();

        // CREATE TABLE "main_tb" ( `idx` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, `date` TEXT, `content` TEXT, `separation` TEXT, `writer` TEXT )

        DB.transaction();
        QSqlQuery query(DB);
        query.exec(QString("select * from main_tb order by idx desc limit %1").arg(ConfigurationDlg.ListCount()));

        while(query.next())
        {
            ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_IDX,new QTableWidgetItem(query.value("idx").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_DATE,new QTableWidgetItem(query.value("date").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_CONTENT,new QTableWidgetItem(query.value("content").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_SEPARATION,new QTableWidgetItem(query.value("separation").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_WRITER,new QTableWidgetItem(query.value("writer").toString()));
        }
        ui->tableWidget->resizeRowsToContents();
        ui->tableWidget->resizeColumnsToContents();

        DB.commit();
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
    QSqlDatabase DB=QSqlDatabase::database("MainDB");

    try
    {
        if(!DB.isOpen())
        {
            QSqlDatabase::removeDatabase("MainDB");
            DBInit();
        }

        ui->tableWidget->clear();
        ui->tableWidget->setRowCount(0);
        TableWidgetInit();

        DB.transaction();
        QSqlQuery query(DB);

        switch(Select)
        {
        case SEARCH_A:
            query.exec(QString("select * from main_tb order by idx desc limit %1").arg(ConfigurationDlg.ListCount()));
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
            QueryText.append(" order by idx desc");
            query.exec(QueryText);
            break;
        }

        while(query.next())
        {
            ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_IDX,new QTableWidgetItem(query.value("idx").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_DATE,new QTableWidgetItem(query.value("date").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_CONTENT,new QTableWidgetItem(query.value("content").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_SEPARATION,new QTableWidgetItem(query.value("separation").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_WRITER,new QTableWidgetItem(query.value("writer").toString()));
        }
        ui->tableWidget->resizeColumnsToContents();
        ui->tableWidget->resizeRowsToContents();
        DB.commit();
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
    //ui->tableWidget->setSortingEnabled(true);

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

    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
    //ui->tableWidget->setSortingEnabled(false);
}

void MainWindow::TableWidgetInit()
{
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<tr("idx")<<tr("Date")<<tr("Content")<<tr("Separation")<<tr("Writer"));
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
        ui->tableWidget->clear();
        ui->tableWidget->setRowCount(0);
        TableWidgetInit();
        DB.transaction();
        QSqlQuery query(DB);

        query.exec(QString("insert into main_tb(date,content,separation,writer) values('%1','%2','%3','%4')")
                   .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd"),ui->textEdit_Input_Content->toPlainText())
                   .arg(ui->comboBox_Input_Separation->currentText(),ConfigurationDlg.User()));

        query.exec(QString("select * from main_tb order by idx desc limit %1").arg(ConfigurationDlg.ListCount()));


        while(query.next())
        {
            ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_IDX,new QTableWidgetItem(query.value("idx").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_DATE,new QTableWidgetItem(query.value("date").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_CONTENT,new QTableWidgetItem(query.value("content").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_SEPARATION,new QTableWidgetItem(query.value("separation").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_WRITER,new QTableWidgetItem(query.value("writer").toString()));
        }
        ui->tableWidget->resizeColumnsToContents();
        ui->tableWidget->resizeRowsToContents();
        DB.commit();
        DB.close();
    }

    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("MainDB");
    }
}

void MainWindow::Delete()
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
        query.exec(QString("delete from main_tb where idx=%1").arg(ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text().toInt()));

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

void MainWindow::Backup()
{
    if(ConfigurationDlg.BackupCheck())
    {
        QString FilePath=ConfigurationDlg.DBName();
        FilePath.replace("SharedMemo.db","Backup");

        QDir Dir(FilePath);
        if(!Dir.exists())
        {
            Dir.mkdir(FilePath);
        }

        FilePath.append(QString("/%1.db").arg(QDate::currentDate().toString("yyyy-MM-dd")));
        if(QFile::copy(ConfigurationDlg.DBName(),FilePath))
        {
            //qDebug()<<"File backup save.";
        }
    }
}

void MainWindow::ExcelSave(QString FileName)
{
    QXlsx::Document xlsx(FileName);
    QStringList ColumnTitle=QStringList()<<tr("idx")<<tr("Date")<<tr("Content")<<tr("Separation")<<tr("Writer");
    int ColumnWidth[5]={6,10,80,10,10};
    QXlsx::Format format;

    format.setBorderStyle(QXlsx::Format::BorderThin);
    format.setFontSize(10);
    format.setFontBold(false);

    for(int i=0; i<ui->tableWidget->rowCount(); i++)
    {
        for(int j=0; j<5; j++)
        {
            xlsx.write(i+2,j+1,ui->tableWidget->item(i,j)->text(),format);
        }
    }

    format.setBottomBorderStyle(QXlsx::Format::BorderDouble);
    format.setFontBold(true);
    format.setFontSize(13);

    for(int i=1; i<6; i++)
    {
        xlsx.setColumnWidth(i,ColumnWidth[i-1]);
        xlsx.write(1,i,ColumnTitle.at(i-1),format);
    }

    if(xlsx.save())
    {
        QMessageBox::information(this,tr("Save"),tr("Excel File is Saved."),QMessageBox::Ok);
    }

    xlsx.deleteLater();
}

void MainWindow::TrayIconInit()
{
    TrayIcon=new QSystemTrayIcon(this);
    TrayIconMenu=new QMenu(this);
    QList<QAction*>ActionList;
    ActionList<<ui->actionView<<ui->actionExit;
    TrayIconMenu->addActions(ActionList);
    TrayIcon->setContextMenu(TrayIconMenu);
    QIcon Icon(":/Img/notes.png");
    TrayIcon->setIcon(Icon);
    TrayIcon->show();
    TrayIcon->setToolTip(tr("SharedMemo"));
    TrayIcon->showMessage(tr("SharedMemo"),tr("Executing..."),QSystemTrayIcon::Information,5000);
    connect(TrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(onSystemTryIconClicked(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::onSystemTryIconClicked(QSystemTrayIcon::ActivationReason reason){
    switch(reason)
    {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        this->setWindowState(Qt::WindowActive);
        this->show();
        break;
    }
}

void MainWindow::MemoUpdate()
{
    on_pushButton_AllSearch_clicked();
    QMessageBox::information(this,tr("Information"),tr("Memo is Update & Add"),QMessageBox::Ok);
}

void MainWindow::hideEvent(QHideEvent *event)
{
    if(isMinimized())
    {
        this->hide();
    }
    QWidget::hideEvent(event);
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

void MainWindow::commitDataRequest(QSessionManager &)
{
    Backup();
}

void MainWindow::print(QPrinter *printer)
{
    QPainter painter;
    if(!painter.begin(printer)) {
        QMessageBox::warning(this,tr("Warning"),tr("can't start printer"),QMessageBox::Ok);
        return;
    }
    // print table
    TablePrinter tablePrinter(&painter, printer);
    QVector<int> columnStretch = QVector<int>() << 2 << 4 << 20 << 3 << 3;
    if(!tablePrinter.printTable(ui->tableWidget->model(), columnStretch)) {
        QMessageBox::warning(this,tr("Error"),tablePrinter.lastError(),QMessageBox::Ok);
    }
    painter.end();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    if(TrayIcon->isVisible())
    {
        TrayIcon->hide();
    }
    Backup();

    Settings->setValue("geometry/SharedMemoGeometry",saveGeometry());
    event->accept();
}

void MainWindow::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    on_actionEdit_triggered(true);
}

void MainWindow::TableWidgetMenuShow(const QPoint &pos)
{
    if(ui->tableWidget->rowCount()>0)
    {
        TableWidgetMenu->exec(ui->tableWidget->viewport()->mapToGlobal(pos));
    }
}

void MainWindow::on_actionEdit_triggered(bool checked)
{
    TableEditDialog Dlg;
    connect(&Dlg,SIGNAL(Edit(QString,QString,QString)),this,SLOT(Edit(QString,QString,QString)));
    Dlg.Read(ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text(),ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text(),ui->tableWidget->item(ui->tableWidget->currentRow(),2)->text());
    Dlg.exec();
}

void MainWindow::on_actionDelete_triggered(bool checked)
{
    if(ui->tableWidget->currentRow()>-1)
    {
        if(QMessageBox::warning(this, tr("Delete"),tr("Delete?"),QMessageBox::Ok| QMessageBox::Cancel,QMessageBox::Ok)== QMessageBox::Ok)
        {
            Delete();
            ui->tableWidget->removeRow(ui->tableWidget->currentRow());
        }
    }
}

void MainWindow::Edit(QString Edit_idx, QString Edit_Date, QString Edit_Content)
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

        query.exec(QString("update main_tb set date='%1',content='%2' where idx=%3")
                   .arg(Edit_Date,Edit_Content).arg(Edit_idx.toInt()));

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
