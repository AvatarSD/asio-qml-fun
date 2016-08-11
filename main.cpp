#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <iostream>
#include <thread>
#include <streambuf>

#include <tcpserver.h>
#include <comanddispatcher.h>
#include <commands.h>

using namespace boost::asio;

int main(int argv, char** argc)
{
	QGuiApplication app(argv, argc);
	QQmlApplicationEngine engine;
	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


	CommandDispatcher dispatcher;
	dispatcher.setRegex(boost::regex("(?:\\r|\\n|\\0|\\A|^)*(\\w+)\\s*(\\P{cntrl}*)\\s*(?:\\r|\\n){2,}"));
	dispatcher.registerCommand(new QtIvokeMethod(engine.findChildren<QObject*>()));


	io_service service;
	ServerTcp server(service, ip::tcp::endpoint(ip::tcp::v4(), 6666));

	server.setReadEventHandler([&](std::shared_ptr<Client> client, std::size_t){dispatcher(client->buff);});
	server.setConnectedEventHandler([](std::shared_ptr<Client> client)
	{
		std::cout << "New client: " << client->sock.remote_endpoint().address().to_string() << std::endl;
	});
	server.setDisconnectedEventHandler([](std::shared_ptr<Client> client, const boost::system::error_code&)
	{
		std::cout << "Disconnected: " << client->sock.remote_endpoint().address().to_string() << std::endl;
	});

	//server.setReadUntilCondition(boost::regex("s|k|\n|\r"));
	server.start();

	std::thread netservise([&](){service.run();});
	app.exec();
}
