#include "qtinvokedispatcher.h"
#include <QVariant>

QtInvokeDispatcher::QtInvokeDispatcher(pClient clt) : Dispatcher(clt, boost::regex("(?:\\r|\\n|\\0|\\A|^)*(\\w+)\\s*(\\P{cntrl}*)\\s*(?:\\r|\\n){2,}"))
{

}

QtInvokeDispatcher::~QtInvokeDispatcher()
{

}

void QtInvokeDispatcher::setRootObject(QObject* obj)
{
	root = obj;
}
QObject * QtInvokeDispatcher::root;

void QtInvokeDispatcher::dispatch(boost::match_results<Dispatcher::streambuf_iterator> res)
{

	invokeMethod(res[1], res[2]);
}

void QtInvokeDispatcher::invokeMethod(std::__cxx11::string method, std::__cxx11::string value)
{
	if(root)
		QMetaObject::invokeMethod(root, method.c_str(), Qt::QueuedConnection, Q_ARG(QVariant, value.c_str()));
}
