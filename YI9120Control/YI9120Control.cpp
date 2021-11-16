#include "pch.h"
#include "Controller.h"
#include "YI9120Control.h"

#include <unordered_map>
#include <iostream>

static int device_seq = 0;
static std::unordered_map<int, std::unique_ptr<Controller>> device_map{};

int createUVD(fn_acquire fn)
{
	int id = ++device_seq;
	device_map.emplace(std::make_pair(id, std::make_unique<Controller>(fn)));
	return id;
}

bool connectUVD(int device, const char* ip, int port)
{
	try
	{
		return device_map.at(device)->connect(ip, port);
	}
	catch (std::out_of_range e)
	{
		std::cerr << e.what() << std::endl;
	}
	return false;
}

bool isUVDConnected(int device)
{
	try
	{
		return device_map.at(device)->isConnected();
	}
	catch (std::out_of_range e)
	{
		std::cerr << e.what() << std::endl;
	}
	return false;
}

bool isUVDReady(int device)
{
	try
	{
		return device_map.at(device)->isReady();
	}
	catch (std::out_of_range e)
	{
		std::cerr << e.what() << std::endl;
	}
	return false;
}

bool isUVDRunning(int device)
{
	try
	{
		return device_map.at(device)->isRunning();
	}
	catch (std::out_of_range e)
	{
		std::cerr << e.what() << std::endl;
	}
	return false;
}

void closeUVD(int device)
{
	device_map.erase(device);
}

void turnOnD2Lamp(int device, bool on)
{
	try
	{
		device_map.at(device)->turnOnD2Lamp(on);
	}
	catch (std::out_of_range e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void turnOnWLamp(int device, bool on)
{
	try
	{
		device_map.at(device)->turnOnWLamp(on);
	}
	catch (std::out_of_range e)
	{
		std::cerr << e.what() << std::endl;
	}
}

const int getWavelength(int device)
{
	try
	{
		return device_map.at(device)->getWavelength();
	}
	catch (std::out_of_range e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}

void setWavelength(int device, int wave)
{
	try
	{
		device_map.at(device)->setWavelength(wave);
	}
	catch (std::out_of_range e)
	{
		std::cerr << e.what() << std::endl;
	}
}

const float getAbsorbance(int device)
{
	try
	{
		return device_map.at(device)->getAbsorbance();
	}
	catch (std::out_of_range e)
	{
		std::cerr << e.what() << std::endl;
	}
	return NAN;
}

void setAutoZero(int device)
{
	try
	{
		device_map.at(device)->setAutoZero();
	}
	catch (std::out_of_range e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void startAcquisition(int device)
{
	try
	{
		device_map.at(device)->startAcquisition();
	}
	catch (std::out_of_range e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void stopAcquisition(int device)
{
	try
	{
		device_map.at(device)->stopAcquisition();
	}
	catch (std::out_of_range e)
	{
		std::cerr << e.what() << std::endl;
	}
}