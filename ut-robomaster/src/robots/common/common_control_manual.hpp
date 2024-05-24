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
    }

    // Commands
    CommandMoveChassisJoystick moveChassisRelative_Joystick{drivers, &chassis, &turret};
    CommandMoveChassisTurretRelativeJoystick moveTurretRelative_Joystick{
        drivers,
        &chassis,
        &turret};
    CommandMoveChassisKeyboard moveTurretRelative_Keyboard{drivers, &chassis, &turret};
    CommandBeybladeChassisKeyboard beyblade_Keyboard{drivers, &chassis, &turret};

    CommandRotateFlywheel rotateFlywheel_Keyboard{drivers, &flywheel};
    CommandRotateFlywheel rotateFlywheel_SwitchUp{drivers, &flywheel};
    CommandRotateFlywheel rotateFlywheel_SwitchMid{drivers, &flywheel};

    CommandMoveTurretJoystick look_Joystick_SwitchMid{drivers, &turret};
    CommandMoveTurretJoystick look_Joystick_SwitchUp{drivers, &turret};
    CommandMoveTurretMouse moveTurretCommandMouse{drivers, &turret};
    CommandMoveTurretAimbot moveTurretCommandAimbot{drivers, &turret};

    // Keyboard mappings
    ToggleCommandMapping keyRToggled{
        drivers,
        {&beyblade_Keyboard},
        RemoteMapState({Remote::Key::R})};

    ToggleCommandMapping keyGToggled{
        drivers,
        {&rotateFlywheel_Keyboard},
        RemoteMapState({Remote::Key::G})};

    HoldCommandMapping rightMouseDown{
        drivers,
        {&moveTurretCommandAimbot},
        RemoteMapState(RemoteMapState::MouseButton::RIGHT)};

    // Controller mappings
    HoldCommandMapping rightSwitchUp{
        drivers,
        {&moveTurretRelative_Joystick, &look_Joystick_SwitchUp},
        RemoteMapState(Remote::Switch::RIGHT_SWITCH, Remote::SwitchState::UP)};

    HoldCommandMapping rightSwitchMid{
        drivers,
        {&moveChassisRelative_Joystick, &look_Joystick_SwitchMid},
        RemoteMapState(Remote::Switch::RIGHT_SWITCH, Remote::SwitchState::MID)};

    HoldCommandMapping rightSwitchDown{
        drivers,
        {&moveTurretRelative_Keyboard, &moveTurretCommandMouse},
        RemoteMapState(Remote::Switch::RIGHT_SWITCH, Remote::SwitchState::DOWN)};

    HoldCommandMapping leftSwitchMid{
        drivers,
        {&rotateFlywheel_SwitchUp},
        RemoteMapState(Remote::Switch::LEFT_SWITCH, Remote::SwitchState::MID)};
};