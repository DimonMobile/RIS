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