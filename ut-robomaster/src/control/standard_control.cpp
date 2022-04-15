#ifndef ENV_UNIT_TESTS

#ifdef TARGET_STANDARD

#include "drivers.hpp"
#include "drivers_singleton.hpp"

#include "tap/control/command_mapper.hpp"
#include "tap/control/hold_repeat_command_mapping.hpp"
#include "tap/control/hold_command_mapping.hpp"
#include "tap/control/press_command_mapping.hpp"
#include "tap/drivers.hpp"


#include "agitator/agitator_rotate_command.hpp"
#include "agitator/agitator_reverse_command.hpp"
#include "agitator/agitator_subsystem.hpp"

#include "flywheel/flywheel_subsystem.hpp"
#include "flywheel/flywheel_on_command.hpp"

#include "chassis/chassis_subsystem.hpp"
#include "chassis/chassis_drive_command.hpp"

#include "tap/communication/gpio/leds.hpp"

using namespace tap::control;
using namespace control;
using namespace tap::communication::serial;

/*
 * NOTE: We are using the DoNotUse_getDrivers() function here
 *      because this file defines all subsystems and command
 *      and thus we must pass in the single statically allocated
 *      Drivers class to all of these objects.
 */
src::driversFunc drivers = src::DoNotUse_getDrivers;
tap::gpio::Leds led;

namespace standard_control
{
/* define subsystems --------------------------------------------------------*/
control::agitator::AgitatorSubsystem theAgitator(drivers());
control::flywheel::FlywheelSubsystem theFlywheel(drivers());
control::chassis::ChassisSubsystem theChassis(drivers());

/* define commands ----------------------------------------------------------*/
control::agitator::AgitatorRotateCommand rotateCommand(&theAgitator);
control::agitator::AgitatorReverseCommand reverseCommand(&theAgitator);
control::flywheel::FlywheelOnCommand flywheelCommand(&theFlywheel);
control::chassis::ChassisDriveCommand chassisDriveCommand(drivers(), &theChassis);

/* define command mappings --------------------------------------------------*/
HoldRepeatCommandMapping rightSwitchUp(
    drivers(),
    {&rotateCommand},
    RemoteMapState(Remote::Switch::RIGHT_SWITCH,Remote::SwitchState::UP), true, -1);

HoldRepeatCommandMapping rightSwitchDown(
    drivers(),
    {&reverseCommand},
    RemoteMapState(Remote::Switch::RIGHT_SWITCH, Remote::SwitchState::DOWN), true, -1);

HoldRepeatCommandMapping leftSwitchUp(
    drivers(),
    {&flywheelCommand},
    RemoteMapState(Remote::Switch::LEFT_SWITCH, Remote::SwitchState::UP), true, -1);



/* register subsystems here -------------------------------------------------*/
void registerStandardSubsystems(tap::Drivers *drivers)
{
    drivers->commandScheduler.registerSubsystem(&theAgitator);
    drivers->commandScheduler.registerSubsystem(&theFlywheel);
    drivers->commandScheduler.registerSubsystem(&theChassis);
}

/* initialize subsystems ----------------------------------------------------*/
void initializeSubsystems() { 
    theAgitator.initialize();
    theFlywheel.initialize();
    theChassis.initialize();
}

/* set any default commands to subsystems here ------------------------------*/
void setDefaultStandardCommands(tap::Drivers *) 
{
    theChassis.setDefaultCommand(&chassisDriveCommand);
}

/* add any starting commands to the scheduler here --------------------------*/
void startStandardCommands(tap::Drivers *) {}

/* register io mappings here ------------------------------------------------*/
void registerStandardIoMappings(tap::Drivers *drivers)
{
    drivers->commandMapper.addMap(&rightSwitchUp);
    drivers->commandMapper.addMap(&rightSwitchDown);
    drivers->commandMapper.addMap(&leftSwitchUp);
}
} // namespace standard_control

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
}

#endif
#endif