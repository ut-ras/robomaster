#ifdef TARGET_STANDARD

#include "tap/communication/gpio/leds.hpp"
#include "tap/control/command_mapper.hpp"
#include "tap/control/hold_command_mapping.hpp"
#include "tap/control/hold_repeat_command_mapping.hpp"
#include "tap/control/press_command_mapping.hpp"
#include "tap/control/toggle_command_mapping.hpp"

#include "commands/command_aim_strategy.hpp"
#include "commands/command_fire_continuous.hpp"
#include "commands/command_fire_once.hpp"
#include "commands/command_look_behind.hpp"
#include "commands/command_move_chassis.hpp"
#include "commands/command_move_turret.hpp"
#include "commands/command_shooter_default.hpp"
#include "commands/command_toggle_beyblade.hpp"
#include "commands/command_unjam.hpp"
#include "robots/robot_state.hpp"
#include "subsystems/chassis/chassis_subsystem.hpp"
#include "subsystems/odometry/odometry_subsystem.hpp"
#include "subsystems/shooter/shooter_subsystem.hpp"
#include "subsystems/turret/turret_subsystem.hpp"

#include "drivers.hpp"
#include "drivers_singleton.hpp"

using namespace tap::control;
using namespace tap::communication::serial;
using namespace commands;
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
RobotState state;

// Subsystems
chassis::ChassisSubsystem chassis(drivers());
turret::TurretSubsystem turret(drivers());
shooter::ShooterSubsystem shooter(drivers());
odometry::OdometrySubsystem odometry(drivers(), &chassis, &turret);

// Commands
CommandMoveChassis moveChassisCommand(drivers(), &state, &chassis, &turret);
CommandMoveTurret moveTurretCommand(drivers(), &state, &turret);
CommandShooterDefault shooterDefaultCommand(drivers(), &shooter);
CommandFireContinuous fireContinuousCommand(drivers(), &shooter);
CommandFireOnce fireOnceCommand(drivers(), &shooter);
CommandToggleBeyblade toggleBeybladeCommand(&chassis, &state);
CommandLookBehind lookBehindCommand(&turret, &state);
CommandAimStrategy aimStrategyCommand(drivers(), &turret);
CommandUnjam unjamCommand(drivers(), &shooter);

// Mappings
HoldCommandMapping fire(
    drivers(),
    {&fireContinuousCommand},
    RemoteMapState(RemoteMapState::MouseButton::LEFT));

PressCommandMapping toggleBeyblade(
    drivers(),
    {&toggleBeybladeCommand},
    RemoteMapState({Remote::Key::R}));

PressCommandMapping lookBehind(drivers(), {&lookBehindCommand}, RemoteMapState({Remote::Key::B}));

PressCommandMapping changeAimStrategy(
    drivers(),
    {&aimStrategyCommand},
    RemoteMapState(RemoteMapState::MouseButton::RIGHT));

HoldCommandMapping unjam(drivers(), {&unjamCommand}, RemoteMapState({Remote::Key::X}));

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
    chassis.setDefaultCommand(&moveChassisCommand);
    turret.setDefaultCommand(&moveTurretCommand);
    shooter.setDefaultCommand(&shooterDefaultCommand);
}

void runStartupCommands(src::Drivers *) {}

void registerMappings(src::Drivers *drivers)
{
    drivers->commandMapper.addMap(&fire);
    drivers->commandMapper.addMap(&toggleBeyblade);
    drivers->commandMapper.addMap(&lookBehind);
    drivers->commandMapper.addMap(&changeAimStrategy);
    drivers->commandMapper.addMap(&unjam);
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