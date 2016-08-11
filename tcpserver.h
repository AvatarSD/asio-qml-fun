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

class TcpServer
{
public:

typedef std::shared_ptr<Client> pClient;

	TcpServer(boost::asio::io_service&, boost::asio::ip::tcp::endpoint);
	~TcpServer();

	void setReadEventHandler(std::function<void(pClient, std::size_t)>);
	void setConnectedEventHandler(std::function<void(pClient)>);
	void setDisconnectedEventHandler(std::function<void(pClient, const boost::system::error_code&)>);

	void start();
	void stop();

	void setMaxClients(std::size_t);
	void setReadUntilCondition(const boost::regex &x);

private:
	std::function<void(pClient, std::size_t)> read;
	std::function<void(pClient)> connected;
	std::function<void(pClient, const boost::system::error_code&)> disconnected;

	boost::asio::io_service * _ioservise;
	boost::asio::ip::tcp::acceptor acceptor;
	std::list<pClient > clients;
	std::size_t maxClients;
	boost::regex cond;

	void onAccept (pClient, boost::system::error_code err);
	void onRead (pClient, boost::system::error_code err, std::size_t size);
};

typedef TcpServer::pClient pClient;


#endif // TCPSERVER_H
