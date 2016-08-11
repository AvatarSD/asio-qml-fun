#ifndef COMANDDISPATCHER_H
#define COMANDDISPATCHER_H

#include <functional>
#include <dispatcher.h>

class Command
{
public:
	Command(pClient clt) : client(clt) {}
	virtual ~Command() {}

	virtual void execute(const std::string& args) = 0;

protected:
	pClient client;
};

class CommandDispatcher : public Dispatcher
{
public:
	typedef std::shared_ptr<Command> pCommand;
	typedef std::map<std::string, std::function<Command*(pClient)> > CommandFactoryMap;

	CommandDispatcher(pClient client);
	~CommandDispatcher();

	template<class T> static bool registerCommand();

private:
	static CommandFactoryMap registeredCommands;
	void run(pCommand command, const std::string& args = ""); // call command
	void dispatch(boost::match_results<streambuf_iterator> res);

};

template<class T>
bool CommandDispatcher::registerCommand()
{
	CommandFactoryMap::iterator it = registeredCommands.find(T::getName());
	if(it != registeredCommands.end())
		return false;
	registeredCommands[T::getName()] = [](pClient client)->Command*{return new T(client);};
	return true;
}

#endif // COMANDDISPATCHER_H
