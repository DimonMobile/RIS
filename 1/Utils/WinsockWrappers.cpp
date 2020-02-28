#include "WinsockWrappers.h"

#include <WinSock2.h>
#include <WS2tcpip.h>

namespace Utils
{
	std::string addrToStr(const in_addr& addr)
	{
		char resultBuf[16]; // 3 * 4 + 3 dots + \n character
		if (inet_ntop(AF_INET, &addr, resultBuf, sizeof(resultBuf)) == nullptr)
			return std::string();
		return std::string(resultBuf);
	}

	in_addr strToAddr(const std::string& addr)
	{
		in_addr result;
		inet_pton(AF_INET, addr.c_str(), &result);
		return result;
	}
} // namespace Utils