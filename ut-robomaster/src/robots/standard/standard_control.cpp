// #ifndef ENV_UNIT_TESTS

// #ifdef TARGET_STANDARD

#include "drivers.hpp"
#include "drivers_singleton.hpp"

#include "tap/control/command_mapper.hpp"
#include "tap/control/hold_repeat_command_mapping.hpp"
#include "tap/control/hold_command_mapping.hpp"
#include "tap/control/press_command_mapping.hpp"
#include "tap/drivers.hpp"



// #include "agitator/agitator_rotate_command.hpp"
// #include "agitator/agitator_reverse_command.hpp"
// #include "agitator/agitator_subsystem.hpp"

#include "subsystems/shooter/shooter_on_command.hpp"
#include "subsystems/shooter/shooter_single_fire_command.hpp"
#include "subsystems/shooter/shooter_subsystem.hpp"

// #include "chassis/chassis_subsystem.hpp"
// #include "chassis/chassis_drive_command.hpp"
// #include "chassis/chassis_drive_keyboard_command.hpp"

// #include "gimbal/gimbal_subsystem.hpp"
// #include "gimbal/gimbal_move_command.hpp"

// #include "turret/turret_subsystem.hpp"
// #include "turret/turret_move_command.hpp"

#include "tap/communication/gpio/leds.hpp"

using namespace tap::control;
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
// control::agitator::AgitatorSubsystem theAgitator(drivers());
subsystems::shooter::ShooterSubsystem theShooter(drivers());
// control::turret::TurretSubsystem theTurret(drivers());  // mouse  
// control::gimbal::GimbalSubsystem theGimbal(drivers());   // joystick
// control::chassis::ChassisSubsystem theChassis(drivers(), &theTurret.getYawMotor());

/* define commands ----------------------------------------------------------*/
// control::agitator::AgitatorRotateCommand rotateCommand(&theAgitator);
// control::agitator::AgitatorReverseCommand reverseCommand(&theAgitator);
subsystems::shooter::ShooterOnCommand shooterCommand(&theShooter);
subsystems::shooter::ShooterSingleFireCommand singleFireCommand(&theShooter);
// control::chassis::ChassisDriveKeyboardCommand chassisDriveKeyboardCommand(drivers(), &theChassis);  // keyboard
//control::chassis::ChassisDriveCommand chassisDriveCommand(drivers(), &theChassis);   // joystick
// control::turret::TurretMoveCommand turretMoveCommand(drivers(), &theTurret);    //mouse 
//control::gimbal::GimbalMoveCommand gimbalMoveCommand(drivers(), &theGimbal);     //joystick

/* define command mappings --------------------------------------------------*/
// HoldRepeatCommandMapping reverseAgitator(
//     drivers(),
//     {&reverseCommand},
//     RemoteMapState({Remote::Key::F}), true, -1);

// HoldRepeatCommandMapping rightSwitchDown(
//     drivers(),
//     {&reverseCommand},
//     RemoteMapState(Remote::Switch::RIGHT_SWITCH, Remote::SwitchState::DOWN), true, -1);

HoldRepeatCommandMapping leftSwitchUp(
    drivers(),
    {&shooterCommand},
    RemoteMapState(Remote::Switch::LEFT_SWITCH, Remote::SwitchState::UP), true, -1);

HoldRepeatCommandMapping rightMouseDown(
    drivers(),
    {&shooterCommand},
    RemoteMapState(RemoteMapState::MouseButton::RIGHT), true, -1);

PressCommandMapping singleFire(
    drivers(),
    {&singleFireCommand},
    RemoteMapState(Remote::Switch::RIGHT_SWITCH, Remote::SwitchState::UP));

// HoldRepeatCommandMapping leftMouseDown(
//     drivers(),
//     {&rotateCommand},
//     RemoteMapState(RemoteMapState::MouseButton::LEFT), true, -1);



/* register subsystems here -------------------------------------------------*/
void registerStandardSubsystems(tap::Drivers *drivers)
{
    // drivers->commandScheduler.registerSubsystem(&theAgitator);
    drivers->commandScheduler.registerSubsystem(&theShooter);
    theShooter.registerSubsystems();
    // drivers->commandScheduler.registerSubsystem(&theChassis);
    // drivers->commandScheduler.registerSubsystem(&theTurret);    // mouse
    //drivers->commandScheduler.registerSubsystem(&theGimbal);     // joystick
}

/* initialize subsystems ----------------------------------------------------*/
void initializeSubsystems() { 
    // theAgitator.initialize();
    theShooter.initialize();
    // theChassis.initialize();
    // theTurret.initialize();     // mouse
    //theGimbal.initialize();  // joystick
}

/* set any default commands to subsystems here ------------------------------*/
void setDefaultStandardCommands(tap::Drivers *) 
{
    // theChassis.setDefaultCommand(&chassisDriveKeyboardCommand); // keyboard
    // theChassis.setDefaultCommand(&chassisDriveCommand);  //joystick
    // theTurret.setDefaultCommand(&turretMoveCommand);    //mouse
    // theGimbal.setDefaultCommand(&gimbalMoveCommand);     //joystick
}

/* add any starting commands to the scheduler here --------------------------*/
void startStandardCommands(tap::Drivers *) {}

/* register io mappings here ------------------------------------------------*/
void registerStandardIoMappings(tap::Drivers *drivers)
{
    // drivers->commandMapper.addMap(&rightSwitchUp);
    // drivers->commandMapper.addMap(&reverseAgitator);
    // drivers->commandMapper.addMap(&rightSwitchDown);
    drivers->commandMapper.addMap(&leftSwitchUp);
    drivers->commandMapper.addMap(&singleFire);
    // drivers->commandMapper.addMap(&leftMouseDown);
    drivers->commandMapper.addMap(&rightMouseDown);
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

// #endif
// #endif