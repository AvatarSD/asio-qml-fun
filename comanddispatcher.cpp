#include "comanddispatcher.h"
#include <algorithm>
#include <boost/asio.hpp>
#include <boost/regex.hpp>
#include <iostream>

#include <chrono>

CommandDispatcher::CommandDispatcher()
{

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

void CommandDispatcher::run(Command* command, const std::string& args)
{
	//std::cout << "new cmd: " << cmd << ", arg is: " << args << std::endl;






}

void CommandDispatcher::dispatch(boost::match_results<streambuf_iterator> res)
{
	std::cout << "-----------------------" << std::endl;
	int i = 0;
	std::for_each(res.begin(), res.end(), [&](const boost::sub_match<streambuf_iterator> & sub)
	{ std::cout << i++ << ": " << sub << std::endl;});
	std::cout << "-----------------------" << std::endl;






}

void CommandDispatcher::operator ()(boost::asio::streambuf& indata)
{
	if (registeredCommands.empty() || !indata.size())
		return;

	/***************/
	std::cout << "\r\n---------INIT-----------" << std::endl;
	std::cout << exp.expression() << std::endl;
	//std::cout << "raw: " << &indata << std::endl;
	/***************/

	auto begin = std::chrono::system_clock::now();

	//	boost::regex_iterator<streambuf_iterator> m1(boost::asio::buffers_begin(indata.data()), boost::asio::buffers_end(indata.data()), exp);
	//	boost::regex_iterator<streambuf_iterator> m2;
	//	streambuf_iterator last = boost::asio::buffers_begin(indata.data());
	//	std::for_each(m1, m2, [&](const boost::match_results<streambuf_iterator> & res){
	//		dispatch(res);
	//		last = res.suffix().first;
	//	});
	//	indata.consume(std::distance(boost::asio::buffers_begin(indata.data()), last));


	boost::match_results<streambuf_iterator> res;
	while(boost::regex_search(boost::asio::buffers_begin(indata.data()), boost::asio::buffers_end(indata.data()), res, exp))
	{
		dispatch(res);
		indata.consume(std::distance(boost::asio::buffers_begin(indata.data()), res.suffix().first));
	};

	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = (end-begin);
	std::cout << "elapsed time: " << elapsed_seconds.count() << "s" << std::endl;

	//	for(auto it = boost::asio::buffers_begin(indata.data()); it != boost::asio::buffers_end(indata.data()); it++)
	//		std::cout << ":" << *it << std::endl;

	std::cout << "\r\n---------END-----------\r\n" << std::endl;
}

