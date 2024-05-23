#include "robots/robot_control_common.hpp"

class HeroControl
{
public:
    HeroControl(src::Drivers *drivers) : drivers(drivers)
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
    AgitatorSubsystem agitator{drivers, AGITATOR};
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

    CommandAgitatorContinuous agitatorContinuousCommand{drivers, &agitator, BarrelId::HERO};
    CommandUnjamAgitator unjamAgitatorCommand{drivers, &agitator};

    CommandRotateFlywheel rotateFlywheelKeyboardCommand{drivers, &flywheel};
    CommandRotateFlywheel rotateFlywheelNoAgitatorCommand{drivers, &flywheel};
    CommandRotateFlywheel rotateFlywheelWithAgitatorCommand{drivers, &flywheel};
    CommandFlywheelOff flywheelOffCommand{drivers, &flywheel};

    CommandMoveTurretJoystick moveTurretCommandJoystick{drivers, &turret};
    CommandMoveTurretJoystick moveTurretWhenChassisIsTurretRelativeCommandJoystick{
        drivers,
        &turret};
    CommandMoveTurretMouse moveTurretCommandMouse{drivers, &turret};

    CommandPlaySound playStartupSoundCommand{drivers, &sound, SOUND_STARTUP};

    // Keyboard mappings ------------------------------------------------------------
    ToggleCommandMapping keyRToggled{
        drivers,
        {&beybladeChassisCommandKeyboard},
        RemoteMapState({Remote::Key::R})};

    ToggleCommandMapping keySToggled{
        drivers,
        {&rotateFlywheelKeyboardCommand},
        RemoteMapState({Remote::Key::S})};

    HoldCommandMapping leftMouseDown{
        drivers,
        {&agitatorContinuousCommand},
        RemoteMapState(RemoteMapState::MouseButton::LEFT)};

    HoldCommandMapping keyXHeld{drivers, {&unjamAgitatorCommand}, RemoteMapState({Remote::Key::X})};

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
        {&agitatorContinuousCommand, &rotateFlywheelWithAgitatorCommand},
        RemoteMapState(Remote::Switch::LEFT_SWITCH, Remote::SwitchState::UP)};

    void initializeSubsystems()
    {
        chassis.initialize();
        agitator.initialize();
        flywheel.initialize();
        turret.initialize();
        sound.initialize();
    }

    void registerSubsystems()
    {
        drivers->commandScheduler.registerSubsystem(&chassis);
        drivers->commandScheduler.registerSubsystem(&agitator);
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
        drivers->commandMapper.addMap(&keySToggled);

        // Controller
        drivers->commandMapper.addMap(&rightSwitchUp);
        drivers->commandMapper.addMap(&rightSwitchMid);
        drivers->commandMapper.addMap(&rightSwitchDown);
        drivers->commandMapper.addMap(&leftSwitchMid);
        drivers->commandMapper.addMap(&leftSwitchUp);
    }
};