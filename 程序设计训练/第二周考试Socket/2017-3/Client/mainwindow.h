#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include "download.h"
#include <QLabel>
#include <QProgressBar>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    int port;
    QString path;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Download *dw;
private:
    Ui::MainWindow *ui;
public slots:
    void sendRequest();
private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
