#include "online.h"
#include "ui_online.h"
#include<QDebug>
#include"tcpclient.h"

Online::Online(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Online)
{
    ui->setupUi(this);
}

Online::~Online()
{
    delete ui;
}

void Online::showUsr(PDU *pdu)
{
    if(NULL==pdu){
        return;
    }
    //提取数据
    uint uiSize = pdu->uiMsgLen/32;//名字个数
    char caTmp[32];
    for(uint i=0;i<uiSize;i++){
        memcpy(caTmp,(char*)(pdu->caMsg)+i*32,32);
        ui->online_lw->addItem(caTmp);
    }
}

void Online::on_addFriend_pd_clicked()
{
    QListWidgetItem *pItem = ui->online_lw->currentItem();
    QString strPerUsrName = pItem->text();//客户端B的用户名
    QString strLoginName = TcpClient::getInstance().loginName();//客户端A的用户名
    PDU *pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_REQUEST;
    memcpy(pdu->caData,strPerUsrName.toStdString().c_str(),strPerUsrName.size());//caData中存放客户端B的用户名
    memcpy(pdu->caData+32,strLoginName.toStdString().c_str(),strLoginName.size());//caData+32位置开始，存放客户端A的用户名
    TcpClient::getInstance().getTcpSocaket().write((char*)pdu,pdu->uiPDULen);//发送数据到服务器
    free(pdu);
    pdu=NULL;
}












