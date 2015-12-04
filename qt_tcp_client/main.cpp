#include <QtCore/QCoreApplication>
#include "eventdispatcher_epoll.h"
#include "mytcpclient.h"

int main(int argc, char *argv[])
{
    EventDispatcherEPoll dispatcher_epoll;
    QCoreApplication a(argc, argv);

    MyStart start;

    start.Start();

    return a.exec();
}
