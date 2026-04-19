#include "tap/control/command_mapper.hpp"
#include "tap/control/hold_command_mapping.hpp"
#include "tap/control/hold_repeat_command_mapping.hpp"
#include "tap/control/press_command_mapping.hpp"
#include "tap/control/toggle_command_mapping.hpp"

#include "robots/robot_constants.hpp"
#include "subsystems/agitator/agitator_subsystem.hpp"
#include "subsystems/chassis/chassis_subsystem.hpp"
#include "subsystems/flywheel/flywheel_subsystem.hpp"
#include "subsystems/odometry/odometry_subsystem.hpp"
#include "subsystems/sound/command_play_sound.hpp"
#include "subsystems/sound/sound_subsystem.hpp"
#include "subsystems/turret/turret_subsystem.hpp"
#include "utils/power_limiter/barrel_cooldown.hpp"

#include "drivers.hpp"

using namespace tap::control;
using namespace tap::communication::serial;

using namespace subsystems::chassis;
using namespace subsystems::agitator;
using namespace subsystems::flywheel;
using namespace subsystems::turret;
using namespace subsystems::sound;
using namespace subsystems::odometry;

using namespace commands;
using power_limiter::BarrelId;

class CommonControl
{
protected:
    CommonControl(src::Drivers* drivers) : drivers(drivers) {}

    void initialize()
    {
        // Initialize subsystems
        chassis.initialize();
        flywheel.initialize();
        turret.initialize();
        sound.initialize();
        odometry.initialize();
        // Register subsystems
        drivers->commandScheduler.registerSubsystem(&chassis);
        drivers->commandScheduler.registerSubsystem(&flywheel);
        drivers->commandScheduler.registerSubsystem(&turret);
        drivers->commandScheduler.registerSubsystem(&sound);
        drivers->cvBoard.setOdometry(&odometry);
        // Run startup commands
        drivers->commandScheduler.addCommand(&playStartupSound);
    }

    src::Drivers* drivers;

    // Subsystems
    ChassisSubsystem chassis{drivers};
    FlywheelSubsystem flywheel{drivers};
    TurretSubsystem turret{drivers};
    SoundSubsystem sound{drivers};
    OdometrySubsystem odometry{drivers, &chassis, &turret};

    // Commands
    CommandPlaySound playStartupSound{drivers, &sound, SOUND_STARTUP};
};