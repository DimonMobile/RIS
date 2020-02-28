#pragma once

#include <string>

namespace Constants
{

	namespace Errors
	{

		enum class Code
		{
			WSAInitError = 0,
			SocketBindError,
			SocketCreateError
		};

		std::string getDescription(Code code);
	} // namespace Errors

} // namespace Constants