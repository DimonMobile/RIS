#include "Constants.h"

std::string Constants::Errors::getDescription(Code code)
{
	switch (code)
	{
	case Code::WSAInitError:
		return "WSAStartup error";
	case Code::SocketBindError:
		return "Socket bind error";
	case Code::SocketCreateError:
		return "Create socket error";
	default:
		return "Unknown error";
	}
}
