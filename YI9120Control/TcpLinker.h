#pragma once

#include <vector>
#include <string>
#include <functional>
#include <chrono>

#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")

const std::chrono::seconds keepalive_second{5};

class TcpLinker
{
public:
    explicit TcpLinker(std::function<void(std::vector<unsigned char> &&)> receive);

    virtual ~TcpLinker();

    TcpLinker(const TcpLinker &) = delete;

    bool isConnected() const
    {
        return sock != INVALID_SOCKET;
    }

public:
    bool connect(const std::string & ipaddress, int port);

    void send(int len, void *data);

    void close();

    void keepAlive();

private:
    void thread_fn();

private:
    SOCKET sock{INVALID_SOCKET};

    std::function<void(std::vector<unsigned char> &&)> receive_fn;

    std::chrono::steady_clock::time_point keepAlive_pt;
};