#include <iostream>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

using namespace std;
using namespace boost::asio;

class Downloader
{
public:
    Downloader() : m_percent(0) {}
    void start()
    {
        m_thread.reset(new boost::thread(boost::bind(&Downloader::download, this)));
        // detach()之后，m_thread不再代表一个可执行线程，interrupt()这些都会无效
//            m_thread->detach();
    }
    void stop()
    {
        m_thread->interrupt();
    }
    int get_percent() { return m_percent; }

    void do_get_message(const std::string& msg)
    {
        std::cout << "do get message " << msg << std::endl;
    }

private:
    void download()
    {
        try
        {
            while (m_percent < 100)
            {
                // 这是个中断点
                boost::this_thread::interruption_point();
                ++m_percent; // 模拟下载过程，加到100算结束
                // 这里也是中断点
                boost::this_thread::sleep(boost::posix_time::seconds(1));
                cout << "Percent is " << m_percent << endl;
            }
        }
        catch (boost::thread_interrupted& /*e*/)
        {
            // 网上都说在中断点处会抛异常，但我在vs2010环境下没有捕获到
            // 以前linux下面倒是有捕获
            // 你最好还是加上try-catch吧
            int a = 0;
            ++a;
        }
    }
    boost::shared_ptr<boost::thread> m_thread; // 下载进程
    int m_percent; // 下载百分比
};


int main(int argc, char* argv[])
{
    //建立io服务
    io_service iosev;

    cout << "要开始下载文件吗？" << endl;
    char ch;
    if (cin >> ch && ch == 'y')
    {
        Downloader download;
        download.start();

        //异步消息
        iosev.post(boost::bind(&Downloader::do_get_message, &download, "Hello1"));
        iosev.run(); // run函数会立即退出， 因为io_service::post消息投递完以后已经没有别的事件可能需要执行

        cout << "已经开始下载" << endl;
        cout << "要停止吗？" << endl;
        if (cin >> ch && ch == 'y')
        {
            download.stop();
        }
        cout << "已经下载了%" << download.get_percent() << endl;

        sleep(10000);
    }
    return 0;
}
