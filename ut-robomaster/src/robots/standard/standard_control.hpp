#include "robots/common/common_control_manual.hpp"
#include "subsystems/agitator/command_agitator_continuous.hpp"
#include "subsystems/chassis/command_move_point.hpp"


class StandardControl : CommonControlManual
{
public:
    StandardControl(src::Drivers *drivers) : CommonControlManual(drivers) {}

    void initialize()
    {
        CommonControlManual::initialize();

        agitator.initialize();
        odometry.initialize();
        drivers->commandScheduler.registerSubsystem(&agitator);
        drivers->commandScheduler.registerSubsystem(&odometry);

        drivers->commandMapper.addMap(&leftMouseDown);
        drivers->commandMapper.addMap(&leftSwitchUp);
        drivers->commandMapper.addMap(&pressOdom);
    }

private:
    // Subsystems
    AgitatorSubsystem agitator{drivers, &flywheel, AGITATOR};
    OdometrySubsystem odometry{drivers, &chassis, &turret};

    // Commands
    CommandAgitatorContinuous rotateAgitator_LeftMouse{drivers, &agitator, BarrelId::STANDARD1};
    CommandAgitatorContinuous rotateAgitator_SwitchUp{
        drivers,
        &agitator,
        BarrelId::STANDARD1,
        true};
    CommandOdomPoint movePoint{
        drivers, &chassis, &odometry};  

    // Mappings
    HoldCommandMapping leftMouseDown{
        drivers,
        {&rotateAgitator_LeftMouse},
        RemoteMapState(RemoteMapState::MouseButton::LEFT)};

    HoldCommandMapping leftSwitchUp{
        drivers,
        {&rotateAgitator_SwitchUp, &rotateFlywheel_SwitchMid},
        RemoteMapState(Remote::Switch::LEFT_SWITCH, Remote::SwitchState::UP)};

    PressCommandMapping pressOdom{
        drivers,
        {&movePoint},
        RemoteMapState({Remote::Key::B})};
};