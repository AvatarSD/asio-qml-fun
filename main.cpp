#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <iostream>
#include <thread>
#include <streambuf>
#include <tcpserver.h>

using namespace boost::asio;

int main(int, char**)
{
	//	QGuiApplication app(argc, argv);
	//	QQmlApplicationEngine engine;
	//	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


	io_service service;
	ServerTcp server(service, ip::tcp::endpoint(ip::tcp::v4(), 6666));

	server.setConnectedEventHandler([](std::shared_ptr<Client> client)
	{
		std::cout << "New client: " << client->sock.remote_endpoint().address().to_string() << std::endl;
	});
	server.setReadEventHandler([](std::shared_ptr<Client> client, std::size_t)
	{
		std::cout << &client->buff << std::endl;
	});
	server.setDisconnectedEventHandler([](std::shared_ptr<Client> client, const boost::system::error_code&)
	{
		std::cout << "Disconnected: " << client->sock.remote_endpoint().address().to_string() << std::endl;
	});

	server.setRegexCondition(boost::regex("s|k|\n|\r"));
	server.start();

	std::thread netservise([&](){service.run();});
	while(1);
	netservise.join();

	return 0;//app.exec();
}
