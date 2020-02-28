#include "UdpClient.h"

#include <iostream>
#include <WinSock2.h>
#include <ctime>

#include "DgramStructs.h"
#include "WinsockWrappers.h"

UdpClient::UdpClient(int timeout, unsigned short port) : m_socket(INVALID_SOCKET), m_interval(timeout), 
														 m_port(port), m_currentValue(0)
{
	WSADATA wsaData;
	WSAStartup(0x202, &wsaData);
	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

UdpClient::~UdpClient()
{
	if (m_socket != INVALID_SOCKET)
		closesocket(m_socket);
	WSACleanup();
}

void UdpClient::loop()
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_port);
	addr.sin_addr = Utils::strToAddr("127.0.0.1");

	forever
	{
		Sleep(m_interval);
		m_currentValue += m_interval;
		GETSINCHRO getStruct;
		memcpy(getStruct.cmd, "SINC", 4);
		getStruct.curvalue = m_currentValue;
		
		int sentLen = sendto(m_socket, reinterpret_cast<const char*>(&getStruct), sizeof(getStruct), 0, (sockaddr*)&addr, sizeof(addr));
		std::cout << "Sent " << sentLen << " bytes. ";
		SETSINCHRO setStruct;

		int addrLen = sizeof(addr);
		unsigned int lastClock = clock();
		recvfrom(m_socket, reinterpret_cast<char*>(&setStruct), sizeof(setStruct), 0, (sockaddr*)&addr, &addrLen);
		std::cout << "recv => m_currentValue: " << m_currentValue << "; correction: " << setStruct.correction << std::endl;
		m_currentValue += setStruct.correction + clock() - lastClock;
	}
}
