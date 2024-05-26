#include "robots/common/common_control_manual.hpp"
#include "subsystems/agitator/command_agitator_continuous.hpp"

class StandardControl : CommonControlManual
{
public:
    StandardControl(src::Drivers *drivers) : CommonControlManual(drivers) {}

    void initialize()
    {
        CommonControlManual::initialize();

        agitator.initialize();
        drivers->commandScheduler.registerSubsystem(&agitator);

        drivers->commandMapper.addMap(&leftMouseDown);
        drivers->commandMapper.addMap(&leftSwitchUp);
    }

private:
    // Subsystems
    AgitatorSubsystem agitator{drivers, AGITATOR};

    // Commands
    CommandAgitatorContinuous rotateAgitator{drivers, &agitator, BarrelId::STANDARD1};

    // Mappings
    HoldCommandMapping leftMouseDown{
        drivers,
        {&rotateAgitator},
        RemoteMapState(RemoteMapState::MouseButton::LEFT)};

    HoldCommandMapping leftSwitchUp{
        drivers,
        {&rotateAgitator, &rotateFlywheel_SwitchMid},
        RemoteMapState(Remote::Switch::LEFT_SWITCH, Remote::SwitchState::UP)};
};