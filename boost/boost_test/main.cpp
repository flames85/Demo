// echo_server.cpp
// g++ -o echo_server -O3 echo_server.cpp -lboost_system -lboost_thread
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

using boost::asio::ip::tcp;

int total_conn = 0;

class Session : public boost::enable_shared_from_this<Session>
{
public:
    Session(boost::asio::io_service &io_service)
           : m_socket(io_service)
    {
    }

    tcp::socket& socket()
    {
        return m_socket;
    }

    void start()
    {
        m_socket.async_read_some(boost::asio::buffer(m_data, max_length),
                                 boost::bind(&Session::handle_read,
                                             shared_from_this(),
                                             boost::asio::placeholders::error,
                                             boost::asio::placeholders::bytes_transferred));
    }

private:
    void handle_read(const boost::system::error_code& error,
                     size_t bytes_transferred)
    {
        if (!error)
        {
            boost::asio::async_write(m_socket,
                                     boost::asio::buffer(m_data, bytes_transferred),
                                     boost::bind(&Session::handle_write,
                                                 shared_from_this(),
                                                 boost::asio::placeholders::error));
        }
    }

    void handle_write(const boost::system::error_code& error)
    {
        if (!error)
        {
            m_socket.async_read_some(boost::asio::buffer(m_data, max_length),
                                     boost::bind(&Session::handle_read,
                                                 shared_from_this(),
                                                 boost::asio::placeholders::error,
                                                 boost::asio::placeholders::bytes_transferred));
        }
    }

    enum
    {
        max_length = 1024
    };
    tcp::socket             m_socket;
    char                    m_data[max_length];
};

class Server
{
public:
    Server(boost::asio::io_service& io_service, short port)
        : m_ioService(io_service),
          m_acceptor(io_service, tcp::endpoint(tcp::v4(), port))
{
    start_accept();
}

private:
    void start_accept()
    {
        boost::shared_ptr<Session> (new Session(m_ioService));
        m_acceptor.async_accept(newSession->socket(),
                                boost::bind(&Server::handle_accept,
                                            this,
                                            newSession,
                                            boost::asio::placeholders::error));
    }

    void handle_accept(boost::shared_ptr<Session> newSession,
                       const boost::system::error_code& error)
    {
        start_accept();

        if (!error)
        {
            std::cout << "total connect =" << ++total_conn <<std::endl;
            newSession->start();
        }
    }

    boost::asio::io_service  &m_ioService;
    tcp::acceptor             m_acceptor;
};

int main(int argc, char* argv[])
{
    try
    {
        if (argc < 2)
        {
            std::cerr << "Usage: async_tcp_echo_server <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;

        using namespace std; // For atoi.
        Server svr(io_service, atoi(argv[1]));

        int thread_num = 6;
        if (argc > 2)
            thread_num = atoi(argv[2]);

        boost::thread_group threadGroup;
        for (int i = 0; i < thread_num; ++i)
        {
            boost::thread *thrd = new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service));
            threadGroup.add_thread(thrd);
        }

        threadGroup.join_all();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
