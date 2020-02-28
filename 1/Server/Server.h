#pragma once

#include <WinSock2.h>

class Server
{
public:
	Server(unsigned short port);
	virtual ~Server();
public:
	void bind();

// Templates
protected:
	// Implement to create a socket with a correct type and return it
	virtual uintptr_t createSocket() = 0;

protected:
	void init();

protected:
	uintptr_t m_socket;
	unsigned short m_port;
};