#ifndef MYQTCPSERVER_H
#define MYQTCPSERVER_H

// qt
#include <QtNetwork>
#include <QSet>

class MyQTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyQTcpServer(QObject *parent = 0);

protected:
//    void incomingConnection (int socketDescriptor);
    
signals:
    
public slots:
    // new connection
    void acceptConnection();
    // recv
    void readClient();
    // peer close
    void peerClose();
    // checkout connection
    void checkoutConnection();

private:

    QSet<QTcpSocket*>   m_connectedClient;

    QTimer              *m_Timer;
};

#endif // MYQTCPSERVER_H
