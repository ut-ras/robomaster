#include "robots/common/common_control_manual.hpp"
#include "subsystems/agitator/command_agitator_continuous.hpp"

class HeroControl : CommonControlManual
{
public:
    HeroControl(src::Drivers *drivers) : CommonControlManual(drivers) {}

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
    AgitatorSubsystem agitator{drivers, &flywheel, AGITATOR};

    // Commands
    CommandAgitatorContinuous rotateAgitator_LeftMouse{drivers, &agitator, BarrelId::HERO};
    CommandAgitatorContinuous rotateAgitator_SwitchUp{drivers, &agitator, BarrelId::HERO, true};

    // Mappings
    HoldCommandMapping leftMouseDown{
        drivers,
        {&rotateAgitator_LeftMouse},
        RemoteMapState(RemoteMapState::MouseButton::LEFT)};

    HoldCommandMapping leftSwitchUp{
        drivers,
        {&rotateAgitator_SwitchUp, &rotateFlywheel_SwitchMid},
        RemoteMapState(Remote::Switch::LEFT_SWITCH, Remote::SwitchState::UP)};
};