#include "commands.h"



boost::regex QtIvokeMethod::getRegex()
{
	return boost::regex("\n");
}

void QtIvokeMethod::execute(const std::vector<std::__cxx11::string>& )
{
}

std::__cxx11::string QtIvokeMethod::getName()
{
	return "invoke";
}
