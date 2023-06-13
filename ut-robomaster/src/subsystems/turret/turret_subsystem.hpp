#ifndef TURRET_SUBSYSTEM_HPP_
#define TURRET_SUBSYSTEM_HPP_

#include "tap/algorithms/contiguous_float.hpp"
#include "tap/control/subsystem.hpp"

#include "modm/math/filter/moving_average.hpp"
#include "modm/math/geometry.hpp"

#include "drivers.hpp"
#include "turret_motor.hpp"

using modm::Vector3f;

namespace subsystems
{
namespace turret
{
using tap::algorithms::ContiguousFloat;
using tap::motor::DjiMotor;

enum AimStrategy
{
    Manual,
    AutoAim,
    AimAssist
};

class TurretSubsystem : public tap::control::Subsystem
{
public:
    TurretSubsystem(src::Drivers* drivers);

    void initialize() override;

    void inputManualAngles(float yaw, float pitch);

    /// @brief Input target data from CV (relative to camera)
    void inputTargetData(Vector3f position, Vector3f velocity, Vector3f acceleration);

    void setAimStrategy(AimStrategy aimStrategy);
    AimStrategy getAimStrategy();

    float getChassisYaw();
    float getTargetLocalYaw();
    float getTargetLocalPitch();
    float getCurrentLocalYaw();
    float getCurrentLocalPitch();

    void refresh() override;

    void runHardwareTests() override;

    const char* getName() override { return "Turret subsystem"; }

    TurretMotor* getYawTurret() { return &yawTurret; }
    TurretMotor* getPitchTurret() { return &pitchTurret; }

    static constexpr float BELT_RATIO = 2.0f;

private:
    src::Drivers* drivers;

    float targetWorldYaw = 0.0f;
    float targetWorldPitch = 0.0f;
    float inputYaw = 0.0f;
    float inputPitch = 0.0f;
    uint32_t lastTime;

    Vector3f targetPosition = Vector3f(0.0f);
    Vector3f targetVelocity = Vector3f(0.0f);
    Vector3f targetAcceleration = Vector3f(0.0f);

    AimStrategy aimStrategy = AimStrategy::Manual;

    DjiMotor yawMotor;
    DjiMotor pitchMotor;

    TurretMotor yawTurret;
    TurretMotor pitchTurret;

    float isCalibrated = false;
    float basePitch = 0.0f;
    float baseYaw = 0.0f;

    ContiguousFloat turretOffset;

#ifdef TARGET_STANDARD
    static constexpr tap::algorithms::SmoothPidConfig YAW_PID_CONFIG = {
        .kp = 65'000.0f,
        .ki = 0.0f,
        .kd = 3'000.0f,
        .maxICumulative = 0.0f,
        .maxOutput = 32'000.0f,
        .tQDerivativeKalman = 1.0f,
        .tRDerivativeKalman = 30.0f,
        .tQProportionalKalman = 1.0f,
        .tRProportionalKalman = 0.0f,
        .errDeadzone = 0.0f,
        .errorDerivativeFloor = 10.0f,
    };

    static constexpr tap::algorithms::SmoothPidConfig PITCH_PID_CONFIG = {
        .kp = 100'183.1f,
        .ki = 0.0f,
        .kd = 1'000.0f,
        .maxICumulative = 0.0f,
        .maxOutput = 32000.0f,
        .tQDerivativeKalman = 1.0f,
        .tRDerivativeKalman = 30.0f,
        .tQProportionalKalman = 1.0f,
        .tRProportionalKalman = 0.0f,
        .errDeadzone = 0.0f,
        .errorDerivativeFloor = 0.0f,
    };
#endif

#ifdef TARGET_HERO
    static constexpr tap::algorithms::SmoothPidConfig YAW_PID_CONFIG = {
        .kp = 100'183.1f,
        .ki = 0.0f,
        .kd = 2'500.0f,
        .maxICumulative = 0.0f,
        .maxOutput = 32'000.0f,
        .tQDerivativeKalman = 1.0f,
        .tRDerivativeKalman = 30.0f,
        .tQProportionalKalman = 1.0f,
        .tRProportionalKalman = 0.0f,
        .errDeadzone = 0.0f,
        .errorDerivativeFloor = 10.0f,
    };

    static constexpr tap::algorithms::SmoothPidConfig PITCH_PID_CONFIG = {
        .kp = 100'183.1f,
        .ki = 0.0f,
        .kd = 1'000.0f,
        .maxICumulative = 0.0f,
        .maxOutput = 32000.0f,
        .tQDerivativeKalman = 1.0f,
        .tRDerivativeKalman = 30.0f,
        .tQProportionalKalman = 1.0f,
        .tRProportionalKalman = 0.0f,
        .errDeadzone = 0.0f,
        .errorDerivativeFloor = 0.0f,
    };
#endif
};

}  // namespace turret
}  // namespace subsystems

#endif
