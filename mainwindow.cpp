#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
