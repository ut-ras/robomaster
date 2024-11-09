#pragma once

#include "tap/communication/sensors/buzzer/buzzer.hpp"
#include "tap/control/command.hpp"
#include "tap/control/subsystem.hpp"

#include "drivers.hpp"

namespace subsystems::control
{
class ClientDisplaySubsystem : public tap::control::Subsystem
{
public:
    ClientDisplaySubsystem(tap::Drivers*);

    void initialize() override;

    void refresh() override;

    void runHardwareTests() override;

    const char* getName() override { return "Client Display subsystem"; }
};
}  // namespace subsystems::control