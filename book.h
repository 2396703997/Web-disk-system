#ifndef BOOK_H
#define BOOK_H

#include <QWidget>
#include<QListWidget>
#include<QPushButton>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include"protocol.h"
#include<QTimer>


class Book : public QWidget
{
    Q_OBJECT
public:
    explicit Book(QWidget *parent = nullptr);
    void updateFileList(const PDU *pdu);
    void clearEnterDir();
    QString enterDir();
    void setDownloadStatus(bool status);
    bool getDownloadStatus();
    QString getSaveFilePath();

    QString getShareFileName();

    qint64 m_iTotal;//文件总的大小
    qint64 m_iRecved;//已经收到的大小




signals:

public slots:
    void createDir();//创建文件夹槽函数
    void flushFile();//查看所有文件槽函数
    void delDir();//删除文件夹槽函数
    void renameFile();//重命名文件夹槽函数
    void enterDir(const QModelIndex &index);
    void returnPre();
    void delRegFile();//删除常规文件槽函数
    void uploadFile();
    void uploadFileData();
    void downloadFile();

    void shareFile();

    void moveFile();
    void selectDestDir();






private:
    QListWidget *m_pBookListW;
    QPushButton *m_pReturnPB;
    QPushButton *m_pCreateDirPB;
    QPushButton *m_pDelDirPB;
    QPushButton *m_pRenamePB;
    QPushButton *m_pFlushFilePB;
    QPushButton *m_pUpLoadPB;
    QPushButton *m_pDownLoadPB;
    QPushButton *m_pDelFilePB;
    QPushButton *m_pShareFilePB;
    QPushButton *m_pMoveFilePB;
    QPushButton *m_pSelectDirPB;

    QString m_strEnterDir;
    QString m_strUploadFilePath;
    QTimer * m_pTimer;//定时器

    QString m_strSaveFilePath;

    bool m_bDownload;

    QString m_strShareFileName;

    QString m_strMoveFileName;

    QString m_strMoveFilePath;
    QString m_strDestDir;



};

#endif // BOOK_H
