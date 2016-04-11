#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/lexical_cast.hpp>

namespace
{
	// strand提供串行执行, 能够保证线程安全, 同时被post或dispatch的方法, 不会被并发的执行. 
	// io_service不能保证线程安全
	boost::asio::io_service m_service;
	boost::asio::strand m_strand(m_service);
	boost::mutex m_mutex;

	void print(int id)
	{
		// boost::mutex::scoped_lock lock(m_mutex);
		static int count = 0;
		std::cout << "id: " << boost::lexical_cast<std::string>(id) << std::endl;
		std::cout << "count: " << boost::lexical_cast<std::string>(++count) << std::endl;
	}

	void ioRun1()
	{
		while(true)
		{
			m_service.run();
		}
	}

	void ioRun2()
	{
		while(true)
		{
			m_service.run();
		}
	}

	void strand_print1()
	{
		printf("Enter print1\n");
		m_strand.dispatch(boost::bind(print, 1));
        printf("Exit print1\n");
	}

	void strand_print2()
	{
		printf("Enter print2\n");
		m_strand.post(boost::bind(print, 2));
         printf("Exit print2\n");
	}

	void strand_print3()
	{
		printf("Enter print3\n");              
		m_strand.post(boost::bind(print, 3));
         printf("Exit print3\n");
	}

	void strand_print4()
	{
		printf("Enter print4\n");
		m_strand.post(boost::bind(print, 4));
         printf("Exit print4\n");
	}

	// 将上面的m_strand换成m_service后，
	void service_print1()
	{
		printf("Enter print1\n");
		m_service.dispatch(boost::bind(print, 1));
         printf("Exit print1\n");
	}

	void service_print2()
	{
		printf("Enter print2\n");
		m_service.post(boost::bind(print, 2));
         printf("Exit print2\n");
	}

	void service_print3()
	{
		printf("Enter print3\n");              
		m_service.post(boost::bind(print, 3));
         printf("Exit print3\n");
	}

	void service_print4()
	{
		printf("Enter print4\n");
		m_service.post(boost::bind(print, 4));
         printf("Exit print4\n");
	}
}

void test_strand()
{
	boost::thread ios1(ioRun1);
	boost::thread ios2(ioRun2);

	boost::thread t1(strand_print1);
	boost::thread t2(strand_print2);
	boost::thread t3(strand_print3);
	boost::thread t4(strand_print4);

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	m_service.run();
}

void test_service()
{
	boost::thread ios1(ioRun1);
	boost::thread ios2(ioRun2);

	boost::thread t1(service_print1);
	boost::thread t2(service_print2);
	boost::thread t3(service_print3);
	boost::thread t4(service_print4);

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	m_service.run();
}

int main()
{
    test_service();
//	test_strand();
}
