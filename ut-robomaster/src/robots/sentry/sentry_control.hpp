#include "robots/robot_control_common.hpp"

class SentryControl
{
public:
    SentryControl(src::Drivers *drivers) : drivers(drivers)
    {
        initializeSubsystems();
        registerSubsystems();
        setDefaultCommands();
        runStartupCommands();
        registerMappings();
    }

private:
    src::Drivers *drivers;

    // Subsystem definitions ---------------------------------------------------------
    ChassisSubsystem chassis{drivers};
    AgitatorSubsystem agitator1{drivers, AGITATOR_L};
    AgitatorSubsystem agitator2{drivers, AGITATOR_R};
    FlywheelSubsystem flywheel{drivers};
    TurretSubsystem turret{drivers};
    SoundSubsystem sound{drivers};

    // Command definitions -----------------------------------------------------------
    CommandMoveChassisJoystick moveChassisCommandJoystick{drivers, &chassis, &turret};
    CommandMoveChassisTurretRelativeJoystick moveChassisTurretRelativeCommandJoystick{
        drivers,
        &chassis,
        &turret};
    CommandMoveChassisKeyboard moveChassisCommandKeyboard{drivers, &chassis, &turret};
    CommandBeybladeChassisKeyboard beybladeChassisCommandKeyboard{drivers, &chassis, &turret};

    CommandAgitatorContinuous agitator1ContinuousCommand{drivers, &agitator1, BarrelId::STANDARD1};
    CommandAgitatorContinuous agitator2ContinuousCommand{drivers, &agitator2, BarrelId::STANDARD2};
    CommandUnjamAgitator unjamAgitator1Command{drivers, &agitator1};
    CommandUnjamAgitator unjamAgitator2Command{drivers, &agitator2};

    CommandRotateFlywheel rotateFlywheelKeyboardCommand{drivers, &flywheel};
    CommandRotateFlywheel rotateFlywheelNoAgitatorCommand{drivers, &flywheel};
    CommandRotateFlywheel rotateFlywheelWithAgitatorCommand{drivers, &flywheel};
    CommandFlywheelOff flywheelOffCommand{drivers, &flywheel};

    CommandMoveTurretJoystick moveTurretCommandJoystick{drivers, &turret};
    CommandMoveTurretJoystick moveTurretWhenChassisIsTurretRelativeCommandJoystick{
        drivers,
        &turret};
    CommandMoveTurretMouse moveTurretCommandMouse{drivers, &turret};
    CommandMoveTurretAimbot moveTurretCommandAimbot{drivers, &turret};

    CommandPlaySound playStartupSoundCommand{drivers, &sound, SOUND_STARTUP};

    // Keyboard mappings ------------------------------------------------------------
    ToggleCommandMapping keyRToggled{
        drivers,
        {&beybladeChassisCommandKeyboard},
        RemoteMapState({Remote::Key::R})};

    ToggleCommandMapping keyGToggled{
        drivers,
        {&rotateFlywheelKeyboardCommand},
        RemoteMapState({Remote::Key::G})};

    HoldCommandMapping leftMouseDown{
        drivers,
        {&agitator1ContinuousCommand, &agitator2ContinuousCommand},
        RemoteMapState(RemoteMapState::MouseButton::LEFT)};

    HoldCommandMapping keyXHeld{
        drivers,
        {&unjamAgitator1Command, &unjamAgitator2Command},
        RemoteMapState({Remote::Key::X})};

    HoldCommandMapping rightMouseDown{
        drivers,
        {&moveTurretCommandAimbot},
        RemoteMapState(RemoteMapState::MouseButton::RIGHT)};

    // Controller mappings ------------------------------------------------------------
    HoldCommandMapping rightSwitchUp{
        drivers,
        {&moveChassisTurretRelativeCommandJoystick,
         &moveTurretWhenChassisIsTurretRelativeCommandJoystick},
        RemoteMapState(Remote::Switch::RIGHT_SWITCH, Remote::SwitchState::UP)};

    HoldCommandMapping rightSwitchMid{
        drivers,
        {&moveChassisCommandJoystick, &moveTurretCommandJoystick},
        RemoteMapState(Remote::Switch::RIGHT_SWITCH, Remote::SwitchState::MID)};

    HoldCommandMapping rightSwitchDown{
        drivers,
        {&flywheelOffCommand},
        RemoteMapState(Remote::Switch::RIGHT_SWITCH, Remote::SwitchState::DOWN)};

    HoldCommandMapping leftSwitchMid{
        drivers,
        {&rotateFlywheelNoAgitatorCommand},
        RemoteMapState(Remote::Switch::LEFT_SWITCH, Remote::SwitchState::MID)};

    HoldCommandMapping leftSwitchUp{
        drivers,
        {&agitator1ContinuousCommand,
         &agitator2ContinuousCommand,
         &rotateFlywheelWithAgitatorCommand},
        RemoteMapState(Remote::Switch::LEFT_SWITCH, Remote::SwitchState::UP)};

    void initializeSubsystems()
    {
        chassis.initialize();
        agitator1.initialize();
        agitator2.initialize();
        flywheel.initialize();
        turret.initialize();
        sound.initialize();
    }

    void registerSubsystems()
    {
        drivers->commandScheduler.registerSubsystem(&chassis);
        drivers->commandScheduler.registerSubsystem(&agitator1);
        drivers->commandScheduler.registerSubsystem(&agitator2);
        drivers->commandScheduler.registerSubsystem(&flywheel);
        drivers->commandScheduler.registerSubsystem(&turret);
        drivers->commandScheduler.registerSubsystem(&sound);
    }

    void setDefaultCommands()
    {
        chassis.setDefaultCommand(&moveChassisCommandKeyboard);
        flywheel.setDefaultCommand(&flywheelOffCommand);
        turret.setDefaultCommand(&moveTurretCommandMouse);
    }

    void runStartupCommands() { drivers->commandScheduler.addCommand(&playStartupSoundCommand); }

    void registerMappings()
    {
        // Keyboard
        drivers->commandMapper.addMap(&keyRToggled);
        drivers->commandMapper.addMap(&leftMouseDown);
        drivers->commandMapper.addMap(&keyXHeld);
        drivers->commandMapper.addMap(&rightMouseDown);
        drivers->commandMapper.addMap(&keyGToggled);

        // Controller
        drivers->commandMapper.addMap(&rightSwitchUp);
        drivers->commandMapper.addMap(&rightSwitchMid);
        drivers->commandMapper.addMap(&rightSwitchDown);
        drivers->commandMapper.addMap(&leftSwitchMid);
        drivers->commandMapper.addMap(&leftSwitchUp);
    }
};