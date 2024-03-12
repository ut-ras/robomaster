#ifdef TARGET_SENTRY

#include "tap/communication/gpio/leds.hpp"
#include "tap/control/command_mapper.hpp"
#include "tap/control/hold_command_mapping.hpp"
#include "tap/control/hold_repeat_command_mapping.hpp"
#include "tap/control/press_command_mapping.hpp"
#include "tap/control/toggle_command_mapping.hpp"

#include "robots/robot_constants.hpp"

#include "drivers.hpp"
#include "drivers_singleton.hpp"

// Chassis includes ----------------------------------------
#include "subsystems/chassis/chassis_subsystem.hpp"
#include "subsystems/chassis/command_sentry_position.hpp"

// Agitator includes ----------------------------------------
#include "subsystems/agitator/agitator_subsystem.hpp"

// Flywheel includes ----------------------------------------
#include "subsystems/flywheel/flywheel_subsystem.hpp"

// Turret includes ------------------------------------------
#include "subsystems/odometry/odometry_subsystem.hpp"
#include "subsystems/turret/command_sentry_aim.hpp"
#include "subsystems/turret/turret_subsystem.hpp"

// Sound includes -----------------------------------------
#include "subsystems/sound/command_play_sound.hpp"
#include "subsystems/sound/sound_subsystem.hpp"

using namespace tap::control;
using namespace tap::communication::serial;

using namespace subsystems::chassis;
using namespace subsystems::agitator;
using namespace subsystems::flywheel;
using namespace subsystems::turret;
using namespace subsystems::odometry;
using namespace subsystems::sound;

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
AgitatorSubsystem agitator1(drivers(), ID_AGITATOR_L, false);
AgitatorSubsystem agitator2(drivers(), ID_AGITATOR_R, true);
FlywheelSubsystem flywheel(drivers());
TurretSubsystem turret(drivers());
OdometrySubsystem odometry(drivers(), &chassis, &turret);
SoundSubsystem sound(drivers());

// Command definitions -----------------------------------------------------------
CommandSentryPosition sentryPositionCommand(drivers(), &chassis);
CommandSentryAim sentryAimCommand(drivers(), &turret);

CommandPlaySound playStartupSoundCommand(drivers(), &sound, SOUND_STARTUP);

// Register subsystems here -----------------------------------------------
void registerStandardSubsystems(src::Drivers *drivers)
{
    drivers->commandScheduler.registerSubsystem(&chassis);
    drivers->commandScheduler.registerSubsystem(&agitator1);
    drivers->commandScheduler.registerSubsystem(&agitator2);
    drivers->commandScheduler.registerSubsystem(&flywheel);
    drivers->commandScheduler.registerSubsystem(&turret);
    drivers->commandScheduler.registerSubsystem(&odometry);
    drivers->commandScheduler.registerSubsystem(&sound);
}

// Initialize subsystems here ---------------------------------------------
void initializeSubsystems()
{
    chassis.initialize();
    agitator1.initialize();
    agitator2.initialize();
    flywheel.initialize();
    turret.initialize();
    odometry.initialize();
    sound.initialize();
}

// Set default commands here -----------------------------------------------
void setDefaultCommands(src::Drivers *)
{
    chassis.setDefaultCommand(&sentryPositionCommand);
    turret.setDefaultCommand(&sentryAimCommand);
}

void runStartupCommands(src::Drivers *drivers)
{
    drivers->commandScheduler.addCommand(&playStartupSoundCommand);
}

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