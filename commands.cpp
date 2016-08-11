#include "commands.h"
#include <boost/regex.h>
#include <QObject>
#include <QVariant>
#include <string>

void QtMananger::execute(const std::string& args)
{
	//client->sock.async_send(boost::asio::buffer("--ok--\r\n"), [](...){});

	if(!rootObject)
		client->sock.async_send(boost::asio::buffer("no root object\r\n"), [](...){});

	boost::match_results<std::string::const_iterator> res;

	if(boost::regex_search(args, res, boost::regex("prop\\s*(\\w+)\\s+(\\w+)\\s+(\\w+|\\d+)\\s*")))
		setProperty(res[1],res[2],res[3]);
	else if(boost::regex_search(args, res, boost::regex("prop\\s+(\\w+)")))
		getProperty(res[1]);
	else if(boost::regex_search(args, res, boost::regex("tree")))
		getObjectTree();
	else if(boost::regex_search(args, res, boost::regex("inv\\s+(\\w+)\\s+(\\w+)")))
		invokeMethod(res[1],res[2]);


	client->sock.async_send(boost::asio::buffer("---ok---\r\n"), [](...){});
	return;
}

void QtMananger::setProperty(std::string name, std::string property, std::string value)
{

	auto foundObject = rootObject->findChild<QObject*>(name.c_str());
	if(foundObject)
	{
		QMetaObject::invokeMethod(foundObject, "setProperty", Qt::QueuedConnection, Q_ARG(QString, property.c_str()), Q_ARG(QVariant, value.c_str()));
		//foundObject->setProperty(property.c_str(), QString(value.c_str()));
		client->sock.async_send(boost::asio::buffer("ok!\r\n"), [](...){});
		return;
	}
}

void QtMananger::getObjectTree()
{
	boost::asio::streambuf buf;
	std::ostream out(&buf);
	int level = 0;

	std::function<void(int,QObject*)> getObject;
	getObject = [&](int lev, QObject* obj){
		out << ":";
		for(auto i = 0; i != lev; i++) out << "-";
		out << obj->objectName().toStdString() << std::endl;
		auto child = obj->children();
		for(auto it = child.begin(); it != child.end(); ++it)
			getObject(++lev, *it);
	};

	out << "available objects:\r\n";
	getObject(level, rootObject);

	client->sock.async_send(buf.data(), [](...){});
}

void QtMananger::getProperty(std::string name)
{

	boost::asio::streambuf buf;
	std::ostream out(&buf);
	out << name << ":\r\n";
	auto foundObject = rootObject->findChild<QObject*>(name.c_str());
	//	auto it = foundObject->dynamicPropertyNames();
	//	auto r = it.back();
	if(foundObject)
		for(auto it = foundObject->dynamicPropertyNames().begin(); it != foundObject->dynamicPropertyNames().end(); ++it)
			out << "-" << it->toStdString() << std::endl;
	//if(r == it.first()) out << "fd";
	else out << "not found\r\n";
	client->sock.async_send(buf.data(), [](...){});
}

void QtMananger::invokeMethod(std::string method, std::string value)
{
	if(rootObject)
		QMetaObject::invokeMethod(rootObject, method.c_str(), Qt::QueuedConnection, Q_ARG(QVariant, value.c_str()));
}

std::string QtMananger::getName()
{
	return "qt";
}

QObject* QtMananger::rootObject;
