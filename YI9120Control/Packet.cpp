#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include "Packet.h"

#include <algorithm>
#include <string.h>

YL9020_UVD_INFORM Packet::getInform()
{
    const std::lock_guard<std::recursive_mutex> lock(packet_mutex);
    return inform;
}

YL9020_UVD_CONFIG Packet::getConfig()
{
    const std::lock_guard<std::recursive_mutex> lock(packet_mutex);
    return config;
}

void Packet::setConfig(const YL9020_UVD_CONFIG &conf)
{
    const std::lock_guard<std::recursive_mutex> lock(packet_mutex);
    config = conf;
}

YL9020_UVD_SETUP Packet::getSetup()
{
    const std::lock_guard<std::recursive_mutex> lock(packet_mutex);
    return setup;
}

void Packet::setSetup(const YL9020_UVD_SETUP &set)
{
    const std::lock_guard<std::recursive_mutex> lock(packet_mutex);
    setup = set;
}

YL9020_UVD_STATE Packet::getState()
{
    const std::lock_guard<std::recursive_mutex> lock(packet_mutex);
    return state;
}

void Packet::receive(bytes &&stream)
{
    const std::lock_guard<std::recursive_mutex> lock(packet_mutex);
    if(stream.empty())
    {
        receivePacket(0);   // disconnected remotely
    }
    else
    {
        std::copy(stream.begin(), stream.end(), std::back_inserter(buffer));
        parse();
    }
}

void Packet::parse()
{
    while (buffer.size() >= sizeof(header))
    {
        header *h = reinterpret_cast<header *>(buffer.data());

        // packet validation
        if (h->length >= 1024 || h->length < sizeof(header) || (h->code & 0xfff00) != PACKCODE_YL9020_INFORM)
        {
            buffer.erase(buffer.begin());
            continue;
        }
        if ((int)buffer.size() < h->length)
            break;

        // parsing a packet
        if (h->size > 0 && h->length == h->size + sizeof(header))
        {
            switch (h->code)
            {
            case PACKCODE_YL9020_INFORM:    parsePacket(h, inform); break;
            case PACKCODE_YL9020_CONFIG:    parsePacket(h, config); break;
            case PACKCODE_YL9020_SETUP:     parsePacket(h, setup); break;
            case PACKCODE_YL9020_STATE:     parsePacket(h, state); break;
            case PACKCODE_YL9020_ACQDATA:   parsePacket(h, acq_data); break;
            case PACKCODE_YL9020_SELFMSG:   parsePacket(h, selfmsg); break;
            }

            receivePacket(h->code);
        }

        // cleaning buffer
        if (buffer.size() == h->length)
            buffer.clear();
        else
            buffer.erase(buffer.begin(), buffer.begin() + h->length);
    }
}