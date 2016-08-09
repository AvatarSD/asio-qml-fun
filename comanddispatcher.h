#ifndef COMANDDISPATCHER_H
#define COMANDDISPATCHER_H

#include <boost/regex.hpp>
#include <boost/asio.hpp>
#include <streambuf>

class Command
{
public:
	Command() {}
	virtual ~Command(void) {}

	virtual boost::regex getRegex() = 0;
	virtual std::string getName() = 0;
	virtual void execute(const std::vector<std::string>& args) = 0;
};

class CommandDispatcher
{
public:
	typedef std::shared_ptr<Command> CommandSPtr;
	typedef std::list<CommandSPtr> CommandMap;

	CommandDispatcher();
	~CommandDispatcher();

	void registerCommand(Command* command);
	void run(const std::string& cmd, const std::string& args); // call command
	void dispatch(std::streambuf&); // parse arguments

	// possibly include isRegistered, unregisterCommand, etc.

private:
	CommandMap registeredCommands;
};

#endif // COMANDDISPATCHER_H
