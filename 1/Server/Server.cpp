#include "Server.h"

#include <stdexcept>

#include "Constants.h"

Server::Server(unsigned short port) : m_socket(INVALID_SOCKET), m_port(port)
{
	init();
}

Server::~Server()
{
	if (m_socket != INVALID_SOCKET)
		closesocket(m_socket);
	WSACleanup();
}

void Server::bind()
{
	sockaddr_in bindAddr;
	bindAddr.sin_family = AF_INET;
	bindAddr.sin_port = htons(m_port);
	bindAddr.sin_addr.S_un.S_addr = INADDR_ANY;

	if ((m_socket = createSocket()) == INVALID_SOCKET)
		throw std::runtime_error(Constants::Errors::getDescription(Constants::Errors::Code::SocketCreateError));

	if (::bind(m_socket, (sockaddr*)&bindAddr, sizeof(bindAddr)) == SOCKET_ERROR)
		throw std::runtime_error(Constants::Errors::getDescription(Constants::Errors::Code::SocketBindError));
}

void Server::init()
{
	WSADATA wsaData;
	if (WSAStartup(0x202, &wsaData) == SOCKET_ERROR)
		throw std::runtime_error(Constants::Errors::getDescription(Constants::Errors::Code::WSAInitError));
}
