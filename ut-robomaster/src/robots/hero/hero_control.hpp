#include "robots/common/common_control_manual.hpp"

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
    AgitatorSubsystem agitator{drivers, AGITATOR};

    // Commands
    CommandAgitatorContinuous agitatorContinuousCommand{drivers, &agitator, BarrelId::HERO};

    // Mappings
    HoldCommandMapping leftMouseDown{
        drivers,
        {&agitatorContinuousCommand},
        RemoteMapState(RemoteMapState::MouseButton::LEFT)};

    HoldCommandMapping leftSwitchUp{
        drivers,
        {&agitatorContinuousCommand, &rotateFlywheelWithAgitatorCommand},
        RemoteMapState(Remote::Switch::LEFT_SWITCH, Remote::SwitchState::UP)};
};