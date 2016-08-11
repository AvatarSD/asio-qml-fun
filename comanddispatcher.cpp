#include "comanddispatcher.h"
#include <algorithm>
#include <boost/asio.hpp>
#include <boost/regex.hpp>
#include <iostream>


CommandDispatcher::CommandFactoryMap CommandDispatcher::registeredCommands;

CommandDispatcher::CommandDispatcher(pClient clt) : Dispatcher(clt, boost::regex("(?:\\r|\\n|\\0|\\A|^)*(\\w+)\\s*(\\P{cntrl}*)\\s*(?:\\r|\\n){2,}"))
{

}

CommandDispatcher::~CommandDispatcher()
{

}

void CommandDispatcher::dispatch(boost::match_results<streambuf_iterator> res)
{
	if(res.size() < 2)
		return;

	auto sub = registeredCommands.find(res[1]);
	if(sub != registeredCommands.end())
		if(sub->first == res[1])
			run(pCommand(sub->second(client)), res[2]);

}

void CommandDispatcher::run(pCommand command, const std::string& args)
{

	command->execute(args);


}
