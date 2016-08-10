#include "comanddispatcher.h"
#include <algorithm>
#include <boost/asio.hpp>
#include <boost/iostreams/filter/regex.hpp>
#include <iostream>

using namespace boost::iostreams;

CommandDispatcher::CommandDispatcher()
{

}

CommandDispatcher::~CommandDispatcher()
{

}

void CommandDispatcher::registerCommand(Command* command)
{
	auto end = registeredCommands.end();
	auto found = std::find_if(registeredCommands.begin(), end, [&](const CommandSPtr& cmd){return (cmd->getName() == command->getName());});
	if(end != found)
		registeredCommands.erase(found);
	registeredCommands.push_back(CommandSPtr(command));
}

void CommandDispatcher::run(const std::string& cmd, const std::string& args)
{
	std::cout << "new cmd: " << cmd << ", arg is: " << args << std::endl;






}

void CommandDispatcher::dispatch(std::streambuf& indata)
{
	if (registeredCommands.empty() || !indata.in_avail())
		return;

	boost::regex exp("(?:\\r|\\n|\\0|\\A|^)*(\\w+)\\s*(\\P{cntrl}*)\\s*(?:\\r|\\n){1,2}");
	std::string str((std::istreambuf_iterator<char>(&indata)), std::istreambuf_iterator<char>());
	boost::smatch res;;

	/***************/
	std::cout << "\r\n---------INIT-----------\r\n" << std::endl;
	std::cout << exp.expression() << std::endl;
	std::cout << "raw: " << str << std::endl;
	/***************/

	int y = 0;
	while(boost::regex_search(str, res, exp))
	{
		std::cout << "-----------------------\r\nres: " << y << ",  data:" << res << std::endl;
		int i = 0;
		std::for_each(res.begin(), res.end(), [&](const std::string::const_iterator & it)
		{
			std::string cmd;
			std::move(it.first, it.second, std::back_inserter(cmd));
			std::cout << i++ << ": " << cmd << std::endl;
		});
		std::cout << "-----------------------\r\n" << std::endl;

	};

	//		std::string cmd, args;
	//		std::move(res[1].first, res[1].second, std::back_inserter(cmd));
	//		std::move(res[2].first, res[2].second, std::back_inserter(args));
	//		run(cmd, args);



}

