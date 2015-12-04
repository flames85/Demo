// qt
#include <QDebug>
#include <QTimer>
// self
#include "myqtcpserver.h"


//void MyQTcpServer::incomingConnection (int socketDescriptor)
//{
//   QTcpSocket *clientConnection = new QTcpSocket;
//   if (!clientConnection) {
//      qDebug("not enough memory to create new QTcpSocket");
//      return;
//   }

//   //! socket 描述符写入
//   if (!clientConnection->setSocketDescriptor(socketDescriptor))
//   {
//       qDebug("couldn't set socket descriptor");
//       clientConnection->deleteLater();
//       return;
//   }

//   connect(clientConnection, SIGNAL(readyRead()), this, SLOT(readClient()));
//   connect(clientConnection, SIGNAL(readChannelFinished()), this, SLOT(peerClose()));
//   m_connectedClient.insert(clientConnection);
//}

MyQTcpServer::MyQTcpServer(QObject *parent) :
    QTcpServer(parent)
{
    m_Timer = new QTimer();
    m_Timer->start(3000);
    connect(m_Timer, SIGNAL(timeout()), this, SLOT(checkoutConnection()));
}

void MyQTcpServer::acceptConnection()
{
    QTcpSocket *clientConnection = this->nextPendingConnection();

    connect(clientConnection, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(clientConnection, SIGNAL(readChannelFinished()), this, SLOT(peerClose()));

    m_connectedClient.insert(clientConnection);
}

void MyQTcpServer::readClient()
{
    QTcpSocket* _clientConnection = dynamic_cast<QTcpSocket*>(sender());
    QString str = _clientConnection->readAll();
    _clientConnection->write("ack");
}

void MyQTcpServer::peerClose()
{
    QTcpSocket* _clientConnection = dynamic_cast<QTcpSocket*>(sender());
    m_connectedClient.remove(_clientConnection);
    _clientConnection->deleteLater();
}

void MyQTcpServer::checkoutConnection()
{
    qDebug() << "connections:" << m_connectedClient.size() ;
}
