#ifndef SHAREFILE_H
#define SHAREFILE_H

#include <QWidget>
#include<QPushButton>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QButtonGroup>
#include<QScrollArea>
#include<QCheckBox>
#include<QListWidget>
class ShareFile : public QWidget
{
    Q_OBJECT
public:
    explicit ShareFile(QWidget *parent = nullptr);

    static ShareFile &getInstance();

    void test();
    void updateFriend(QListWidget *pFriendList);

signals:

public slots:
    void cancelSelect();//取消选择槽函数
    void selectAll();//全选槽函数
    void okShare();//确定槽函数
    void cancelShare();//取消槽函数


private:
    QPushButton *m_pSelectAllPB;//全选按钮
    QPushButton *m_pCancelSelectPB;//取消全选按钮

    QPushButton *m_pOKPB;//确定按钮
    QPushButton *m_pCancelPB;//取消按钮

    QScrollArea *m_pSA;//显示好友的展示区域
    QWidget *m_pFriendW;//展示区域

    QVBoxLayout *m_pFriendWVBL;
    QButtonGroup *m_pButtonGroup;
};

#endif // SHAREFILE_H
