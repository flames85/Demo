// qt
#include <QtCore/QCoreApplication>
#include "eventdispatcher_epoll.h"

// this-pro
#include "myqtcpserver.h"


int main(int argc, char *argv[])
{
    EventDispatcherEPoll dispatcher;
    QCoreApplication a(argc, argv);

    MyQTcpServer *server = new MyQTcpServer();
    server->listen(QHostAddress::Any, 1234);
    QObject::connect(server, SIGNAL(newConnection()), server, SLOT(acceptConnection()));

    return a.exec();
}
