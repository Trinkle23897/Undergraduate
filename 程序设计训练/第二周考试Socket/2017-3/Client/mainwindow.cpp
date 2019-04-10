#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    port=911;
//    for(int i=0;i<5;++i)
//    {
//        getBar(i)->setValue(0);
//        download[i]=new Download(++port,this);
//        connect(download[i],SIGNAL(refreshFileName(QString)),getLabel(i),SLOT(setText(QString)));
//        connect(download[i],SIGNAL(refreshProgressBar(int)),getBar(i),SLOT(setValue(int)));
//        connect(download[i],&QThread::finished,[this,i](){this->getLabel(i)->setText("Complete!");});
//    }
//    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(sendRequest()));
}

void MainWindow::sendRequest()
{

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    QFileDialog*fileDialog=new QFileDialog(this);
    fileDialog->setWindowTitle(tr("Open Text"));
    fileDialog->setDirectory("../");
    if(fileDialog->exec() == QDialog::Accepted) {
        path = fileDialog->selectedFiles()[0];
        qDebug()<<path;
    }
}

void MainWindow::on_pushButton_clicked()
{
    QFileDialog*fileDialog=new QFileDialog(this);
    fileDialog->setWindowTitle(tr("Open Text"));
    fileDialog->setDirectory("../");
    if(fileDialog->exec() == QDialog::Accepted) {
        path = fileDialog->selectedFiles()[0];
        qDebug()<<path;
    }
    dw=new Download(++port,this);
    dw->avaliable=false;
    dw->filename=path;
    dw->start();
}
