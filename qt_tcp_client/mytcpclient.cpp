#include <QTimer>
#include <QtNetwork>
#include <QDebug>
#include "mytcpclient.h"

MyTcpClient::MyTcpClient(qint32 nIndex, QObject *parent) :
    QTcpSocket(parent),
    m_nClientIndex(nIndex),
    m_timer(new QTimer(this))
{
    QObject::connect(this, SIGNAL(connected()), this, SLOT(TriggerWrite()));
    QObject::connect(this, SIGNAL(readyRead()), this, SLOT(TriggerRead()));
    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(TriggerWrite()));
}

void MyTcpClient::TriggerWrite()
{
    QString strData;
    strData = QString("Hello I am No.%1").arg(m_nClientIndex);
    this->write(strData.toLatin1());
    qDebug() << "send:" << strData;

    if(!m_timer->isActive())
        m_timer->start(2000);
}

void MyTcpClient::TriggerRead()
{
    QByteArray array;
    array = this->readAll();
    qDebug() << "recv:" << array;
}

/////////////////////////////////


MyStart::MyStart(QObject *parent):
    QObject(parent)
{
}


void MyStart::Start() {
    for(qint32 nIndex = 0; nIndex < 1024; nIndex++)
    {
        MyTcpClient *client = new MyTcpClient(nIndex);
        client->connectToHost(QHostAddress("127.0.0.1"), 1234);
    }
}

