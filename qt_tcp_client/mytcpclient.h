#ifndef MYTCPCLIENT_H
#define MYTCPCLIENT_H

#include <QTcpSocket>

class QTimer;

class MyTcpClient : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpClient(qint32 nIndex, QObject *parent = 0);

public slots:
    void TriggerWrite();
    void TriggerRead();

private:
    qint32 m_nClientIndex;
    QTimer *m_timer;
};


class MyStart : public QObject
{
    Q_OBJECT
public:
    MyStart(QObject *parent = 0);

    void Start();

};

#endif // MYTCPCLIENT_H
