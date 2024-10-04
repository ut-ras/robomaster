#pragma once

#include "drivers.hpp"
#include "tap/control/command.hpp"
#include "tap/control/subsystem.hpp"
#include "tap/communication/sensors/buzzer/buzzer.hpp"

using namespace tap::control;

namespace subsystems::control
{
class ClientDisplaySubsystem : public Subsystem
{
public:
	ClientDisplaySubsystem(tap::Drivers*);
	virtual ~ClientDisplaySubsystem() {}

	void initialize() override {};

	const char* getName() override { return "client display"; }
};
}