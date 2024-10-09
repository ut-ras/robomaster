#include "tap/control/command_mapper.hpp"
#include "tap/control/hold_command_mapping.hpp"
#include "tap/control/hold_repeat_command_mapping.hpp"
#include "tap/control/press_command_mapping.hpp"
#include "tap/control/toggle_command_mapping.hpp"

#include "robots/common/common_control.hpp"
#include "subsystems/chassis/command_move_chassis.hpp"
#include "subsystems/chassis/command_move_chassis_joystick.hpp"
#include "subsystems/chassis/command_move_chassis_keyboard.hpp"
#include "subsystems/flywheel/command_flywheel_off.hpp"
#include "subsystems/flywheel/command_rotate_flywheel.hpp"
#include "subsystems/turret/command_move_turret.hpp"
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

        // Controller
        drivers->commandMapper.addMap(&rightSwitchUp);
        drivers->commandMapper.addMap(&rightSwitchMid);
        drivers->commandMapper.addMap(&rightSwitchDown);
        drivers->commandMapper.addMap(&leftSwitchMid);

        chassis.setDefaultCommand(&moveTurretRelative);
        turret.setDefaultCommand(&look);
    }

    // Commands
    CommandMoveChassis moveTurretRelative{drivers, &chassis, &turret, true, true};
    CommandMoveChassis moveBeyblade{drivers, &chassis, &turret, true, true, true};  // Beyblade is always turret relative
    CommandMoveChassis moveJoystickChassisKeyboardTurretRelative{drivers, &chassis, &turret, false, true};
    CommandMoveChassis moveChassisRelative{drivers, &chassis, &turret, false, false};

    CommandRotateFlywheel rotateFlywheel_Keyboard{drivers, &flywheel};
    CommandRotateFlywheel rotateFlywheel_SwitchUp{drivers, &flywheel};
    CommandRotateFlywheel rotateFlywheel_SwitchMid{drivers, &flywheel};

    CommandMoveTurret look{drivers, &turret};

    // Keyboard mappings
    ToggleCommandMapping keyRToggled{drivers, {&moveBeyblade}, RemoteMapState({Remote::Key::R})};

    ToggleCommandMapping keyGToggled{
        drivers,
        {&rotateFlywheel_Keyboard},
        RemoteMapState({Remote::Key::G})};

    // Controller mappings
    HoldCommandMapping rightSwitchUp{
        drivers,
        {&moveTurretRelative},
        RemoteMapState(Remote::Switch::RIGHT_SWITCH, Remote::SwitchState::UP)};

    HoldCommandMapping rightSwitchMid{
        drivers,
        {&moveJoystickChassisKeyboardTurretRelative},
        RemoteMapState(Remote::Switch::RIGHT_SWITCH, Remote::SwitchState::MID)};

    HoldCommandMapping rightSwitchDown{
        drivers,
        {&moveChassisRelative},
        RemoteMapState(Remote::Switch::RIGHT_SWITCH, Remote::SwitchState::DOWN)};

    HoldCommandMapping leftSwitchMid{
        drivers,
        {&rotateFlywheel_SwitchUp},
        RemoteMapState(Remote::Switch::LEFT_SWITCH, Remote::SwitchState::MID)};
};