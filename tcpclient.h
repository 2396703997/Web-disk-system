#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include<QFile>
#include<QTcpSocket>//连接服务器和收发服务器数据
#include"protocol.h"
#include"opewidget.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class TcpClient;
}
QT_END_NAMESPACE

class TcpClient : public QWidget
{
    Q_OBJECT

public:
    TcpClient(QWidget *parent = nullptr);
    ~TcpClient();
    void loadConfig();
    static TcpClient &getInstance();
    QTcpSocket &getTcpSocaket();
    QString loginName();
    QString curPath();
    void setCurPath(QString strCurPath);



public slots:
    void showConnect();
    void recvMsg();



private slots:
    // void on_send_pd_clicked();

    void on_login_pb_clicked();

    void on_regist_pb_clicked();

    void on_cancel_pb_clicked();

private:
    Ui::TcpClient *ui;
    QString m_strIP;
    quint16 m_usPort;
    QTcpSocket m_tcpSocket;//连接服务器，和服务器进行数据交互
    QString m_strLoginName;//客户端A的用户名
    QString m_strCurPath;
    QFile m_file;
};
#endif // TCPCLIENT_H
