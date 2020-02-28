#pragma once

#include <string>

struct in_addr;

namespace Utils
{
	std::string addrToStr(const in_addr& addr);
	in_addr strToAddr(const std::string& addr);
} // namespace Utils