#pragma once

#include "Server.h"
#include "NtpService.h"

#define forever for (;;)

struct GETSINCHRO;
struct SETSINCHRO;

class UdpServer : public Server
{
public:
	UdpServer(unsigned short port);
	void startLoop();

protected:
	virtual uintptr_t createSocket() override;

private:
	sockaddr readStruct(GETSINCHRO* sync) const;
	void writeStruct(const SETSINCHRO& sync, const sockaddr& to) const;

private:
	NtpService m_ntpService;
};