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
#include <QQuickWindow>

#include <colornotifier.h>

#include <QCommandLineParser>

using namespace boost::asio;

int main(int argv, char** argc)
{
    /* qt routine */
	QGuiApplication app(argv, argc);
    QCoreApplication::setApplicationName("lodgia b-day server");
    QCoreApplication::setApplicationVersion("1.1");

    /* qml ui */
	QQmlEngine engine;
	QQmlComponent component(&engine, QUrl(QStringLiteral("qrc:/main.qml")));
    QQuickWindow *window = qobject_cast<QQuickWindow *>(component.create());
    QtInvokeDispatcher::setRootObject(window);

    /* color notifier */
    ColorNotifier * colorNotifier = new ColorNotifier();
    QObject::connect(window, SIGNAL(textColorChanged(QColor)), colorNotifier, SLOT(setColor(QColor)));
    QObject::connect(window, SIGNAL(backColorChanged(QColor)), colorNotifier, SLOT(setBack(QColor)));

    /* network */
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
    server.start();

    /* command-line options */
    QCommandLineParser parser;
    parser.setApplicationDescription("Test helper");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption portOption(QStringList() << "p" << "port", "Write color to <port>.", "port");
    parser.addOption(portOption);
    /* option parsing */
    parser.process(app);
    if(parser.isSet(portOption))
        std::cout << "port set: " << parser.value(portOption).toStdString() << std::endl;
    else
        std::cout << "port not set" << std::endl;

    /* run */
	std::thread netservise([&](){service.run();});
	app.exec();
}
