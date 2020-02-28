#include "ArgsParser.h"

ArgsParser::ArgsParser(int argc, char** argv) : m_argc(argc), m_argv(argv)
{
	if (argc < 2)
	{
		m_errorString = "Invalid arguments number, please provide at least one.";
	}
	else
	{
		m_interval = atoi(argv[1]);
	}
}

bool ArgsParser::hasError() const
{
	if (m_errorString.empty())
		return false;

	return true;
}

const std::string& ArgsParser::errorString() const
{
	return m_errorString;
}

int ArgsParser::interval() const
{
	return m_interval;
}
