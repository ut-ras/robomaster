#ifdef TARGET_STANDARD

#include "tap/communication/gpio/leds.hpp"
#include "tap/control/command_mapper.hpp"
#include "tap/control/hold_command_mapping.hpp"
#include "tap/control/hold_repeat_command_mapping.hpp"
#include "tap/control/press_command_mapping.hpp"
#include "tap/control/toggle_command_mapping.hpp"

#include "drivers.hpp"
#include "drivers_singleton.hpp"

// Agitator includes ----------------------------------------
#include "subsystems/agitator/agitator_subsystem.hpp"
#include "subsystems/agitator/command_rotate_agitator.hpp"

using namespace tap::control;
using namespace tap::communication::serial;

using namespace subsystems::agitator;

using namespace commands;

/*
 * NOTE: We are using the DoNotUse_getDrivers() function here
 *      because this file defines all subsystems and command
 *      and thus we must pass in the single statically allocated
 *      Drivers class to all of these objects.
 */
src::driversFunc drivers = src::DoNotUse_getDrivers;

namespace standard_control
{
// Subsystem definitions ---------------------------------------------------------
AgitatorSubsystem agitator(drivers());

// Command definitions -----------------------------------------------------------
CommandRotateAgitator rotateAgitatorCommand(drivers(), &agitator);

// Keyboard mappings ------------------------------------------------------------

// Register subsystems here -----------------------------------------------
void registerStandardSubsystems(src::Drivers *drivers)
{
    drivers->commandScheduler.registerSubsystem(&agitator);
}

// Initialize subsystems here ---------------------------------------------
void initializeSubsystems() { agitator.initialize(); }

// Set default commands here -----------------------------------------------
void setDefaultCommands(src::Drivers *) { agitator.setDefaultCommand(&rotateAgitatorCommand); }

void runStartupCommands(src::Drivers *) {}

// Register IO mappings here -----------------------------------------------
void registerMappings(src::Drivers *drivers)
{
    // Keyboard mappings ------------------------------------------------------------

    // Joystick mappings ------------------------------------------------------------
}
}  // namespace standard_control

namespace control
{
void initSubsystemCommands(src::Drivers *drivers)
{
    standard_control::initializeSubsystems();
    standard_control::registerStandardSubsystems(drivers);
    standard_control::setDefaultCommands(drivers);
    standard_control::runStartupCommands(drivers);
    standard_control::registerMappings(drivers);
}
}  // namespace control
#endif