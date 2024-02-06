#pragma once

#include "tap/control/command.hpp"
#include "tap/communication/serial/ref_serial_transmitter.hpp"
#include "tap/communication/serial/ref_serial_data.hpp"
#include "modm/processing/protothread.hpp"
#include "modm/processing/resumable.hpp"

#include "drivers.hpp"

#include "control/client-display/client_display_subsystem.hpp"

using namespace tap::control;
using namespace tap::communication::serial;

using subsystems::control::ClientDisplaySubsystem;

class BeybladeIndicator : protected modm::Resumable<2>
{
private:
	const uint8_t* graphicName = (uint8_t*)"\x00\x00\x01";
	RefSerialTransmitter &refSerialTransmitter;
	RefSerialData::Tx::Graphic5Message msg;

public:
	BeybladeIndicator(RefSerialTransmitter refSerialTransmitter)
	: refSerialTransmitter(refSerialTransmitter)
	{}

	modm::ResumableResult<bool> sendInitialGraphics();
	modm::ResumableResult<bool> update();
	
	void initialize();
};

namespace commands
{
class CommandClientDisplay : Command, modm::pt::Protothread
{
private:
	src::Drivers &drivers;
	RefSerialTransmitter refSerialTransmitter;
	BeybladeIndicator beybladeIndicator;
	bool restarting = true;

	void restartHud();
public:
	CommandClientDisplay(
		src::Drivers drivers,
		ClientDisplaySubsystem *clientDisplay) // temporarily using this to start this command
		: Command(),
		drivers(drivers),
		refSerialTransmitter(&drivers),
		beybladeIndicator(refSerialTransmitter)
	{
		addSubsystemRequirement(clientDisplay);
	}
	
	bool run();
	
	const char *getName() const override { return "client display"; }
	void initialize() override
	{
		this->restarting = true;
	};

	void execute() override
	{
		run();
	}
	void end(bool) override {}
	bool isFinished() const override { return false; }
};
}
