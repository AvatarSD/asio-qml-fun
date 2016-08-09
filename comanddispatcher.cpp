#include "comanddispatcher.h"
#include <iostream>


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

	boost::smatch res;
	boost::regex exp("(\\w+)\\s*(\\P{cntrl}*)"); //"(?:\\r|\\n|\\0|\\A)*(\\w+)\\s*(\\P{cntrl})(?:\\r|\\n)"

	std::string str((std::istreambuf_iterator<char>(&indata)), std::istreambuf_iterator<char>());

	std::cout << "raw: " << str;
	std::cout << "-----------------------\r\nsearch: " << (boost::regex_search(str, res, exp) ? "true" : "false") << std::endl;
	std::cout << "res: " << res.size() << ",  data:" << std::endl;
	for(auto it = res.begin(); it != res.end(); ++it)
	{
		std::string cmd;
		std::move(it->first, it->second, std::back_inserter(cmd));
		std::cout << cmd << std::endl;
	}

	//		std::string cmd, args;
	//		std::move(res[1].first, res[1].second, std::back_inserter(cmd));
	//		std::move(res[2].first, res[2].second, std::back_inserter(args));
	//		run(cmd, args);

}

