#include "friend.h"
#include"protocol.h"
#include"tcpclient.h"
#include<QInputDialog>
#include<QDebug>
#include"privatechat.h"
#include<QMessageBox>

Friend::Friend(QWidget *parent)
    : QWidget{parent}
{
    m_pShowMsgTE = new QTextEdit;
    m_pFriendListWidget = new QListWidget;
    m_pInputMsgLE = new QLineEdit;

    m_pDelFriendPB = new QPushButton("删除好友");
    m_pFlushFriendPB = new QPushButton("刷新好友");
    m_pShowOnlineUsrPB = new QPushButton("显示在线用户");
    m_pSearchUsrPB = new QPushButton("搜索好友");

    m_pMsgSendPB= new QPushButton("信息发送");//发送消息按钮
    m_pPrivateChatPB = new QPushButton("私聊");//私聊按钮
    QVBoxLayout *pRightPBVBL = new QVBoxLayout;
    pRightPBVBL->addWidget(m_pDelFriendPB);
    pRightPBVBL->addWidget(m_pFlushFriendPB);
    pRightPBVBL->addWidget(m_pShowOnlineUsrPB);
    pRightPBVBL->addWidget(m_pSearchUsrPB);
    pRightPBVBL->addWidget(m_pPrivateChatPB);

    QHBoxLayout *TopHBL = new QHBoxLayout;
    TopHBL->addWidget(m_pShowMsgTE);
    TopHBL->addWidget(m_pFriendListWidget);
    TopHBL->addLayout(pRightPBVBL);
    QHBoxLayout *pMsgHBL = new QHBoxLayout;
    pMsgHBL->addWidget(m_pInputMsgLE);
    pMsgHBL->addWidget(m_pMsgSendPB);

    m_pOnline = new Online;

    QVBoxLayout *pMain = new QVBoxLayout;
    pMain->addLayout(TopHBL);
    pMain->addLayout(pMsgHBL);
    pMain->addWidget(m_pOnline);

    m_pOnline->hide();

    setLayout(pMain);

    connect(m_pShowOnlineUsrPB,SIGNAL(clicked(bool)),this,SLOT(showOnline()));
    connect(m_pSearchUsrPB,SIGNAL(clicked(bool)),this,SLOT(searchUsr()));//将可视化界面中的按钮与槽函数关联
    connect(m_pFlushFriendPB,SIGNAL(clicked(bool)),this,SLOT(flushFriend()));//将刷新好友的按钮与槽函数关联起来
    connect(m_pDelFriendPB,SIGNAL(clicked(bool)),this,SLOT(delFriend()));//将删除好友的按钮与槽函数关联起来
    connect(m_pPrivateChatPB,SIGNAL(clicked(bool)),this,SLOT(privateChat()));//将私聊的按钮与槽函数关联起来
    connect(m_pMsgSendPB,SIGNAL(clicked(bool)),this,SLOT(groupChat()));   //将群聊的按钮与槽函数关联起来
}

void Friend::showAllOnlineUsr(PDU *pdu)
{
    if(NULL==pdu){
        return;
    }
    m_pOnline->showUsr(pdu);
}

void Friend::updateFriendList(PDU *pdu)
{
    if(NULL == pdu){
        return;
    }
    uint uiSize = pdu->uiMsgLen/32;
    char caName[32] ={'\0'};
    for(uint i=0;i<uiSize;i++){
        memcpy(caName,(char*)(pdu->caMsg)+i*32,32);
        m_pFriendListWidget->addItem(caName);
    }
}

void Friend::updateGroupMsg(PDU *pdu)
{
    QString strMsg = QString("%1 says : %2").arg(pdu->caData).arg((char*)(pdu->caMsg));
    m_pShowMsgTE->append(strMsg);

}

QListWidget *Friend::getFriendList()
{
    return m_pFriendListWidget;
}
//显示在线用户槽函数
void Friend::showOnline()
{

    if(m_pOnline->isHidden()){
        m_pOnline->show();
        PDU *pdu = mkPDU(0);//产生协议数据单元
        pdu->uiMsgType = ENUM_MSG_TYPE_ALL_ONLINE_REQUEST;
        TcpClient::getInstance().getTcpSocaket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu=NULL;
    }else{
        m_pOnline->hide();
    }
}
//搜索用户槽函数
void Friend::searchUsr()
{
    m_strSearchName = QInputDialog::getText(this,"搜索","用户名");
    if(!m_strSearchName.isEmpty()){
        qDebug()<<m_strSearchName;
        PDU *pdu = mkPDU(0);
        memcpy(pdu->caData,m_strSearchName.toStdString().c_str(),m_strSearchName.size());
        pdu->uiMsgType=ENUM_MSG_TYPE_SEARCH_USR_REQUEST;
        TcpClient::getInstance().getTcpSocaket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu=NULL;

    }
}

void Friend::flushFriend()
{
    m_pFriendListWidget->clear();
    QString strName = TcpClient::getInstance().loginName();
    PDU *pdu=mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST;
    memcpy(pdu->caData,strName.toStdString().c_str(),strName.size());
    TcpClient::getInstance().getTcpSocaket().write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu=NULL;
}

void Friend::delFriend()
{
    //异常处理
    if(NULL != m_pFriendListWidget->currentItem()){
        QString strFriendName = m_pFriendListWidget->currentItem()->text();//通过好友列表框获得当前框中选中的内容
        qDebug()<<strFriendName;
        PDU *pdu = mkPDU(0);
        pdu->uiMsgType=ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST;
        QString strSelfName = TcpClient::getInstance().loginName();
        memcpy(pdu->caData,strSelfName.toStdString().c_str(),strSelfName.size());//将客户端A的用户名存入pdu中
        memcpy(pdu->caData+32,strFriendName.toStdString().c_str(),strFriendName.size());//将客户端B的用户名存入pdu中
        TcpClient::getInstance().getTcpSocaket().write((char*)pdu->uiPDULen);//将pdu发送给服务器
        free(pdu);
        pdu=NULL;
    }
}

void Friend::privateChat()
{

    if(NULL != m_pFriendListWidget->currentItem()){
        QString strChatName = m_pFriendListWidget->currentItem()->text();//通过好友列表框获得当前框中选中的内容
        qDebug()<<strChatName;
        PrivateChat::getInstance().setChatName(strChatName);
        if(PrivateChat::getInstance().isHidden()){
            PrivateChat::getInstance().show();
        }
    }else{
        QMessageBox::warning(this,"私聊","请选择私聊的对象");
    }
}

void Friend::groupChat()
{

    QString strMsg = m_pInputMsgLE->text();
    m_pInputMsgLE->clear();
    if(!strMsg.isEmpty()){
        PDU *pdu = mkPDU(strMsg.size()+1);
        pdu->uiMsgType=ENUM_MSG_TYPE_GROUP_CHAT_REQUEST;
        QString strName = TcpClient::getInstance().loginName();
        strncpy(pdu->caData,strName.toStdString().c_str(),strName.size());
        strncpy((char*)(pdu->caMsg),strMsg.toStdString().c_str(),strMsg.size());
        TcpClient::getInstance().getTcpSocaket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu=NULL;
    }else{
        QMessageBox::warning(this,"群聊","信息不能为空");
    }
}
