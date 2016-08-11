#include "comanddispatcher.h"
#include <algorithm>
#include <boost/asio.hpp>
#include <boost/regex.hpp>
#include <iostream>

#include <chrono>

CommandDispatcher::CommandDispatcher()
{
	setRegex(boost::regex("(?:\\r|\\n|\\0|\\A|^)*(\\w+)\\s*(\\P{cntrl}*)\\s*(?:\\r|\\n){2,}"));
}

CommandDispatcher::~CommandDispatcher()
{

}

void CommandDispatcher::setRegex(const boost::regex& expression)
{
	exp = expression;
}

void CommandDispatcher::registerCommand(Command* command)
{
	auto end = registeredCommands.end();
	auto found = std::find_if(registeredCommands.begin(), end, [&](const CommandSPtr& cmd){return (cmd->getName() == command->getName());});
	if(end != found)
		registeredCommands.erase(found);
	registeredCommands.push_back(CommandSPtr(command));
}

void CommandDispatcher::operator ()(std::shared_ptr<Client> client)
{
	if (registeredCommands.empty() || !client->buff.size())
		return;

	boost::match_results<streambuf_iterator> res;
	while(boost::regex_search(boost::asio::buffers_begin(client->buff.data()), boost::asio::buffers_end(client->buff.data()), res, exp))
	{
		dispatch(res, client);
		client->buff.consume(std::distance(boost::asio::buffers_begin(client->buff.data()), res.suffix().first));
	};

}

void CommandDispatcher::dispatch(boost::match_results<streambuf_iterator> res, std::shared_ptr<Client> client)
{
	//	std::cout << "-----------------------" << std::endl;
	//	int i = 0;
	//	std::for_each(res.begin(), res.end(), [&](const boost::sub_match<streambuf_iterator> & sub)
	//	{ std::cout << i++ << ": " << sub << std::endl;});
	//	std::cout << "-----------------------" << std::endl;

	auto seq = res.begin();
	for(auto sub : registeredCommands)
		if(sub->getName() == *(++seq))
		{
			std::vector<std::string> args;
			std::for_each(++seq, res.end(), [&](const auto & sub){ args.push_back(sub);});
			run(client, sub, args);
		}
}

void CommandDispatcher::run(std::shared_ptr<Client> client, CommandSPtr command, const std::vector<std::string>& args)
{
	//std::cout << "new cmd: " << cmd << ", arg is: " << args << std::endl;

	//std::count << command->getName() << ":" << std::endl;
	//for(auto arg : args)
	//	std::cout << " -" << arg << std::endl;

	command->execute(client, args);


}
