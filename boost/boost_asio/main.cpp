#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>

using namespace boost::asio;
using boost::system::error_code;
using ip::tcp;

class Service
{
    public:
    //类的初始化创建：设置io_service, 设置1000端口
    Service(io_service &iosev)
        :m_iosev(iosev),m_acceptor(iosev, tcp::endpoint(tcp::v4(), 12345))
    {
    }

    //创建一个tcp的socket；且还是侦听
    void start()
    {
        // 开始等待连接（非阻塞）
        boost::shared_ptr<tcp::socket> psocket(new tcp::socket(m_iosev));

        // 触发的事件只有error_code参数，所以用boost::bind把socket绑定进去
        m_acceptor.async_accept(*psocket,
                                boost::bind(&Service::accept_handler, this, psocket, _1) );
    }

    // 有客户端连接时accept_handler触发
    void accept_handler(boost::shared_ptr<tcp::socket> psocket, error_code ec)
    {
        if(ec) return;

        // 继续等待连接
        start();

        // 显示远程IP
        std::cout << psocket->remote_endpoint().address() << std::endl;

        // 发送信息(非阻塞)
        boost::shared_ptr<std::string> pstr(new std::string("hello async world!"));
        psocket->async_write_some(buffer(*pstr),
                                  boost::bind(&Service::write_handler, this, pstr, _1, _2));
    }

    // 异步写操作完成后write_handler触发
    void write_handler(boost::shared_ptr<std::string> pstr, error_code ec, size_t bytes_transferred)
    {
        if(ec)
            std::cout<< "发送失败!" << std::endl;
        else
            std::cout<< *pstr << " 已发送" << std::endl;

        //异步消息
        m_iosev.post(boost::bind(&Service::do_get_message, this, "write_handler"));
    }

    void do_get_message(const std::string& msg)
    {
        std::cout << "do get message " << msg << std::endl;
    }

    private:
    io_service          &m_iosev;
    ip::tcp::acceptor   m_acceptor;
};


int main(int argc, char* argv[])
{
    //建立io服务
    io_service iosev;

    Service sev(iosev);

    //开始侦听socket的连接；和开始接收远程数据
    sev.start();

    //异步消息
    iosev.post(boost::bind(&Service::do_get_message, &sev, "Hello"));

    //开始执行回调函数, 只要io_service内无需要等待的执行的事务，run函数会退出，即所有事件循环结束
    //因为此时一致有m_acceptor和psocket在占用着io_service, 所以run函数一直不会退出。那么只要在这个期间，调用io_service::post都是可行的。
    iosev.run();

    return 0;
}
