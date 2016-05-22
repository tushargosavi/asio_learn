#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time_config.hpp>


using namespace boost;

class Printer {
  private:
    boost::asio::deadline_timer timer1;
    boost::asio::deadline_timer timer2;
    boost::asio::strand strand;

    int count;
  public:
     Printer(boost::asio::io_service & io)
    :timer1(io, boost::posix_time::seconds(1)),
	timer2(io, boost::posix_time::seconds(1)), strand(io), count(0) {
	timer1.async_wait(strand.
			  wrap(boost::bind(&Printer::print1, this)));
	timer2.async_wait(strand.
			  wrap(boost::bind(&Printer::print2, this)));
    } void print1() {
	if (count < 10) {
	    std::cout << "Timer 1 fired " << count << std::endl;
	    ++count;
	    timer1.expires_at(timer1.expires_at() +
			      boost::posix_time::seconds(1));
	    timer1.async_wait(strand.
			      wrap(boost::bind(&Printer::print1, this)));
	}
    }

    void print2() {
	if (count < 10) {
	    std::cout << "Timer 2 fired " << count << std::endl;
	    ++count;
	    timer2.expires_at(timer2.expires_at() +
			      boost::posix_time::seconds(1));
	    timer2.async_wait(strand.
			      wrap(boost::bind(&Printer::print2, this)));
	}
    }

    ~Printer() {
	std::cout << "Final count is " << count << std::endl;
    }
};

int main(int argc, char **argv)
{
    boost::asio::io_service io;
    Printer p(io);
    boost::thread t(boost::bind(&boost::asio::io_service::run, &io));
    io.run();
    t.join();

    return 0;
}
