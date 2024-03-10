#pragma once

#include "tap/architecture/periodic_timer.hpp"
#include "tap/communication/sensors/buzzer/buzzer.hpp"
#include "tap/control/command.hpp"

#include "sounds/sound_startup.hpp"

#include "drivers.hpp"
#include "startup_subsystem.hpp"

// #define S 0.0f
// #define C5 523.25f
// #define Cs5 554.37f
// #define Ds5 622.25f
// #define E5 659.25f
// #define F5 698.46f
// #define Fs5 739.99f
// #define G5 783.99f
// #define Gs5 830.61f
// #define A5 880.0f
// #define C6 1046.5f
// #define Cs6 1108.73f

namespace commands
{

using subsystems::startup::StartupSubsystem;
using tap::arch::PeriodicMilliTimer;

class CommandStartup : public tap::control::Command
{
public:
    CommandStartup(src::Drivers *drivers, StartupSubsystem *startup)
        : drivers(drivers),
          startup(startup),
          timer(NOTE_INTERVAL)
    {
        addSubsystemRequirement(startup);
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "startup command"; }

private:
    src::Drivers *drivers;
    StartupSubsystem *startup;

    uint16_t note = 0;
    PeriodicMilliTimer timer;
};  // class CommandStartup
}  // namespace commands