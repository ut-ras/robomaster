#ifdef TARGET_STANDARD

#include "tap/communication/gpio/leds.hpp"
#include "tap/control/command_mapper.hpp"
#include "tap/control/hold_command_mapping.hpp"
#include "tap/control/hold_repeat_command_mapping.hpp"
#include "tap/control/press_command_mapping.hpp"
#include "tap/control/toggle_command_mapping.hpp"

#include "drivers.hpp"
#include "drivers_singleton.hpp"

// Chassis includes ----------------------------------------
#include "subsystems/chassis/chassis_subsystem.hpp"
#include "subsystems/chassis/command_beyblade_chassis_keyboard.hpp"
#include "subsystems/chassis/command_move_chassis_joystick.hpp"
#include "subsystems/chassis/command_move_chassis_keyboard.hpp"
#include "subsystems/chassis/command_move_chassis_turret_relative_joystick.hpp"

// Agitator includes ----------------------------------------
#include "subsystems/agitator/agitator_subsystem.hpp"
#include "subsystems/agitator/command_rotate_agitator_continuous.hpp"
#include "subsystems/agitator/command_unjam_agitator.hpp"

// Flywheel includes ----------------------------------------
#include "subsystems/flywheel/command_flywheel_off.hpp"
#include "subsystems/flywheel/command_rotate_flywheel.hpp"
#include "subsystems/flywheel/flywheel_subsystem.hpp"

// Turret includes ------------------------------------------
#include "subsystems/odometry/odometry_subsystem.hpp"
#include "subsystems/turret/command_move_turret_aimbot.hpp"
#include "subsystems/turret/command_move_turret_joystick.hpp"
#include "subsystems/turret/command_move_turret_mouse.hpp"
#include "subsystems/turret/turret_subsystem.hpp"

using namespace tap::control;
using namespace tap::communication::serial;

using namespace subsystems::chassis;
using namespace subsystems::agitator;
using namespace subsystems::flywheel;
using namespace subsystems::turret;
using namespace subsystems::odometry;

using namespace commands;

/*
 * NOTE: We are using the DoNotUse_getDrivers() function here
 *      because this file defines all subsystems and command
 *      and thus we must pass in the single statically allocated
 *      Drivers class to all of these objects.
 */
src::driversFunc drivers = src::DoNotUse_getDrivers;

namespace sentry_control
{

// Subsystem definitions ---------------------------------------------------------
ChassisSubsystem chassis(drivers());
AgitatorSubsystem agitator(drivers());
FlywheelSubsystem flywheel(drivers());
TurretSubsystem turret(drivers());
OdometrySubsystem odometry(drivers(), &chassis, &turret);

// Command definitions -----------------------------------------------------------
CommandMoveTurretAimbot moveTurretCommandAimbot(drivers(), &turret);

// Register subsystems here -----------------------------------------------
void registerStandardSubsystems(src::Drivers *drivers)
{
    drivers->commandScheduler.registerSubsystem(&chassis);
    drivers->commandScheduler.registerSubsystem(&agitator);
    drivers->commandScheduler.registerSubsystem(&flywheel);
    drivers->commandScheduler.registerSubsystem(&turret);
    drivers->commandScheduler.registerSubsystem(&odometry);
}

// Initialize subsystems here ---------------------------------------------
void initializeSubsystems()
{
    chassis.initialize();
    agitator.initialize();
    flywheel.initialize();
    turret.initialize();
    odometry.initialize();
}

// Set default commands here -----------------------------------------------
void setDefaultCommands(src::Drivers *)
{
    chassis.setDefaultCommand(&moveChassisCommandKeyboard);
    flywheel.setDefaultCommand(&flywheelOffCommand);
    turret.setDefaultCommand(&moveTurretCommandMouse);
}

void runStartupCommands(src::Drivers *) {}

// Register IO mappings here -----------------------------------------------
void registerMappings(src::Drivers *drivers) {}
}  // namespace sentry_control

namespace control
{
void initSubsystemCommands(src::Drivers *drivers)
{
    sentry_control::initializeSubsystems();
    sentry_control::registerStandardSubsystems(drivers);
    sentry_control::setDefaultCommands(drivers);
    sentry_control::runStartupCommands(drivers);
    sentry_control::registerMappings(drivers);
}
}  // namespace control
#endif