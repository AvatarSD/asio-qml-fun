#include "commands.h"



boost::regex QtIvokeMethod::getRegex()
{
	return boost::regex("\n");
}

void QtIvokeMethod::execute(std::shared_ptr<Client> client, const std::vector<std::__cxx11::string>& )
{

	client->sock.async_send(boost::asio::buffer("--ok--\r\n"), [](...){});
}

std::__cxx11::string QtIvokeMethod::getName()
{
	return "invoke";
}
