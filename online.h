#ifndef ONLINE_H
#define ONLINE_H

#include <QDialog>
#include<QWidget>
#include"protocol.h"

namespace Ui {
class Online;
}

class Online : public QDialog
{
    Q_OBJECT

public:
    explicit Online(QWidget *parent = nullptr);
    ~Online();
    void showUsr(PDU *pdu);

private slots:
    void on_addFriend_pd_clicked();

private:
    Ui::Online *ui;
};

#endif // ONLINE_H
