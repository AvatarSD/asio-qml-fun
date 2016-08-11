#include "tcpserver.h"
#include <algorithm>


TcpServer::TcpServer(boost::asio::io_service& service, boost::asio::ip::tcp::endpoint ep) :
	_ioservise(&service), acceptor(service, ep)
{
	maxClients = 1;
	read =         [](pClient, std::size_t                      ){};
	connected =    [](pClient                                   ){};
	disconnected = [](pClient, const boost::system::error_code& ){};

	cond.set_expression("\\r|\\n|\\0");

	clients.push_back(std::make_shared<Client>(_ioservise));
}

TcpServer::~TcpServer()
{

}

void TcpServer::onRead (pClient client, boost::system::error_code err, std::size_t size)
{
	if(err)
	{
		if(disconnected)
			disconnected(client, err);
		client->sock.close();
		if(clients.size() == 1)
			acceptor.async_accept(client->sock, std::bind(&TcpServer::onAccept, this, client, std::placeholders::_1));
		else
			clients.remove(client);
		return;
	}

	if(read)
		read(client, size);

	boost::asio::async_read_until(client->sock, client->buff, cond, std::bind(&TcpServer::onRead, this, client, std::placeholders::_1, std::placeholders::_2));
}

void TcpServer::onAccept (pClient client, boost::system::error_code err)
{
	if(err)
		throw err;

	if(connected)
		connected(client);

	if(clients.size() < maxClients)
	{
		pClient newClient(new Client(_ioservise));
		clients.push_back(newClient);
		acceptor.async_accept(newClient->sock, std::bind(&TcpServer::onAccept, this, newClient, std::placeholders::_1));

	}

	boost::asio::async_read_until(client->sock, client->buff, cond, std::bind(&TcpServer::onRead, this, client, std::placeholders::_1, std::placeholders::_2));
}

void TcpServer::start()
{
	stop();
	acceptor.async_accept(clients.front()->sock, std::bind(&TcpServer::onAccept, this, clients.front(), std::placeholders::_1));
}

void TcpServer::stop()
{
	std::remove_if(clients.begin(), clients.end(), [&](pClient &t)
	{
		t->sock.close();
		if(clients.size() > 1)
			return true;
		return false;
	});
}

void TcpServer::setMaxClients(std::size_t maxVal)
{
	maxClients = maxVal;
}

void TcpServer::setReadUntilCondition(const boost::regex& x)
{
	cond = x;
}

void TcpServer::setReadEventHandler(std::function<void(pClient, std::size_t)> _read)
{
	read = _read;
}

void TcpServer::setConnectedEventHandler(std::function<void(pClient)> _connected)
{
	connected = _connected;
}

void TcpServer::setDisconnectedEventHandler(std::function<void(pClient, const boost::system::error_code&)> _disconnected)
{
	disconnected = _disconnected;
}
