#include "tcpserver.h"
#include <algorithm>


ServerTcp::ServerTcp(boost::asio::io_service& service, boost::asio::ip::tcp::endpoint ep) :
	_ioservise(&service), acceptor(service, ep)
{
	maxClients = 1;
	read =         [](std::shared_ptr<Client>, std::size_t                      ){};
	connected =    [](std::shared_ptr<Client>                                   ){};
	disconnected = [](std::shared_ptr<Client>, const boost::system::error_code& ){};

	cond.set_expression("\r|\n");

	clients.push_back(std::make_shared<Client>(_ioservise));
}
ServerTcp::~ServerTcp()
{

}

void ServerTcp::onRead (std::shared_ptr<Client> client, boost::system::error_code err, std::size_t size)
{
	if(err)
	{
		if(disconnected)
			disconnected(client, err);
		client->sock.close();
		if(clients.size() == 1)
			acceptor.async_accept(client->sock, std::bind(&ServerTcp::onAccept, this, client, std::placeholders::_1));
		else
			clients.remove(client);
		return;
	}

	if(read)
		read(client, size);

	boost::asio::async_read_until(client->sock, client->buff, cond, std::bind(&ServerTcp::onRead, this, client, std::placeholders::_1, std::placeholders::_2));
}

void ServerTcp::onAccept (std::shared_ptr<Client> client, boost::system::error_code err)
{
	if(err)
		throw err;

	if(connected)
		connected(client);

	if(clients.size() < maxClients)
	{
		std::shared_ptr<Client> newClient(new Client(_ioservise));
		clients.push_back(newClient);
		acceptor.async_accept(newClient->sock, std::bind(&ServerTcp::onAccept, this, newClient, std::placeholders::_1));

	}

	boost::asio::async_read_until(client->sock, client->buff, cond, std::bind(&ServerTcp::onRead, this, client, std::placeholders::_1, std::placeholders::_2));
}

void ServerTcp::start()
{
	stop();
	acceptor.async_accept(clients.front()->sock, std::bind(&ServerTcp::onAccept, this, clients.front(), std::placeholders::_1));
}

void ServerTcp::stop()
{
	std::remove_if(clients.begin(), clients.end(), [&](std::shared_ptr<Client> &t)
	{
		t->sock.close();
		if(clients.size() > 1)
			return true;
		return false;
	});
}

void ServerTcp::setMaxClients(std::size_t maxVal)
{
	maxClients = maxVal;
}

void ServerTcp::setRegexCondition(const boost::regex& x)
{
	cond = x;
}

void ServerTcp::setReadEventHandler(std::function<void(std::shared_ptr<Client>, std::size_t)> _read)
{
	read = _read;
}

void ServerTcp::setConnectedEventHandler(std::function<void(std::shared_ptr<Client>)> _connected)
{
	connected = _connected;
}

void ServerTcp::setDisconnectedEventHandler(std::function<void(std::shared_ptr<Client>, const boost::system::error_code&)> _disconnected)
{
	disconnected = _disconnected;
}
