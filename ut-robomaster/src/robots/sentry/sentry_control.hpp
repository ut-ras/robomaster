#include "robots/common/common_control_manual.hpp"

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
    CommandAgitatorContinuous agitatorLContinuousCommand{drivers, &agitatorL, BarrelId::STANDARD1};
    CommandAgitatorContinuous agitatorRContinuousCommand{drivers, &agitatorR, BarrelId::STANDARD2};

    // Mappings
    HoldCommandMapping leftMouseDown{
        drivers,
        {&agitatorLContinuousCommand, &agitatorRContinuousCommand},
        RemoteMapState(RemoteMapState::MouseButton::LEFT)};

    HoldCommandMapping leftSwitchUp{
        drivers,
        {&agitatorLContinuousCommand,
         &agitatorRContinuousCommand,
         &rotateFlywheelWithAgitatorCommand},
        RemoteMapState(Remote::Switch::LEFT_SWITCH, Remote::SwitchState::UP)};
};