#include "tcpserver.h"
#include "ui_tcpserver.h"
#include "mytcpserver.h"
#include<QByteArray>
#include<QDebug>
#include<QMessageBox>
#include<QHostAddress>
#include <QFile>
TcpServer::TcpServer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TcpServer)
{
    ui->setupUi(this);
    loadConfig();
    MyTcpServer::getInstance().listen(QHostAddress(m_strIP),m_usPort);//监听
}

TcpServer::~TcpServer()
{
    delete ui;
}

void TcpServer::loadConfig()
{
    QFile file(":/server.config");
    if(file.open(QIODevice::ReadOnly)){
        QByteArray baData = file.readAll();
        QString strData=baData.toStdString().c_str();
        // qDebug()<<strData;//打印数据
        file.close();
        //分割字符串
        strData.replace("\r\n"," ");
        // qDebug()<<strData;
        QStringList strList = strData.split(" ");
        // for(int i=0;i<strList.size();i++){

        //     qDebug()<<"--->"<<strList[i];
        // }
        m_strIP = strList.at(0);
        m_usPort = strList.at(1).toUShort();
        qDebug()<<"ip:"<<m_strIP;
        qDebug()<<"port:"<<m_usPort;
    }
    else{
        QMessageBox::critical(this,"open config","open config failed");
    }
}
