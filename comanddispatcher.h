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
	virtual void execute(std::shared_ptr<Client> client, const std::vector<std::string>& args) = 0;
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
	void run(std::shared_ptr<Client> client, CommandSPtr command, const std::vector<std::string>& args); // call command
	void dispatch(boost::match_results<streambuf_iterator> res, std::shared_ptr<Client> client);
	void operator ()(std::shared_ptr<Client> client); //dispatch parse arguments

	// possibly include isRegistered, unregisterCommand, etc.

private:
	CommandMap registeredCommands;
	boost::regex exp;
};

#endif // COMANDDISPATCHER_H
