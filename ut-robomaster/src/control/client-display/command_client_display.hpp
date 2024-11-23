#pragma once

#include <vector>
#include "tap/communication/sensors/buzzer/buzzer.hpp"
#include "tap/communication/serial/ref_serial_data.hpp"
#include "tap/communication/serial/ref_serial_transmitter.hpp"
#include "tap/control/command.hpp"

#include "control/client-display/client_display_subsystem.hpp"
#include "control/client-display/graphics/graphic_abstract.hpp"

#include "modm/processing/protothread.hpp"
#include "modm/processing/resumable.hpp"
#include "subsystems/flywheel/flywheel_subsystem.hpp"

#include "drivers.hpp"

using namespace tap::control;
using namespace tap::communication::serial;

using subsystems::control::ClientDisplaySubsystem;
using subsystems::flywheel::FlywheelSubsystem;

namespace commands
{
class CommandClientDisplay : public Command, modm::pt::Protothread
{
public:
    CommandClientDisplay(src::Drivers *drivers, ClientDisplaySubsystem* client)
        : Command(),
          drivers(drivers),
        //   refSerialTransmitter(drivers),
          client(client)
    {
        addSubsystemRequirement(client);
    }

    bool run();

    void initialize() override;
    void execute() override;
    void end(bool) override;
    bool isFinished() const override;
    const char *getName() const override { return "client display"; }

	//Plan to make a new class, called graphic
	//in it, there contains virtual functions
	//initialize, run (PT threads)
	//within each of the derived classes, there would be a refSerialTransmitter
	//also the corresponding subsystem to get the proper infomation

	//then only one command to call on the list of derived graphics, initialize all of them, run all of them
	//I think it should be a vector of classes?

	//1. Make Graphic class, derived class
	//2. Create a vector of these classes within the subsystem initialize (not command), revert this command into subsystem with client_display()
	//3. Within this command, it should now have complete vector of the graphics, and corresponding calls the initalize, and run functions (execute)

	//Possibility to create a command to hide graphics?

private:

    src::Drivers *drivers;
    ClientDisplaySubsystem* client;
    graphic::graphic_abstract* graphic;
    // RefSerialTransmitter refSerialTransmitter;
    // const uint8_t graphicId[3] = {0, 0, 1};  // 3 byte identifier for this graphic element
    // RefSerialData::Tx::Graphic1Message msg;

};
}  // namespace commands