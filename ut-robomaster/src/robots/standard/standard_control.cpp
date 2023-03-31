// #ifndef ENV_UNIT_TESTS

// #ifdef TARGET_STANDARD

#include "tap/control/command_mapper.hpp"
#include "tap/control/hold_command_mapping.hpp"
#include "tap/control/hold_repeat_command_mapping.hpp"
#include "tap/control/press_command_mapping.hpp"
#include "tap/control/toggle_command_mapping.hpp"

#include "drivers.hpp"
#include "drivers_singleton.hpp"
#include "robot_comms.hpp"

// #include "agitator/agitator_rotate_command.hpp"
// #include "agitator/agitator_reverse_command.hpp"
// #include "agitator/agitator_subsystem.hpp"

#include "subsystems/chassis/chassis_subsystem.hpp"
#include "subsystems/chassis/command_move_chassis.hpp"
#include "subsystems/odometry/odometry_subsystem.hpp"
#include "subsystems/shooter/shooter_on_command.hpp"
#include "subsystems/shooter/shooter_single_fire_command.hpp"
#include "subsystems/shooter/shooter_subsystem.hpp"
// #include "chassis/chassis_drive_command.hpp"
// #include "chassis/chassis_drive_keyboard_command.hpp"

// #include "gimbal/gimbal_subsystem.hpp"
// #include "gimbal/gimbal_move_command.hpp"

#include "tap/communication/gpio/leds.hpp"

#include "subsystems/turret/turret_command.hpp"
#include "subsystems/turret/turret_subsystem.hpp"

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
chassis::ChassisSubsystem theChassis(drivers());
turret::TurretSubsystem theTurret(drivers());
odometry::OdometrySubsystem theOdometry(drivers(), &theChassis);
communication::BeagleBoneCommunicator theBeaglebone(drivers());

/* define commands ----------------------------------------------------------*/
chassis::MoveChassisCommand moveChassisCommand(&theChassis, drivers());
turret::TurretCommand turretCommand(&theTurret, drivers());

/* define command mappings --------------------------------------------------*/
// HoldCommandMapping testMoveChassis(
//     drivers(),
//     {&setKinematicsCommand},
//     RemoteMapState({Remote::Key::F}));
// control::agitator::AgitatorSubsystem theAgitator(drivers());
shooter::ShooterSubsystem theShooter(drivers());
// control::turret::TurretSubsystem theTurret(drivers());  // mouse
// control::gimbal::GimbalSubsystem theGimbal(drivers());   // joystick
// control::chassis::ChassisSubsystem theChassis(drivers(), &theTurret.getYawMotor());

/* define commands ----------------------------------------------------------*/
// control::agitator::AgitatorRotateCommand rotateCommand(&theAgitator);
// control::agitator::AgitatorReverseCommand reverseCommand(&theAgitator);
shooter::ShooterOnCommand shooterCommand(&theShooter);
shooter::ShooterSingleFireCommand singleFireCommand(&theShooter);
// control::chassis::ChassisDriveKeyboardCommand chassisDriveKeyboardCommand(drivers(),
// &theChassis);  // keyboard
// control::chassis::ChassisDriveCommand chassisDriveCommand(drivers(), &theChassis);   // joystick
// control::turret::TurretMoveCommand turretMoveCommand(drivers(), &theTurret);    //mouse
// control::gimbal::GimbalMoveCommand gimbalMoveCommand(drivers(), &theGimbal);     //joystick

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
    RemoteMapState(Remote::Switch::LEFT_SWITCH, Remote::SwitchState::UP),
    true,
    -1);

HoldRepeatCommandMapping leftMouseDown(
    drivers(),
    {&shooterCommand},
    RemoteMapState(RemoteMapState::MouseButton::LEFT),
    true,
    -1);

PressCommandMapping singleFire(
    drivers(),
    {&singleFireCommand},
    RemoteMapState(RemoteMapState::MouseButton::RIGHT));

// HoldRepeatCommandMapping leftMouseDown(
//     drivers(),
//     {&rotateCommand},
//     RemoteMapState(RemoteMapState::MouseButton::LEFT), true, -1);

/* register subsystems here -------------------------------------------------*/
void registerStandardSubsystems(src::Drivers *drivers)
{
    drivers->commandScheduler.registerSubsystem(&theChassis);
    drivers->commandScheduler.registerSubsystem(&theTurret);
    // drivers->commandScheduler.registerSubsystem(&theAgitator);
    drivers->commandScheduler.registerSubsystem(&theShooter);
    theShooter.registerSubsystems();
    drivers->commandScheduler.registerSubsystem(&theOdometry);
    // drivers->commandScheduler.registerSubsystem(&theChassis);
    // drivers->commandScheduler.registerSubsystem(&theTurret);    // mouse
    // drivers->commandScheduler.registerSubsystem(&theGimbal);     // joystick
}

/* initialize subsystems ----------------------------------------------------*/
void initializeSubsystems()
{
    theChassis.initialize();
    theTurret.initialize();
    // theAgitator.initialize();
    theShooter.initialize();
    theOdometry.initialize();
    // theChassis.initialize();
    // theTurret.initialize();     // mouse
    // theGimbal.initialize();  // joystick
}

/* set any default commands to subsystems here ------------------------------*/
void setDefaultStandardCommands(src::Drivers *)
{
    theChassis.setDefaultCommand(&moveChassisCommand);
    theTurret.setDefaultCommand(&turretCommand);
    // theChassis.setDefaultCommand(&chassisDriveKeyboardCommand); // keyboard
    // theChassis.setDefaultCommand(&chassisDriveCommand);  //joystick
    // theTurret.setDefaultCommand(&turretMoveCommand);    //mouse
    // theGimbal.setDefaultCommand(&gimbalMoveCommand);     //joystick
}

/* add any starting commands to the scheduler here --------------------------*/
void startStandardCommands(src::Drivers *) {}

/* register io mappings here ------------------------------------------------*/
void registerStandardIoMappings(src::Drivers *drivers)
{
    // drivers->commandMapper.addMap(&testMoveChassis);
    // drivers->commandMapper.addMap(&rightSwitchUp);
    // drivers->commandMapper.addMap(&reverseAgitator);
    // drivers->commandMapper.addMap(&rightSwitchDown);
    drivers->commandMapper.addMap(&leftSwitchUp);
    drivers->commandMapper.addMap(&singleFire);
    drivers->commandMapper.addMap(&leftMouseDown);
    // drivers->commandMapper.addMap(&rightMouseDown);
}
}  // namespace standard_control

namespace control
{
void initSubsystemCommands(src::Drivers *drivers)
{
    comms::RobotCommsSingleton::init(drivers);

    standard_control::initializeSubsystems();
    standard_control::registerStandardSubsystems(drivers);
    standard_control::setDefaultStandardCommands(drivers);
    standard_control::startStandardCommands(drivers);
    standard_control::registerStandardIoMappings(drivers);
}
}  // namespace control

// #endif
// #endif
