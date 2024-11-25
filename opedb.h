#ifndef OPEDB_H
#define OPEDB_H

#include <QObject>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QStringList>

class OpeDB : public QObject
{
    Q_OBJECT
public:
    explicit OpeDB(QObject *parent = nullptr);
    static OpeDB& getInstance();
    void init();
    ~OpeDB();

    bool handleRegist(const char *name,const char *pwd);
    bool handleLogin(const char *name,const char *pwd);
    void handleOffline(const char *name);
    QStringList handleAllOnline();//处理在线用户操作
    int handleSearchUsr(const char *name);//搜索用户操作
    int handleAddFriend(const char *pername,const char *name);//处理添加好友操作
    void handleAgreeAddFriend(const char *pername,const char *name);
    QStringList handleFlushFriend(const char *name);//处理刷新好友操作
    bool handleDelFriend(const char *name,const char *friendName);//处理删除好友操作



signals:

public slots:
private:
    QSqlDatabase m_db;  //连接数据库
};

#endif // OPEDB_H
