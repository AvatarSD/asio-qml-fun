#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <functional>
#include <list>
#include <memory>
#include <boost/asio.hpp>
#include <boost/regex.hpp>

struct Client
{
	Client(boost::asio::io_service * servise) : sock(*servise) {}
	boost::asio::ip::tcp::socket sock;
	boost::asio::streambuf buff;
};

class ServerTcp
{
public:
	ServerTcp(boost::asio::io_service&, boost::asio::ip::tcp::endpoint);
	~ServerTcp();

	void setReadEventHandler(std::function<void(std::shared_ptr<Client>, std::size_t)>);
	void setConnectedEventHandler(std::function<void(std::shared_ptr<Client>)>);
	void setDisconnectedEventHandler(std::function<void(std::shared_ptr<Client>, const boost::system::error_code&)>);

	void start();
	void stop();

	void setMaxClients(std::size_t);
	void setReadUntilCondition(const boost::regex &x);

private:
	std::function<void(std::shared_ptr<Client>, std::size_t)> read;
	std::function<void(std::shared_ptr<Client>)> connected;
	std::function<void(std::shared_ptr<Client>, const boost::system::error_code&)> disconnected;

	boost::asio::io_service * _ioservise;
	boost::asio::ip::tcp::acceptor acceptor;
	std::list<std::shared_ptr<Client> > clients;
	std::size_t maxClients;
	boost::regex cond;

	void onAccept (std::shared_ptr<Client>, boost::system::error_code err);
	void onRead (std::shared_ptr<Client>, boost::system::error_code err, std::size_t size);
};

#endif // TCPSERVER_H
