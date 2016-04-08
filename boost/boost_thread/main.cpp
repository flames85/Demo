#include <iostream>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

using namespace std;

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
    cout << "要开始下载文件吗？" << endl;
    char ch;
    if (cin >> ch && ch == 'y')
    {
        Downloader d;
        d.start();
        cout << "已经开始下载" << endl;
        cout << "要停止吗？" << endl;
        if (cin >> ch && ch == 'y')
        {
            d.stop();
        }
        cout << "已经下载了%" << d.get_percent() << endl;
        sleep(10000);
    }
    return 0;
}
