#pragma once

#include "tap/communication/serial/remote.hpp"
#include "tap/control/command.hpp"

#include "communication/cv_board.hpp"
#include "subsystems/turret/turret_subsystem.hpp"

#include "drivers.hpp"

namespace commands
{
using subsystems::turret::TurretSubsystem;
using tap::communication::serial::Remote;

class CommandSentryAim : public tap::control::Command
{
public:
    CommandSentryAim(src::Drivers* drivers, TurretSubsystem* turret)
        : drivers(drivers),
          turret(turret)
    {
        addSubsystemRequirement(turret);
    }

    void initialize() override;
    void execute() override;
    void end(bool interrupted) override;
    bool isFinished() const override;

    const char* getName() const override { return "sentry autonomous aim"; }

private:
    src::Drivers* drivers;
    TurretSubsystem* turret;
    uint8_t lastTurretDataIndex = 0;

    // Scanning state
    float scanYaw = 0.0f;
    float scanDirection = 1.0f;

    // Tracking state
    bool hasActiveTarget = false;
    float targetLockTimer = 0.0f;
    uint32_t lastCVDataIndex = 0;

    // Tunable parameters
    static constexpr float SCAN_SPEED = 0.3f;           // rad/s
    static constexpr float SCAN_AMPLITUDE = 1.047f;     // ~60 degrees (M_PI/3)
    static constexpr float TARGET_LOCK_TIMEOUT = 0.5f;  // seconds
    static constexpr float DT = 0.001f;                 // 1ms loop period
};

}  // namespace commands