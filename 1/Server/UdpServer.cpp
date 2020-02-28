#include "UdpServer.h"

#include <limits>
#include <iostream>
#include <ctime>

#include "DgramStructs.h"
#include "WinsockWrappers.h"

UdpServer::UdpServer(unsigned short port) : Server(port)
{
}

void UdpServer::UdpServer::startLoop()
{
	int requestId = 0;
	int correctionSum = 0;

	int minCorrection = (std::numeric_limits<int>::max)();
	int maxCorrection = (std::numeric_limits<int>::min)();
	forever
	{
		GETSINCHRO getStruct;
		sockaddr clientAddr = readStruct(&getStruct);

		if (memcmp(getStruct.cmd, "SINC", 4) == 0)
		{
			SETSINCHRO setStruct;
			memcpy(setStruct.cmd, "SINC", 4);
			setStruct.correction = clock() - getStruct.curvalue;

			if (setStruct.correction > maxCorrection)
				maxCorrection = setStruct.correction;

			if (setStruct.correction < minCorrection)
				minCorrection = setStruct.correction;

			correctionSum += setStruct.correction;
			writeStruct(setStruct, clientAddr);
			std::cout << '[' << ++requestId << ']' << Utils::addrToStr(reinterpret_cast<sockaddr_in*>(&clientAddr)->sin_addr) 
				      << " - " << setStruct.correction << "; avg: " << correctionSum / requestId << std::endl;
		}
	}
	//std::cout << "Min: " << minCorrection << "; Max: " << maxCorrection << "; Avg: " << (double)correctionSum / requestId << "; Total: " << requestId << std::endl;
}

uintptr_t UdpServer::createSocket()
{
	return socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

sockaddr UdpServer::readStruct(GETSINCHRO *sync) const
{
	sockaddr result;
	int sockaddrLen = sizeof(sockaddr);

	recvfrom(m_socket, reinterpret_cast<char*>(sync), sizeof(GETSINCHRO), 0, &result, &sockaddrLen);
	return result;
}

void UdpServer::writeStruct(const SETSINCHRO &sync, const sockaddr &to) const
{
	sendto(m_socket, reinterpret_cast<const char*>(&sync), sizeof(sync), 0, &to, sizeof(to));
}