#ifndef FRIEND_H
#define FRIEND_H

#include <QWidget>
#include<QTextEdit>
#include<QListWidget>
#include<QLineEdit>
#include<QPushButton>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include"online.h"

class Friend : public QWidget
{
    Q_OBJECT
public:
    explicit Friend(QWidget *parent = 0);
    void showAllOnlineUsr(PDU *pdu);

    void updateFriendList(PDU *pdu);//刷新好友列表
    void updateGroupMsg(PDU *pdu);

    QString m_strSearchName;//临时搜索名字变量

    QListWidget *getFriendList();

signals:
public slots:
    void showOnline();//显示在线用户信号槽函数
    void searchUsr();//搜索信号槽函数
    void flushFriend();//刷新好友列表槽函数
    void delFriend();//删除好友槽函数
    void privateChat();//私聊槽函数
    void groupChat();//群聊槽函数

private:
    QTextEdit *m_pShowMsgTE;//显示消息
    QListWidget *m_pFriendListWidget;//好友列表
    QLineEdit *m_pInputMsgLE;//输入框

    QPushButton *m_pDelFriendPB;//删除好友
    QPushButton *m_pFlushFriendPB;//刷新好友
    QPushButton *m_pShowOnlineUsrPB;//显示在线用户按钮
    QPushButton *m_pSearchUsrPB;//搜索用户按钮
    QPushButton *m_pMsgSendPB;//发送消息按钮
    QPushButton *m_pPrivateChatPB;//私聊按钮

    Online *m_pOnline;

};

#endif // FRIEND_H
