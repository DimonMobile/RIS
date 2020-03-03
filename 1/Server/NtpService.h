#pragma once

#include <string>
#include <mutex>

class NtpService
{
public:
	NtpService(int interval, const std::string &server);
	unsigned long getTime();
	void stop();
	~NtpService();
private:
	void syncTime();

private:
	bool m_working;
	int m_interval;
	volatile unsigned long m_currentTime;
	std::string m_serverAddress;
	std::thread m_timeThread;
	std::mutex m_timeAccessMutex;
};

