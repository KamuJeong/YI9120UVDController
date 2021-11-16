#pragma once

#include <mutex>
#include <vector>
#include <iterator>

#include "PacketStructureFor9120.h"

using bytes = std::vector<unsigned char>;

class Packet
{
public:
    YL9020_UVD_INFORM getInform();

    YL9020_UVD_CONFIG getConfig();

    void setConfig(const YL9020_UVD_CONFIG & conf);

    YL9020_UVD_SETUP getSetup();
    
    void setSetup(const YL9020_UVD_SETUP & set);

    YL9020_UVD_STATE getState();

protected:
    template<int C, int I, typename T>
    bytes prepareSetPacket(const T& data, int len = 0)
    {
        len = len == 0? sizeof(T) : len;

        bytes transmit_buffer;
        
        header packet{ len + (int)sizeof(header), 0, C, I, 0, len, };
        transmit_buffer.insert(transmit_buffer.end(), (unsigned char*)&packet, (unsigned char*)&packet + sizeof(packet));
        transmit_buffer.insert(transmit_buffer.end(), (unsigned char*)&data, (unsigned char*)&data + len);

        return transmit_buffer;
    }

    template<int C, int I, typename T>
    bytes prepareRequestPacket(const T& data)
    {
        bytes transmit_buffer;
        
        header packet{ sizeof(header), 0, C, I, 0, sizeof(T), };
        transmit_buffer.insert(transmit_buffer.end(), (unsigned char*)&packet, (unsigned char*)&packet + sizeof(packet));

        return transmit_buffer;
    }


    void receive(bytes&& stream);

    virtual void receivePacket(int code) {};

private:
    void parse();

    template<typename T>
    void parsePacket(header *h, T& data)
    {
        memcpy((char *)&data + h->offset, h + 1, 
                                    max(0, min(h->size, (int)sizeof(data) - h->offset)));
    }
private:
    std::recursive_mutex packet_mutex;
    bytes buffer;

    YL9020_UVD_INFORM inform;
    YL9020_UVD_CONFIG config;
    YL9020_UVD_SETUP setup;
    YL9020_UVD_STATE state;

protected:
    YL9020_UVD_ACQDATA acq_data;
    YL9020_UVD_SELFMSG selfmsg;
};