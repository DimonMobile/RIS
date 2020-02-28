#pragma once

#include <stdint.h>

#define forever for (;;)

class UdpClient
{
public:
	UdpClient(int interval, unsigned short port);
	~UdpClient();

	void loop();

private:
	uintptr_t m_socket;

	int m_interval;
	unsigned short m_port;
	int m_currentValue;
};

