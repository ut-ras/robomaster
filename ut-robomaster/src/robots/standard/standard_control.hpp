#include "control/client-display/command_client_display.hpp"
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
        drivers->commandScheduler.registerSubsystem(&client);

        drivers->commandMapper.addMap(&leftMouseDown);
        drivers->commandMapper.addMap(&leftSwitchUp);

        drivers->commandMapper.addMap(&hudTestKey);
    }

private:
    // Subsystems
    AgitatorSubsystem agitator{drivers, &flywheel, AGITATOR};
    ClientDisplaySubsystem client{drivers, &turret, &flywheel};

    // Commands
    CommandAgitatorContinuous rotateAgitator_LeftMouse{drivers, &agitator, BarrelId::STANDARD1};
    CommandAgitatorContinuous rotateAgitator_SwitchUp{
        drivers,
        &agitator,
        BarrelId::STANDARD1,
        true};

    commands::CommandClientDisplay hudTest{drivers, &client};

    // Mappings
    HoldCommandMapping leftMouseDown{
        drivers,
        {&rotateAgitator_LeftMouse},
        RemoteMapState(RemoteMapState::MouseButton::LEFT)};

    HoldCommandMapping leftSwitchUp{
        drivers,
        {&rotateAgitator_SwitchUp, &rotateFlywheel_SwitchMid},
        RemoteMapState(Remote::Switch::LEFT_SWITCH, Remote::SwitchState::UP)};

    HoldCommandMapping hudTestKey{drivers, {&hudTest}, RemoteMapState({Remote::Key::Z})};
};