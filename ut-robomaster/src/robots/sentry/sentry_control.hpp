#include "robots/common/common_control_manual.hpp"
#include "subsystems/agitator/command_agitator_continuous.hpp"

class SentryControl : CommonControlManual
{
public:
    SentryControl(src::Drivers *drivers) : CommonControlManual(drivers) {}

    void initialize()
    {
        CommonControlManual::initialize();

        agitatorL.initialize();
        drivers->commandScheduler.registerSubsystem(&agitatorL);

        agitatorR.initialize();
        drivers->commandScheduler.registerSubsystem(&agitatorR);

        drivers->commandMapper.addMap(&leftMouseDown);
        drivers->commandMapper.addMap(&leftSwitchUp);
    }

private:
    // Subsystems
    AgitatorSubsystem agitatorL{drivers, AGITATOR_L};
    AgitatorSubsystem agitatorR{drivers, AGITATOR_R};

    // Commands
    CommandAgitatorContinuous rotateAgitatorL{drivers, &agitatorL, BarrelId::STANDARD1};
    CommandAgitatorContinuous rotateAgitatorR{drivers, &agitatorR, BarrelId::STANDARD2};

    // Mappings
    HoldCommandMapping leftMouseDown{
        drivers,
        {&rotateAgitatorL, &rotateAgitatorR},
        RemoteMapState(RemoteMapState::MouseButton::LEFT)};

    HoldCommandMapping leftSwitchUp{
        drivers,
        {&rotateAgitatorL, &rotateAgitatorR, &rotateFlywheel_SwitchMid},
        RemoteMapState(Remote::Switch::LEFT_SWITCH, Remote::SwitchState::UP)};
};