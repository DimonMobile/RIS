#include <iostream>
#include <fstream>
#include <WinSock2.h>

int main()
{
    WSADATA wsaData;
    if (WSAStartup(0x202, &wsaData) == SOCKET_ERROR)
        std::cerr << "WSAStartup error" << std::endl;

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(7777);
    addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    const char* enterData = "enter";
    std::cout << "Entering critical section" << std::endl;
    sendto(sock, enterData, strlen(enterData) + 1, 0, (sockaddr*)&addr, sizeof(addr));
    char recvBuf[64];

    sockaddr_in from;
    int len = sizeof(sockaddr_in);
    recvfrom(sock, recvBuf, 64, 0, (sockaddr*)&from, &len);
    if (!strcmp(recvBuf, "OK"))
        std::cout << "Recieved OK" << std::endl;
    else
        std::cout << "Recieved trash" << std::endl;
    Sleep(200);

    std::ofstream stream("output.txt", std::ios_base::app);
    stream << "One more line of text, yeah" << std::endl;
    stream.close();

    const char* leaveData = "leave";
    sendto(sock, leaveData, strlen(leaveData) + 1, 0, (sockaddr*)&addr, sizeof(addr));

    closesocket(sock);
    WSACleanup(); 
}