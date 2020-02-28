#include <iostream>

#include "UdpServer.h"

int main()
{
	try
	{
		UdpServer server(7777);
		server.bind();
		server.startLoop();
	}
	catch (const std::runtime_error &e)
	{
		std::cerr << "Erorr code: " << e.what() << std::endl;
		throw e;
	}
}
