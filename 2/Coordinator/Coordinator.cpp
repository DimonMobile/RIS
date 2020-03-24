#include <iostream>
#include <queue>
#include <algorithm>

#include <WinSock2.h>

#define forever for (;;)

struct CA
{
    char ipaddr[15];
    char resource[20];
    enum STATUS
    {
        NOINIT = 0,
        INIT,
        ENTER,
        WAIT
    } status;
};

CA InitCA(const char ipaddr[15], const char resource[20])
{
    CA ca;
    memcpy(ca.ipaddr, ipaddr, sizeof(ipaddr));
    memcpy(ca.resource, resource, sizeof(resource));
    ca.status = CA::INIT;

    std::cout << "Section initialized" << std::endl;
    return ca;
}

bool EnterCA(CA& ca)
{
    std::cout << "Entering to section...";
    if (ca.status == CA::INIT || ca.status == CA::WAIT)
    {
        ca.status = CA::ENTER;
        std::cout << "OK" << std::endl;
        return true;
    }
    std::cout << "FAIL" << std::endl;
    return false;
}

bool LeaveCA(CA& ca)
{
    std::cout << "Leaving the section...";
    if (ca.status == CA::ENTER)
    {
        ca.status = CA::WAIT;
        std::cout << "OK" << std::endl;
        return true;
    }

    return false;
}

bool CloseCA(CA& ca)
{
    memset(&ca, 0, sizeof(ca));
    std::cout << "Section uninitialized" << std::endl;
    return true;
}

int main()
{
    WSADATA wsaData;
    if (WSAStartup(0x202, &wsaData) == SOCKET_ERROR)
        std::cerr << "wsastartup error" << std::endl;

    SOCKET server = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    unsigned long nonBlock = 1;
    ioctlsocket(server, FIONBIO, &nonBlock);

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(7777);
    addr.sin_addr.S_un.S_addr = INADDR_ANY;

    if (bind(server, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
        std::cerr << "Bind error" << std::endl;

    CA ca = InitCA("127.0.0.1", "rname");
    
    fd_set readSet, writeSet;

    std::queue<std::pair<std::string, unsigned short>> clients;

    forever
    {
        FD_ZERO(&readSet);
        FD_ZERO(&writeSet);
        FD_SET(server, &readSet);

        timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        
        if (select(0, &readSet, nullptr, nullptr, &timeout) == 1) // newdata
        {
            char recvBuf[64];
            sockaddr_in clientAddr;
            int clientAddrLen = sizeof(clientAddr);
            int recieved = recvfrom(server, recvBuf, sizeof(recvBuf), 0, (sockaddr*)&clientAddr, &clientAddrLen);

            std::pair<std::string, unsigned short> remotePair(inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
            if (!strcmp(recvBuf, "enter"))
            {
                clients.push(remotePair);
                if (EnterCA(ca))
                    sendto(server, "OK", 3, 0, (sockaddr*)&clientAddr, sizeof(clientAddr));
            }
            else if (!strcmp(recvBuf, "leave"))
            {
                if (!clients.empty() && clients.front() == remotePair)
                {
                    if (LeaveCA(ca)) // here notify new client in queue
                    {
                        clients.pop();
                        if (!clients.empty())
                        {
                            EnterCA(ca);
                            auto client = clients.front();
                            sockaddr_in clientaddr;
                            clientaddr.sin_family = AF_INET;
                            clientaddr.sin_addr.S_un.S_addr = inet_addr(client.first.c_str());
                            clientaddr.sin_port = htons(client.second);
                            sendto(server, "OK", 3, 0, (sockaddr*)&clientaddr, sizeof(clientaddr));
                        }
                    }
                }
            }
        }
    }
    CloseCA(ca);
    closesocket(server);
    WSACleanup();
}
