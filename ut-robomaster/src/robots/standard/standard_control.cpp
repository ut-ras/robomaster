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

// Agitator includes ----------------------------------------
#include "subsystems/agitator/agitator_subsystem.hpp"
#include "subsystems/agitator/command_rotate_agitator_continuous.hpp"
#include "subsystems/agitator/command_unjam_agitator.hpp"

// Flywheel includes ----------------------------------------
#include "subsystems/flywheel/flywheel_subsystem.hpp"
#include "subsystems/flywheel/command_rotate_flywheel.hpp"
#include "subsystems/flywheel/command_flywheel_off.hpp"

// #include "commands/command_aim_strategy.hpp"
// #include "commands/command_look_behind.hpp"
// #include "commands/command_move_turret.hpp"

#include "subsystems/odometry/odometry_subsystem.hpp"
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

namespace standard_control
{

// Subsystem definitions ---------------------------------------------------------
ChassisSubsystem chassis(drivers());
AgitatorSubsystem agitator(drivers());
FlywheelSubsystem flywheel(drivers());
TurretSubsystem turret(drivers());
OdometrySubsystem odometry(drivers(), &chassis, &turret);

// Command definitions -----------------------------------------------------------
CommandMoveChassisJoystick moveChassisCommandJoystick(drivers(), &chassis, &turret);
CommandMoveChassisKeyboard moveChassisCommandKeyboard(drivers(), &chassis, &turret);
CommandBeybladeChassisKeyboard beybladeChassisCommandKeyboard(drivers(), &chassis, &turret);

CommandRotateAgitatorContinuous rotateAgitatorContinuousCommand(drivers(), &agitator);
CommandUnjamAgitator unjamAgitatorCommand(drivers(), &agitator);

CommandRotateFlywheel rotateFlywheelCommand(drivers(), &flywheel);
CommandFlywheelOff flywheelOffCommand(drivers(), &flywheel);

// CommandMoveTurret moveTurretCommand(drivers(), &state, &turret);
// CommandLookBehind lookBehindCommand(&turret, &state);
// CommandAimStrategy aimStrategyCommand(drivers(), &turret);

// Keyboard mappings ------------------------------------------------------------
ToggleCommandMapping keyRToggled(
    drivers(),
    {&beybladeChassisCommandKeyboard},
    RemoteMapState({Remote::Key::R}));

HoldCommandMapping leftMouseDown(
    drivers(),
    {&rotateAgitatorContinuousCommand},
    RemoteMapState(RemoteMapState::MouseButton::LEFT));

HoldCommandMapping keyXHeld(
    drivers(), 
    {&unjamAgitatorCommand}, 
    RemoteMapState({Remote::Key::X}));

// Joystick mappings ------------------------------------------------------------
HoldCommandMapping rightSwitchDown(
    drivers(),
    {&moveChassisCommandJoystick},
    RemoteMapState(Remote::Switch::RIGHT_SWITCH, Remote::SwitchState::DOWN));

HoldCommandMapping leftSwitchDown(
    drivers(),
    {&flywheelOffCommand},
    RemoteMapState(Remote::Switch::LEFT_SWITCH, Remote::SwitchState::DOWN));

HoldCommandMapping leftSwitchMid(
    drivers(),
    {&rotateFlywheelCommand},
    RemoteMapState(Remote::Switch::LEFT_SWITCH, Remote::SwitchState::MID));

HoldCommandMapping leftSwitchUp(
    drivers(),
    {&rotateAgitatorContinuousCommand, &rotateFlywheelCommand},
    RemoteMapState(Remote::Switch::LEFT_SWITCH, Remote::SwitchState::UP));
    
// PressCommandMapping lookBehind(drivers(), {&lookBehindCommand}, RemoteMapState({Remote::Key::B}));

// PressCommandMapping changeAimStrategy(
//     drivers(),
//     {&aimStrategyCommand},
//     RemoteMapState(RemoteMapState::MouseButton::RIGHT));



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
    flywheel.setDefaultCommand(&rotateFlywheelCommand);
    // turret.setDefaultCommand(&moveTurretCommand);
}

void runStartupCommands(src::Drivers *) {}

// Register IO mappings here -----------------------------------------------
void registerMappings(src::Drivers *drivers)
{
    // Keyboard mappings ------------------------------------------------------------
    drivers->commandMapper.addMap(&keyRToggled);
    drivers->commandMapper.addMap(&leftMouseDown);
    drivers->commandMapper.addMap(&keyXHeld);

    // Joystick mappings ------------------------------------------------------------
    drivers->commandMapper.addMap(&rightSwitchDown);    
    drivers->commandMapper.addMap(&leftSwitchDown);
    drivers->commandMapper.addMap(&leftSwitchMid);
    drivers->commandMapper.addMap(&leftSwitchUp);
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