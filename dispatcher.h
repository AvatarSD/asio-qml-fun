#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <boost/regex.hpp>
#include <boost/asio.hpp>
#include <tcpserver.h>

class Dispatcher
{
public:

	typedef boost::asio::buffers_iterator<boost::asio::streambuf::const_buffers_type> streambuf_iterator;

	Dispatcher(pClient clt, const boost::regex & reg);
	~Dispatcher();

	void operator ()(); //dispatch parse arguments

protected:
	virtual void dispatch(boost::match_results<streambuf_iterator> res) = 0;

	pClient client;
	boost::regex exp;
};

#endif // DISPATCHER_H
