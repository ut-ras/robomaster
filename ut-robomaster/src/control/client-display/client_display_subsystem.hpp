#pragma once

#include "drivers.hpp"
#include "tap/control/command.hpp"
#include "tap/control/subsystem.hpp"

using namespace tap::control;

namespace subsystems::control
{
class ClientDisplaySubsystem : public tap::control::Subsystem
{
public:
	ClientDisplaySubsystem(tap::Drivers*);
	virtual ~ClientDisplaySubsystem() {}

	const char* getName() override { return "client display"; }
};
}
