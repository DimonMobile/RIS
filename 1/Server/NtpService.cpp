#include "NtpService.h"

#include <WinSock2.h>
#include <thread>

#include "WinsockWrappers.h"
#include "DgramStructs.h"


NtpService::NtpService(int interval, const std::string& server) 
	: m_working(true), m_interval(interval), m_serverAddress(server), m_timeThread(&NtpService::syncTime, this)
{
}

unsigned long NtpService::getTime()
{
	std::lock_guard<std::mutex> guard(m_timeAccessMutex);
	return m_currentTime;
}

unsigned long NtpService::evaluateTime()
{
	std::lock_guard<std::mutex> guard(m_timeAccessMutex);
	return static_cast<unsigned long>((clock() / (CLOCKS_PER_SEC / 1000.0) - m_lastTimeGot) + m_currentTime);
}

void NtpService::stop()
{
	m_working = false;
}

NtpService::~NtpService()
{
	m_working = false;
	if (m_timeThread.joinable())
		m_timeThread.join();
}

void NtpService::syncTime()
{
	while (m_working)
	{
		NTPPackage package = { 0 };
		package.head[0] = 0x1B;
		package.head[1] = 0x00;
		package.head[2] = 4;
		package.head[3] = 0xEC;

		SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(123);
		addr.sin_addr = Utils::strToAddr(m_serverAddress); // "88.147.254.230"
		
		int addrLen = sizeof(addr);
		sendto(sock, reinterpret_cast<char*>(&package), sizeof(package), 0, reinterpret_cast<sockaddr*>(&addr), addrLen);
		recvfrom(sock, reinterpret_cast<char*>(&package), sizeof(package), 0, reinterpret_cast<sockaddr*>(&addr), &addrLen);
		closesocket(sock);

		m_timeAccessMutex.lock();
		m_currentTime = ntohl(package.TransmitTimestamp[0]) - 613608u * 3600;
		m_lastTimeGot = clock() / (CLOCKS_PER_SEC / 1000.0);
		m_timeAccessMutex.unlock();

		Sleep(m_interval);
	}
}
