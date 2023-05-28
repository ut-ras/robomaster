#include "tap/communication/gpio/leds.hpp"
#include "tap/control/command_mapper.hpp"
#include "tap/control/hold_command_mapping.hpp"
#include "tap/control/hold_repeat_command_mapping.hpp"
#include "tap/control/press_command_mapping.hpp"
#include "tap/control/toggle_command_mapping.hpp"

#include "subsystems/chassis/chassis_subsystem.hpp"
#include "subsystems/chassis/command_move_chassis.hpp"
#include "subsystems/chassis/command_sentry_position.hpp"
#include "subsystems/odometry/odometry_subsystem.hpp"
#include "subsystems/shooter/command_fire_continuous.hpp"
#include "subsystems/shooter/command_fire_once.hpp"
#include "subsystems/shooter/shooter_subsystem.hpp"
#include "subsystems/turret/command_move_turret.hpp"
#include "subsystems/turret/turret_subsystem.hpp"

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
// Subsystems
chassis::ChassisSubsystem chassis(drivers());
turret::TurretSubsystem turret(drivers());
shooter::ShooterSubsystem shooter(drivers());
odometry::OdometrySubsystem odometry(drivers(), &chassis, &turret);

// Commands
chassis::CommandMoveChassis moveChassisCommand(drivers(), &chassis, &turret);
chassis::CommandSentryPosition sentryPositionCommand(drivers(), &chassis);
turret::CommandMoveTurret moveTurretCommand(drivers(), &turret);
shooter::CommandFireContinuous fireContinuousCommand(drivers(), &shooter);
shooter::CommandFireOnce fireOnceCommand(drivers(), &shooter);

// Mappings
PressCommandMapping singleFire(
    drivers(),
    {&fireOnceCommand},
    RemoteMapState(RemoteMapState::MouseButton::RIGHT));

void registerStandardSubsystems(src::Drivers *drivers)
{
    drivers->commandScheduler.registerSubsystem(&chassis);
    drivers->commandScheduler.registerSubsystem(&turret);
    drivers->commandScheduler.registerSubsystem(&shooter);
    drivers->commandScheduler.registerSubsystem(&odometry);
    shooter.registerSubsystems();
}

void initializeSubsystems()
{
    chassis.initialize();
    turret.initialize();
    shooter.initialize();
    odometry.initialize();
}

void setDefaultCommands(src::Drivers *)
{
    chassis.setDefaultCommand(&sentryPositionCommand);
    turret.setDefaultCommand(&moveTurretCommand);
    shooter.setDefaultCommand(&fireContinuousCommand);
}

void runStartupCommands(src::Drivers *) {}

void registerMappings(src::Drivers *drivers) { drivers->commandMapper.addMap(&singleFire); }
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
