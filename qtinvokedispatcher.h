#ifndef QTINVOKEDISPATCHER_H
#define QTINVOKEDISPATCHER_H

#include <dispatcher.h>
#include <QObject>

class QtInvokeDispatcher : public Dispatcher
{
public:
	QtInvokeDispatcher(pClient clt);
	~QtInvokeDispatcher();

	static void setRootObject(QObject * obj);

private:
	void dispatch(boost::match_results<streambuf_iterator> res) override;
	static QObject * root;

	void invokeMethod(std::string method, std::string value);
};

#endif // QTINVOKEDISPATCHER_H
