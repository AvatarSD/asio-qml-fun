#ifndef COMMANDS_H
#define COMMANDS_H

#include <comanddispatcher.h>

#include <QObject>
#include <QList>
#include <boost/regex.h>

class QtIvokeMethod : public Command
{
public:
	QtIvokeMethod(const QList<QObject*> & objects) : Command(), rootObjects(objects){}
	virtual ~QtIvokeMethod() {}

private:
	QList<QObject*> rootObjects;

	// Command interface
public:
	boost::regex getRegex() override;
	void execute(std::shared_ptr<Client> client, const std::vector<std::string>& args) override;
	std::string getName() override;
};




#endif // COMMANDS_H
