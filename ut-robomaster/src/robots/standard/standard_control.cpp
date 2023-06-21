#ifdef TARGET_STANDARD

#include "drivers.hpp"
#include "drivers_singleton.hpp"

#include "tap/communication/gpio/leds.hpp"
#include "tap/control/command_mapper.hpp"
#include "tap/control/hold_command_mapping.hpp"
#include "tap/control/hold_repeat_command_mapping.hpp"
#include "tap/control/press_command_mapping.hpp"
#include "tap/control/toggle_command_mapping.hpp"

// Chassis includes ----------------------------------------
#include "subsystems/chassis/chassis_subsystem.hpp"
#include "subsystems/chassis/command_move_chassis_joystick.hpp"
#include "subsystems/chassis/command_move_chassis_keyboard.hpp"
#include "subsystems/chassis/command_beyblade_chassis_keyboard.hpp"

// Agitator includes
#include "subsystems/agitator/agitator_subsystem.hpp"
#include "subsystems/agitator/command_rotate_agitator_continuous.hpp"
#include "subsystems/agitator/command_unjam_agitator.hpp"

// #include "commands/command_aim_strategy.hpp"
// #include "commands/command_look_behind.hpp"
// #include "commands/command_move_turret.hpp"

#include "subsystems/odometry/odometry_subsystem.hpp"
#include "subsystems/turret/turret_subsystem.hpp"

using namespace tap::control;
using namespace tap::communication::serial;

using namespace subsystems::chassis;
using namespace subsystems::turret;
using namespace subsystems::agitator;
using namespace subsystems::odometry;

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
ChassisSubsystem chassis(drivers());
TurretSubsystem turret(drivers());
AgitatorSubsystem agitator(drivers());
OdometrySubsystem odometry(drivers(), &chassis, &turret);

// Command definitions -----------------------------------------------------------
CommandMoveChassisJoystick moveChassisCommandJoystick(drivers(), &chassis, &turret);
CommandMoveChassisKeyboard moveChassisCommandKeyboard(drivers(), &chassis, &turret);
CommandBeybladeChassisKeyboard beybladeChassisCommandKeyboard(drivers(), &chassis, &turret);

CommandRotateAgitatorContinuous rotateAgitatorContinuousCommand(drivers(), &agitator);
CommandUnjamAgitator unjamAgitatorCommand(drivers(), &agitator);

// CommandMoveTurret moveTurretCommand(drivers(), &state, &turret);
// CommandShooterDefault shooterDefaultCommand(drivers(), &shooter);
// CommandFireContinuous fireContinuousCommand(drivers(), &shooter);
// CommandFireOnce fireOnceCommand(drivers(), &shooter);
// CommandLookBehind lookBehindCommand(&turret, &state);
// CommandAimStrategy aimStrategyCommand(drivers(), &turret);
// CommandUnjam unjamCommand(drivers(), &shooter);

// Command mapping definitions ---------------------------------------------------
HoldCommandMapping rightSwitchDown(
    drivers(),
    {&moveChassisCommandJoystick},
    RemoteMapState(Remote::Switch::RIGHT_SWITCH, Remote::SwitchState::DOWN));

ToggleCommandMapping keyRToggled(
    drivers(),
    {&beybladeChassisCommandKeyboard},
    RemoteMapState({Remote::Key::R}));

HoldCommandMapping leftMouseDown(
    drivers(),
    {&rotateAgitatorContinuousCommand},
    RemoteMapState(RemoteMapState::MouseButton::LEFT));

HoldCommandMapping keyXHeld(drivers(), {&unjamAgitatorCommand}, RemoteMapState({Remote::Key::X}));
// PressCommandMapping lookBehind(drivers(), {&lookBehindCommand}, RemoteMapState({Remote::Key::B}));

// PressCommandMapping changeAimStrategy(
//     drivers(),
//     {&aimStrategyCommand},
//     RemoteMapState(RemoteMapState::MouseButton::RIGHT));



// Register subsystems here -----------------------------------------------
void registerStandardSubsystems(src::Drivers *drivers)
{
    drivers->commandScheduler.registerSubsystem(&chassis);
    drivers->commandScheduler.registerSubsystem(&turret);
    drivers->commandScheduler.registerSubsystem(&agitator);
    drivers->commandScheduler.registerSubsystem(&odometry);
}

// Initialize subsystems here ---------------------------------------------
void initializeSubsystems()
{
    chassis.initialize();
    turret.initialize();
    agitator.initialize();
    odometry.initialize();
}

// Set default commands here -----------------------------------------------
void setDefaultCommands(src::Drivers *)
{
    chassis.setDefaultCommand(&moveChassisCommandKeyboard);
    // turret.setDefaultCommand(&moveTurretCommand);
}

void runStartupCommands(src::Drivers *) {}

// Register IO mappings here -----------------------------------------------
void registerMappings(src::Drivers *drivers)
{
    drivers->commandMapper.addMap(&rightSwitchDown);
    drivers->commandMapper.addMap(&keyRToggled);
    drivers->commandMapper.addMap(&leftMouseDown);
    drivers->commandMapper.addMap(&keyXHeld);
    // drivers->commandMapper.addMap(&lookBehind);
    // drivers->commandMapper.addMap(&changeAimStrategy);
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