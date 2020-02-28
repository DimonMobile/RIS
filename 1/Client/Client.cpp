#include <iostream>

#include "ArgsParser.h"
#include "UdpClient.h"

int main(int argc, char **argv)
{
	ArgsParser argsParser(argc, argv);
	if (argsParser.hasError())
	{
		std::cerr << argsParser.errorString() << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "Interval set to " << argsParser.interval() << std::endl;

	UdpClient udpClient(argsParser.interval(), 7777);
	udpClient.loop();
}