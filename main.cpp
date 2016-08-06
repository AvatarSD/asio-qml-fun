#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <iostream>
#include <thread>
#include <functional>


#include <boost/asio.hpp>
using namespace boost::asio;



int main(int argc, char *argv[])
{
	//	QGuiApplication app(argc, argv);
	//	QQmlApplicationEngine engine;
	//	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


	io_service service;
	ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 6666));

	ip::tcp::socket sock(service);
	char buff[1024];

	std::function<void(boost::system::error_code, std::size_t)> onRead;
	std::function<void(boost::system::error_code)> onAccept;

	onRead = [&](boost::system::error_code err, std::size_t size)
	{
		if(err)
		{
			std::cout << "Disconnected from " << sock.remote_endpoint().address().to_string() << std::endl;
			sock.close();
			acceptor.async_accept(sock, onAccept);
		}
		else
		{

			//test
			std::cout.write(buff, size);
			std::cout << std::endl;


			sock.async_receive(buffer(buff,1024), onRead);
		}
	};

	onAccept = [&](boost::system::error_code err)
	{
		if(err)
		{
			std::cout << err.message() << std::endl;
			return;
		}
		std::cout << "New client: " << sock.remote_endpoint().address().to_string() << std::endl;
		sock.async_receive(buffer(buff,1024), onRead);
	};

	acceptor.async_accept(sock, onAccept);




	std::thread netservise([&](){service.run();});

	//while(1);

	netservise.join();
	return 0;//pp.exec();
}
