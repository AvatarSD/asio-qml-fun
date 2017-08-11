#include <QGuiApplication>
#include <QQmlEngine>
#include <QQmlComponent>

#include <iostream>
#include <thread>
#include <streambuf>

#include <tcpserver.h>
#include <commands.h>
#include <unordered_map>

#include <qtinvokedispatcher.h>

using namespace boost::asio;

int main(int argv, char** argc)
{
	QGuiApplication app(argv, argc);

	QQmlEngine engine;
	QQmlComponent component(&engine, QUrl(QStringLiteral("qrc:/main.qml")));

	QtInvokeDispatcher::setRootObject(component.create());

	io_service service;
	TcpServer server(service, ip::tcp::endpoint(ip::tcp::v4(), 6666));
	std::unordered_map<pClient, std::shared_ptr<Dispatcher> > dispatchersList;
	server.setReadEventHandler([&](pClient client, std::size_t){(*dispatchersList[client])();});
	server.setConnectedEventHandler([&](pClient client)
	{
		dispatchersList[client] = std::make_shared<QtInvokeDispatcher>(client);
		std::cout << "New client: " << client->sock.remote_endpoint().address().to_string() << std::endl;
	});
	server.setDisconnectedEventHandler([&](pClient client, const boost::system::error_code&)
	{
		dispatchersList.erase(client);
		std::cout << "Disconnected: " << client->sock.remote_endpoint().address().to_string() << std::endl;
	});

	//server.setReadUntilCondition(boost::regex("s|k|\n|\r"));
	server.start();

	std::thread netservise([&](){service.run();});
	app.exec();
