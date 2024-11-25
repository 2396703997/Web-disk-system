#include "sharefile.h"
#include"tcpclient.h"
#include"opewidget.h"
ShareFile::ShareFile(QWidget *parent)
    : QWidget{parent}
{
    m_pSelectAllPB = new QPushButton("全选");//全选按钮
    m_pCancelSelectPB = new QPushButton("取消全选");//取消全选按钮

    m_pOKPB = new QPushButton("确定");//确定按钮
    m_pCancelPB = new QPushButton("取消");//取消按钮

    m_pSA = new QScrollArea;//显示好友的展示区域
    m_pFriendW = new QWidget;//展示区域
    m_pFriendWVBL = new QVBoxLayout(m_pFriendW);
    m_pButtonGroup = new QButtonGroup(m_pFriendW);//管理好友
    m_pButtonGroup->setExclusive(false);

    QHBoxLayout *pTopHBL = new QHBoxLayout;
    pTopHBL->addWidget(m_pSelectAllPB);
    pTopHBL->addWidget(m_pCancelSelectPB);
    pTopHBL->addStretch();

    QHBoxLayout *pDownHBL = new QHBoxLayout;
    pDownHBL->addWidget(m_pOKPB);
    pDownHBL->addWidget(m_pCancelPB);

    QVBoxLayout *pMainVBL = new QVBoxLayout;
    pMainVBL->addLayout(pTopHBL);
    pMainVBL->addWidget(m_pSA);
    pMainVBL->addLayout(pDownHBL);

    setLayout(pMainVBL);
    // test();
    //将信号槽函数与可视化界面关联起来
    connect(m_pCancelSelectPB,SIGNAL(clicked(bool)),this,SLOT(cancelSelect()));
    connect(m_pSelectAllPB,SIGNAL(clicked(bool)),this,SLOT(selectAll()));
    connect(m_pOKPB,SIGNAL(clicked(bool)),this,SLOT(okShare()));
    connect(m_pCancelPB,SIGNAL(clicked(bool)),this,SLOT(cancelShare()));
}

ShareFile &ShareFile::getInstance()
{
    static ShareFile instance;
    return instance;
}

void ShareFile::test()
{
    QVBoxLayout *p = new QVBoxLayout(m_pFriendW);
    QCheckBox *pCB = NULL;
    for(int i=0;i<15;i++){
        pCB = new QCheckBox("jack");
        p->addWidget(pCB);
        m_pButtonGroup->addButton(pCB);
    }
    m_pSA->setWidget(m_pFriendW);
}

void ShareFile::updateFriend(QListWidget *pFriendList)
{
    if(NULL == pFriendList){
        return;
    }
    QAbstractButton *tmp =NULL;
    QList<QAbstractButton*> preFriendList = m_pButtonGroup->buttons();
    for(int i=0;i<preFriendList.size();i++){
        tmp =preFriendList[i];
        m_pFriendWVBL->removeWidget(preFriendList[i]);
        m_pButtonGroup->removeButton(preFriendList[i]);
        preFriendList.removeOne(tmp);
        delete tmp;
        tmp = NULL;
    }
    QCheckBox *pCB = NULL;
    for(int i=0;i<pFriendList->count();i++){
        pCB = new QCheckBox(pFriendList->item(i)->text());
        m_pFriendWVBL->addWidget(pCB);
        m_pButtonGroup->addButton(pCB);
    }
    m_pSA->setWidget(m_pFriendW);

}

void ShareFile::cancelSelect()
{
    QList<QAbstractButton*> cbList = m_pButtonGroup->buttons();
    for(int i = 0;i < cbList.size();i++){
        if(cbList[i]->isChecked()){
            cbList[i]->setChecked(false);
        }
    }
}

void ShareFile::selectAll()
{
    QList<QAbstractButton*> cbList = m_pButtonGroup->buttons();
    for(int i = 0;i < cbList.size();i++){
        if(!cbList[i]->isChecked()){
            cbList[i]->setChecked(true);
        }
    }
}

void ShareFile::okShare()
{
    QString strName = TcpClient::getInstance().loginName();//获取分享者的姓名
    QString strCurPath = TcpClient::getInstance().curPath();//获取分享的文件的路径
    QString strShareFileName = OpeWidget::getInstance().getBook()->getShareFileName();//获取共享的文件名

    QString strPath = strCurPath+"/"+strShareFileName;//拼接成包含文件名的路径

    QList<QAbstractButton*> cbList = m_pButtonGroup->buttons();
    int num = 0;
    for(int i = 0;i < cbList.size();i++){
        if(cbList[i]->isChecked()){
            num++;//记录被勾选的好友个数
        }
    }

    PDU *pdu = mkPDU(32*num+strPath.size()+1);
    pdu->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_REQUEST;
    sprintf(pdu->caData,"%s %d",strName.toStdString().c_str(),num);
    int j = 0;
    for(int i = 0;i < cbList.size();i++){
        if(cbList[i]->isChecked()){
            memcpy((char*)(pdu->caMsg+j*32),cbList[i]->text().toStdString().c_str(),cbList[i]->text().size());
            j++;
        }

    }
    memcpy((char*)(pdu->caMsg)+num*32,strPath.toStdString().c_str(),32);
    //发送请求给服务器
    TcpClient::getInstance().getTcpSocaket().write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu = NULL;
}

void ShareFile::cancelShare()
{
    hide();
}
