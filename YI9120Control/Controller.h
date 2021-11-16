#pragma once

#include <functional>

#include "Packet.h"
#include "TcpLinker.h"

class Controller : public Packet
{
public:
    explicit Controller(std::function<void(float, int, const float *)> acquire = nullptr);

    virtual ~Controller();

public:
    bool connect(const std::string &ipaddress = "10.10.10.20", int port = 4242);

    bool isConnected() const;

    bool isReady();

    bool isRunning();

    void turnOnD2Lamp(bool on);

    void turnOnWLamp(bool on);

    const int getWavelength();

    void setWavelength(int wave);

    const float getAbsorbance();

    void setAutoZero();

    void startAcquisition();

    void stopAcquisition();

    void close();

protected:
    virtual void receivePacket(int code) override;

    void sendCommand(int cmd);

    void sendSetup(const YL9020_UVD_SETUP & setup);

    void sendConfig(const YL9020_UVD_CONFIG & config);

private:
    TcpLinker * pLinker{};
    bool is_connected{};
    std::function<void(float, int, const float *)> acquire_fn;
};