#pragma once

struct GETSINCHRO // client request for counter sync
{
	char cmd[4];  // always SINC
	int curvalue; // current counter value
};

struct SETSINCHRO   // server response for time counter sync
{
	char cmd[4];    // always SINC
	int correction; // value wich necessary to add to time counter
};

struct NTPPackage
{
	char head[4];
	unsigned long RootDelay;
	unsigned long RootDispersion;
	char ReferenceIdentifier[4];
	unsigned long ReferenceTimestamp[2];
	unsigned long long OriginateTimestamp;
	unsigned long TransmitTimestamp[2];
	unsigned long KeyIdentifier;
	unsigned long long MessageDigest[2];
};