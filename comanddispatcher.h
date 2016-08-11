#ifndef COMANDDISPATCHER_H
#define COMANDDISPATCHER_H

#include <boost/regex.hpp>
#include <boost/asio.hpp>
#include <tcpserver.h>



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
	typedef boost::asio::buffers_iterator<boost::asio::streambuf::const_buffers_type> streambuf_iterator;

	CommandDispatcher();
	~CommandDispatcher();

	void setRegex(const boost::regex & expression);

	void registerCommand(Command* command);
	void run(Command* command, const std::string& args); // call command
	void dispatch(boost::match_results<streambuf_iterator> res);
	void operator ()(boost::asio::streambuf&); //dispatch parse arguments

	// possibly include isRegistered, unregisterCommand, etc.

private:
	CommandMap registeredCommands;
	boost::regex exp;
};

#endif // COMANDDISPATCHER_H
