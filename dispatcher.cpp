#include "dispatcher.h"


Dispatcher::Dispatcher(pClient clt, const boost::regex & reg) : client(clt), exp(reg)
{

}

Dispatcher::~Dispatcher()
{

}

void Dispatcher::operator ()()
{
	if (!client->buff.size())
		return;

	boost::match_results<streambuf_iterator> res;
	while(boost::regex_search(boost::asio::buffers_begin(client->buff.data()), boost::asio::buffers_end(client->buff.data()), res, exp))
	{
		dispatch(res);
		client->buff.consume(std::distance(boost::asio::buffers_begin(client->buff.data()), res.suffix().first));
	};

}
