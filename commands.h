#ifndef COMMANDS_H
#define COMMANDS_H

#include <comanddispatcher.h>

#include <QObject>
#include <QList>
#include <boost/regex.h>

class QtMananger : public Command
{
public:
	QtMananger(pClient client) : Command(client){}
	virtual ~QtMananger() {}

	static std::string getName();
	static void setRoot(QObject* root){rootObject = root;}

	// Command interface
	void execute(const std::string& args) override;

private:
	static QObject* rootObject;

	void setProperty(std::string name, std::string property, std::string value);
	void getObjectTree();
	void getProperty(std::string);
	void invokeMethod(std::string, std::string);


};






#endif // COMMANDS_H
