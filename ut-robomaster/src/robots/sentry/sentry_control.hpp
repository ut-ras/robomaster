#include "robots/common/common_control_manual.hpp"
#include "subsystems/agitator/command_agitator_continuous.hpp"
#include "subsystems/chassis/command_sentry_position.hpp"
#include "subsystems/turret/command_sentry_aim.hpp"

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

        chassis.setDefaultCommand(&sentryPosition);
        turret.setDefaultCommand(&sentryAim);
    }

private:
    // Subsystems
    AgitatorSubsystem agitatorL{drivers, &flywheel, AGITATOR_L};
    AgitatorSubsystem agitatorR{drivers, &flywheel, AGITATOR_R};

    // Commands
    CommandAgitatorContinuous rotateAgitatorL{drivers, &agitatorL, BarrelId::STANDARD1};
    CommandAgitatorContinuous rotateAgitatorR{drivers, &agitatorR, BarrelId::STANDARD2};

    CommandSentryPosition sentryPosition{drivers, &chassis};
    CommandSentryAim sentryAim{drivers, &turret};

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