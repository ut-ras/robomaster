#pragma once

#include "tap/control/command.hpp"

#include "client_display_subsystem.hpp"
#include "drivers.hpp"

using namespace tap::control;
using namespace tap::communication::serial;

using subsystems::hud::ClientDisplaySubsystem;

namespace commands
{
class CommandClientDisplayReset : public Command
{
public:
    CommandClientDisplayReset(src::Drivers* drivers, ClientDisplaySubsystem* client)
        : Command(),
          drivers(drivers),
          client(client)
    {
        addSubsystemRequirement(client);
    }

    void initialize() override;
    void execute() override;
    void end(bool) override;
    bool isFinished() const override;
    const char* getName() const override { return "client display reset"; }

private:
    src::Drivers* drivers;
    ClientDisplaySubsystem* client;
};
}  // namespace commands