#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier

#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

#include "TcpLinker.h"
#include <iostream>
#include <thread>

TcpLinker::TcpLinker(std::function<void(std::vector<unsigned char> &&)> receive)
{
    receive_fn = receive;

    WSADATA wsadata;
    if (SOCKET_ERROR == WSAStartup(0x0202, &wsadata))
    {
        std::cerr << "WSAStartup() error" << std::endl;
    }
}

TcpLinker::~TcpLinker()
{
    close();

    if (SOCKET_ERROR == WSACleanup())
    {
        std::cout << "WSACleanup() error" << std::endl;
    }
}

void TcpLinker::thread_fn()
{
    while(true)
    {
        u_long len;
        if(::ioctlsocket(sock, FIONREAD, &len) == SOCKET_ERROR)
        {
            if(WSAGetLastError() == WSAENETDOWN)
                break;
        }

        if(len > 0)
        {
            std::vector<unsigned char> data(len);
            int result = ::recv(sock, (char *)data.data(), len,  0);
            if(result <= 0)
                break;

            if(receive_fn != nullptr)
            {
                if(result < (int)len)    data.resize(result);
                receive_fn(std::move(data)); 
            }
        }

        if(std::chrono::steady_clock::now() - keepAlive_pt > keepalive_second)
        {
            break;
        }

        std::this_thread::sleep_for(std::chrono::microseconds(100000));
    }

    if (receive_fn != nullptr)
    {
        receive_fn(std::move(std::vector<unsigned char>()));
    }

    close();
}

bool TcpLinker::connect(const std::string &ipaddress, int port)
{
    if (isConnected())
        return false;

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock != INVALID_SOCKET)
    {
        SOCKADDR_IN target;                                    //Socket address information
        target.sin_family = AF_INET;                           // address family Internet
        target.sin_port = htons(port);                         //Port to connect on
        target.sin_addr.s_addr = inet_addr(ipaddress.c_str());                 //Target IP

        if (::connect(sock, (SOCKADDR*)&target, sizeof(target)) != SOCKET_ERROR)
        {
            keepAlive();
            std::thread t{ &TcpLinker::thread_fn, this };
            t.detach();

            return true;
        }
        else
        {
            std::cerr << "fail in connection to " << ipaddress << ":" << port << std::endl;
            close();
        }
    }
    return false;
}

void TcpLinker::send(int len, void *data)
{
    ::send(sock, (const char *)data, len, 0);
}

void TcpLinker::close()
{
    if (sock != INVALID_SOCKET)
        closesocket(sock);

    sock = INVALID_SOCKET;
}

void TcpLinker::keepAlive()
{
    keepAlive_pt = std::chrono::steady_clock::now();
}