#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include "Controller.h"
#include <iostream>
#include <thread>
#include <chrono>

Controller::Controller(std::function<void(float, int, const float*)> acquire)
	: acquire_fn{ acquire }
{
	pLinker = new TcpLinker([this](std::vector<unsigned char>&& s)
		{
			receive(std::move(s));
		});
}

Controller::~Controller()
{
	pLinker->close();
	delete pLinker;
}

bool Controller::connect(const std::string& ipaddress, int port)
{
	if (!pLinker->isConnected())
	{
		pLinker->connect(ipaddress, port);

		std::this_thread::sleep_for(std::chrono::microseconds(250000));

		auto packet = prepareSetPacket<PACKCODE_YL9020_INFORM, 0>(getInform());
		pLinker->send((int)packet.size(), packet.data());

		while (!isConnected() && pLinker->isConnected())
			std::this_thread::sleep_for(std::chrono::microseconds(100000));

	}
	return isConnected();
}


bool Controller::isConnected() const
{
	return is_connected;
}

bool Controller::isReady()
{
	return isConnected() && getState().btState == YL9020_UVD_STATE::XUVDStateReady;
}

bool Controller::isRunning()
{
	return isConnected() && getState().btState == YL9020_UVD_STATE::XUVDStateRun;
}

void Controller::close()
{
	pLinker->close();
	is_connected = false;
}

void Controller::turnOnD2Lamp(bool on)
{
	auto setup = getSetup();
	setup.btCommand = 0;
	setup.InitEvent.btD2Lamp = on ? 1 : 0;
	sendSetup(setup);
}

void Controller::turnOnWLamp(bool on)
{
	auto setup = getSetup();
	setup.btCommand = 0;
	setup.InitEvent.btWLamp = on ? 1 : 0;
	sendSetup(setup);
}

const int Controller::getWavelength()
{
	return getState().sWavelengthA;
}

void Controller::setWavelength(int wave)
{
	if (wave >= 190 && wave <= 1024)
	{
		auto setup = getSetup();
		setup.btCommand = 0;
		setup.nProgramCount = 0;
		setup.InitEvent.sWavelengthA = setup.InitEvent.sWavelengthB = wave;
		sendSetup(setup);
	}
}

const float Controller::getAbsorbance()
{
	return getState().SignalA.fAbsorbance;
}

void Controller::setAutoZero()
{
	sendCommand(YL9020_UVD_SETUP::XUVDCmdAutoZero);
}

void Controller::startAcquisition()
{
	sendCommand(YL9020_UVD_SETUP::XUVDCmdRun);
}

void Controller::stopAcquisition()
{
	sendCommand(YL9020_UVD_SETUP::XUVDCmdStop);
	sendCommand(YL9020_UVD_SETUP::XUVDCmdInit);
}

void Controller::receivePacket(int code)
{
	switch (code)
	{
	case 0:
		is_connected = false;
		std::cout << "disconnected" << std::endl;
		break;

	case PACKCODE_YL9020_STATE:
		if (!is_connected)
		{
			is_connected = true;

			auto packet = prepareRequestPacket<PACKCODE_YL9020_INFORM, 0>(getInform());
			pLinker->send((int)packet.size(), packet.data());
			packet = prepareRequestPacket<PACKCODE_YL9020_CONFIG, 0>(getConfig());
			pLinker->send((int)packet.size(), packet.data());
			packet = prepareRequestPacket<PACKCODE_YL9020_SETUP, 0>(getSetup());
			pLinker->send((int)packet.size(), packet.data());

			std::cout << "connection ok!" << std::endl;
		}
		pLinker->keepAlive();
		break;
	case PACKCODE_YL9020_ACQDATA:
		if (acquire_fn != nullptr)
		{
			std::vector<float> absorbances;
			for (auto s : acq_data.SignalA)
				absorbances.push_back(s.fAbsorbance);

			acquire_fn(acq_data.index, (int)absorbances.size(), absorbances.data());
		}
		break;
	}

}

void Controller::sendCommand(int cmd)
{
	auto packet = prepareSetPacket<PACKCODE_YL9020_SETUP, 0>(cmd, 1);
	pLinker->send((int)packet.size(), packet.data());
}

void Controller::sendSetup(const YL9020_UVD_SETUP& setup)
{
	setSetup(setup);

	auto packet = prepareSetPacket<PACKCODE_YL9020_SETUP, 0>(setup);
	pLinker->send((int)packet.size(), packet.data());

	sendCommand(YL9020_UVD_SETUP::XUVDCmdInit);
}

void Controller::sendConfig(const YL9020_UVD_CONFIG& config)
{
	auto packet = prepareSetPacket<PACKCODE_YL9020_CONFIG, 0>(config);
	pLinker->send((int)packet.size(), packet.data());

	packet = prepareRequestPacket<PACKCODE_YL9020_CONFIG, 0>(config);
	pLinker->send((int)packet.size(), packet.data());
}