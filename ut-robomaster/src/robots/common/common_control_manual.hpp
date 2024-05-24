#include "tap/control/command_mapper.hpp"
#include "tap/control/hold_command_mapping.hpp"
#include "tap/control/hold_repeat_command_mapping.hpp"
#include "tap/control/press_command_mapping.hpp"
#include "tap/control/toggle_command_mapping.hpp"

#include "robots/common/common_control.hpp"
#include "subsystems/chassis/command_beyblade_chassis_keyboard.hpp"
#include "subsystems/chassis/command_move_chassis_joystick.hpp"
#include "subsystems/chassis/command_move_chassis_keyboard.hpp"
#include "subsystems/chassis/command_move_chassis_turret_relative_joystick.hpp"
#include "subsystems/flywheel/command_flywheel_off.hpp"
#include "subsystems/flywheel/command_rotate_flywheel.hpp"
#include "subsystems/turret/command_move_turret_aimbot.hpp"
#include "subsystems/turret/command_move_turret_joystick.hpp"
#include "subsystems/turret/command_move_turret_mouse.hpp"

class CommonControlManual : protected CommonControl
{
protected:
    CommonControlManual(src::Drivers* drivers) : CommonControl(drivers) {}

    void initialize()
    {
        CommonControl::initialize();

        // Mouse and Keyboard
        drivers->commandMapper.addMap(&keyRToggled);
        drivers->commandMapper.addMap(&keyGToggled);
        drivers->commandMapper.addMap(&rightMouseDown);

        // Controller
        drivers->commandMapper.addMap(&rightSwitchUp);
        drivers->commandMapper.addMap(&rightSwitchMid);
        drivers->commandMapper.addMap(&rightSwitchDown);
        drivers->commandMapper.addMap(&leftSwitchMid);
        drivers->commandMapper.addMap(&leftSwitchDown);
    }

    // Commands
    CommandMoveChassisJoystick moveChassisCommandJoystick{drivers, &chassis, &turret};
    CommandMoveChassisTurretRelativeJoystick moveChassisTurretRelativeCommandJoystick{
        drivers,
        &chassis,
        &turret};
    CommandMoveChassisKeyboard moveChassisCommandKeyboard{drivers, &chassis, &turret};
    CommandBeybladeChassisKeyboard beybladeChassisCommandKeyboard{drivers, &chassis, &turret};

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

    // Keyboard mappings
    ToggleCommandMapping keyRToggled{
        drivers,
        {&beybladeChassisCommandKeyboard},
        RemoteMapState({Remote::Key::R})};

    ToggleCommandMapping keyGToggled{
        drivers,
        {&rotateFlywheelKeyboardCommand},
        RemoteMapState({Remote::Key::G})};

    HoldCommandMapping rightMouseDown{
        drivers,
        {&moveTurretCommandAimbot},
        RemoteMapState(RemoteMapState::MouseButton::RIGHT)};

    // Controller mappings
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
        {&moveChassisCommandKeyboard, &moveTurretCommandMouse},
        RemoteMapState(Remote::Switch::RIGHT_SWITCH, Remote::SwitchState::DOWN)};

    HoldCommandMapping leftSwitchMid{
        drivers,
        {&rotateFlywheelNoAgitatorCommand},
        RemoteMapState(Remote::Switch::LEFT_SWITCH, Remote::SwitchState::MID)};

    HoldCommandMapping leftSwitchDown{
        drivers,
        {&flywheelOffCommand},
        RemoteMapState(Remote::Switch::LEFT_SWITCH, Remote::SwitchState::DOWN)};
};