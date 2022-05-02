#include "mainwindow.h"
#include "qclipboard.h"
#include "ui_mainwindow.h"
#include "clientlogic.h"
#include "serverlogic.h"

#include <QTextEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QMessageBox>
#include <QInputDialog>
#include <QStatusBar>

#include <QTcpSocket>
#include <QTcpServer>
//#include <QtNetwork/QHostAddress>
#include <QString>
//#include <QClipboard>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    setWindowTitle("Text Over Network!");
    setWindowIcon(QIcon(":/res/icons/m512.png"));
    ui->ipText->setPlaceholderText("Enter server IP:Port here..");
    ui->statusbar->setStyleSheet("color: #9e2601");
    ui->statusbar->showMessage("Not connected!");
    ui->copyButton->setEnabled(false);
    ServerLogic *sl = new ServerLogic();
    sl->sconnect();



}

//На проект затрачено: 9.5 ч.

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::on_clearButton_clicked()
{
    ui->textEdit->clear();
}


void MainWindow::on_clientRadio_clicked()
{
    ui->clientRadio->setChecked(true);
    checkRadio();
}


void MainWindow::on_serverRadio_clicked()
{
    ui->serverRadio->setChecked(true);
    checkRadio();
}

void MainWindow::checkRadio(){
    if (ui->clientRadio->isChecked()){
        ui->ipText->setPlaceholderText("Enter server IP:Port here..");
        ui->ipText->setEnabled(true);
        ui->ipText->clear();
        ui->connectButton->setEnabled(true);
        ui->submitButton->setEnabled(true);
        ui->copyButton->setEnabled(false);
        ui->copyButton->setText("Copy");
        copyCount = 1;
        ui->textEdit->clear();
        ui->textEdit->setEnabled(true);
        ui->textEdit->setPlaceholderText("Send text to server! Type something and click \"Submit!\"");


    }
    else if (ui->serverRadio->isChecked()){
        ui->ipText->setEnabled(false);
        ui->connectButton->setEnabled(false);
        ui->submitButton->setEnabled(false);
        ui->ipText->clear();
        ui->copyButton->setText("Copy");
        ui->copyButton->setEnabled(true);
        copyCount = 1;

        ui->ipText->setText(sl.getIp() + ":" + QString::number(sl.getPort()));
        ui->textEdit->setEnabled(false);
        ui->textEdit->setPlaceholderText("You will receive message here!");


    }
    else {
        ui->textEdit->setText("Error");
    }
}



void MainWindow::on_submitButton_clicked()
{

}





void MainWindow::on_actionAbout_triggered()
{
    QMessageBox about;
    about.setTextFormat(Qt::RichText);
    about.setWindowTitle("About ToN");
    about.setText("Simple app to send and receive text over TCP/IP connection!<br><br>by ákd.<br><a href='https://github.com/ollyhearn/'>My GitHub!</a><br><br>v0.0.7");
    about.exec();
}


void MainWindow::on_actionSet_port_triggered()
{
    QString curport = QString::number(sl.getPort());
    while(!sl.SetPort(QInputDialog::getText(this, "Set port", "You can pick any port you wish, uless it is a reserved port", QLineEdit::Normal, curport).toLong())){
        QMessageBox::warning(this, "Scary warning!", "You set the wrong port, use ports only in [1:65535]");
    }
    checkRadio();
    QMessageBox::information(this, "Congratulations!", "Port set succsess!");
}


void MainWindow::on_connectButton_clicked()
{
    ClientLogic *tempcli = new ClientLogic();
    if(tempcli->clientConnect(ui->ipText->toPlainText())){
        ui->statusbar->setStyleSheet("color: #298244");
        ui->statusbar->showMessage("Connected!");
    }
    else{
        ui->statusbar->setStyleSheet("color: #9e2601");
        ui->statusbar->showMessage("Wrong IP:Port!", 2000);
    }
    delete tempcli;
}


void MainWindow::on_copyButton_clicked()
{
    Clipboard->setText(ui->ipText->toPlainText());
    if(copyCount == 1){
        ui->copyButton->setText("Copied!");
        copyCount++;
    }
    else{
        if(copyCount < 10){
            ui->copyButton->setText("Copied x" + QString::number(copyCount) + "!");
        }
        else{
            ui->copyButton->setText(":(");
            QMessageBox::information(this, "you shit", "stop doing this");
        }
        copyCount++;
    }
}

