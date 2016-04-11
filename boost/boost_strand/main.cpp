#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
using namespace std;
using namespace boost;
using namespace asio;

asio::io_service             m_service;
asio::io_service::strand     m_strand(m_service);
mutex                        m_mutex;

void print( int fatherID)
{
    static int count = 0;
    cout << "fatherID "<< fatherID <<" "<<endl;
    sleep(1);
    cout << "count " << count++ << endl;
}

void ioRun1()
{
    while(1) {
        m_service.run();
    }
}

void ioRun2()
{
    while(1) {
        m_service.run();
    }
}

void print1()
{
    m_strand.dispatch(bind(print,1));
}

void print2()
{
    m_strand.post(bind(print,2));
}

void print3()
{
    m_strand.post(bind(print,3));
}

int main(void)
{
    boost::thread io1(ioRun1);
    boost::thread io2(ioRun2);

    boost::thread t1(print1);
    boost::thread t2(print2);
    boost::thread t3(print3);
    cout << "start" << endl;

    io1.join();
    io2.join();

    t1.join();
    t2.join();
    t3.join();

    cout << "end" << endl;
    return 0;
}
