#include "tap/control/hold_command_mapping.hpp"

#include "subsystems/flywheel/command_rotate_flywheel.hpp"
#include "subsystems/flywheel/flywheel_subsystem.hpp"
#include "subsystems/sound/command_play_sound.hpp"
#include "subsystems/sound/sound_subsystem.hpp"

#include "drivers.hpp"

using namespace tap::control;
using namespace tap::communication::serial;

using namespace subsystems::flywheel;
using namespace subsystems::sound;

using namespace commands;

class StandardControl
{
public:
    StandardControl(src::Drivers* drivers) : drivers{drivers} {}

    void initialize()
    {
        sound.initialize();
        drivers->commandScheduler.registerSubsystem(&sound);

        // Initialize and register the flywheel subsystem
        // flywheel.initialize();
        // drivers->commandScheduler.registerSubsystem(&flywheel);

        // Register the command mapping
        // drivers->commandMapper.addMap(&leftSwitchUp);
    }

private:
    src::Drivers* drivers;
    SoundSubsystem sound{drivers};
    CommandPlaySound playStartupSound{drivers, &sound, SOUND_STARTUP};

    // Define flywheel subsystem, command, and input mapping here

    // FlywheelSubsystem flywheel{drivers};

    // CommandRotateFlywheel rotateFlywheels{drivers, &flywheel};

    // HoldCommandMapping leftSwitchUp{
    //     drivers,
    //     {&rotateFlywheels},
    //     RemoteMapState(Remote::Switch::LEFT_SWITCH, Remote::SwitchState::UP)};
};