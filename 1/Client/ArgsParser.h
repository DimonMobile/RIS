#pragma once

#include <string>

class ArgsParser
{
public:
	ArgsParser(int argc, char** argv);
	bool hasError() const;
	const std::string& errorString() const;
	int interval() const;

private:
	int m_argc;
	char** m_argv;

	int m_interval;
	std::string m_errorString;
};

