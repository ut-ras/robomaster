#include "tap/control/command_mapper.hpp"
#include "tap/control/hold_command_mapping.hpp"
#include "tap/control/hold_repeat_command_mapping.hpp"
#include "tap/control/press_command_mapping.hpp"
#include "tap/drivers.hpp"

#include "subsystems/test/test_command.hpp"

#include "drivers.hpp"
#include "drivers_singleton.hpp"

using namespace tap::control;
using namespace tap::communication::serial;
using namespace subsystems;

/*
 * NOTE: We are using the DoNotUse_getDrivers() function here
 *      because this file defines all subsystems and command
 *      and thus we must pass in the single statically allocated
 *      Drivers class to all of these objects.
 */
src::driversFunc drivers = src::DoNotUse_getDrivers;

namespace standard_control
{
/* define subsystems --------------------------------------------------------*/
test::TestSubsystem theTest(drivers());

/* define commands ----------------------------------------------------------*/
test::TestCommand testCommand(&theTest, drivers());

/* define command mappings --------------------------------------------------*/
// HoldCommandMapping testMoveChassis(
//     drivers(),
//     {&setKinematicsCommand},
//     RemoteMapState({Remote::Key::F}));

/* register subsystems here -------------------------------------------------*/
void registerStandardSubsystems(tap::Drivers *drivers)
{
    drivers->commandScheduler.registerSubsystem(&theTest);
}

/* initialize subsystems ----------------------------------------------------*/
void initializeSubsystems() { theTest.initialize(); }

/* set any default commands to subsystems here ------------------------------*/
void setDefaultStandardCommands(tap::Drivers *) { theTest.setDefaultCommand(&testCommand); }

/* add any starting commands to the scheduler here --------------------------*/
void startStandardCommands(tap::Drivers *) {}

/* register io mappings here ------------------------------------------------*/
void registerStandardIoMappings(tap::Drivers *drivers)
{
    // drivers->commandMapper.addMap(&testMoveChassis);
}
}  // namespace standard_control

namespace control
{
void initSubsystemCommands(tap::Drivers *drivers)
{
    standard_control::initializeSubsystems();
    standard_control::registerStandardSubsystems(drivers);
    standard_control::setDefaultStandardCommands(drivers);
    standard_control::startStandardCommands(drivers);
    standard_control::registerStandardIoMappings(drivers);
}
}  // namespace control