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

    // Scanning state
    float scanYaw = 0.0f;
    float scanDirection = 1.0f;

    // Tracking state
    bool hasActiveTarget = false;
    float targetLockTimer = 0.0f;
    uint8_t lastTurretDataIndex = 0;  // tracks last CV packet index seen

    // Tunable parameters
    static constexpr float SCAN_SPEED = 0.3f;           // rad/s
    static constexpr float SCAN_AMPLITUDE = 1.047f;     // ~60 degrees (M_PI/3)
    static constexpr float TARGET_LOCK_TIMEOUT = 0.5f;  // seconds
    static constexpr float DT = 0.001f;                 // 1ms loop period

    // Boot test sequence — cycles through hardcoded poses before CV takes over.
    // 10 000 ticks × 2 ms/tick = 20 s per pose.
    struct TestPose
    {
        float localYaw;   // rad offset from chassis heading
        float pitch;      // rad (+ = up), clamped by PITCH_MIN/PITCH_MAX
    };
    static constexpr int NUM_TEST_POSES = 5;
    static constexpr int TICKS_PER_POSE = 10000;
    static constexpr TestPose TEST_POSES[NUM_TEST_POSES] = {
        {  0.0f,   0.00f },   // pose 0: center, level
        {  0.7f,   0.20f },   // pose 1: 40° left,  near max pitch up
        { -0.7f,  -0.20f },   // pose 2: 40° right, near max pitch down
        {  0.7f,  -0.20f },   // pose 3: 40° left,  near max pitch down
        { -0.7f,   0.20f },   // pose 4: 40° right, near max pitch up
    };

    int testSeqIndex = 0;
    int testSeqTicks = 0;
};

}  // namespace commands

///////////////////////////////////////////////
// # Only rebuilds changed files (~10 seconds)
// scons build robot=TARGET_SENTRY -j8
// scons run robot=TARGET_SENTRY
///////////////////////////////////////////////