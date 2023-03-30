#ifndef TURRET_SUBSYSTEM_HPP_
#define TURRET_SUBSYSTEM_HPP_

#include "tap/control/subsystem.hpp"

#include "modm/math/geometry.hpp"
#include "turret_motor.hpp"

#include "drivers.hpp"

using modm::Vector3f;

namespace subsystems
{
namespace turret
{
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

    void setDesiredAngles(float yaw, float pitch);

    /// @brief Input target data from CV (relative to camera)
    void inputTargetData(Vector3f position, Vector3f velocity, Vector3f acceleration);

    void setAimStrategy(AimStrategy aimStrategy);
    void refresh() override;

    void runHardwareTests() override;

    const char* getName() override { return "Turret subsystem"; }

    TurretMotor getYawTurret() { return yawTurret; }
    TurretMotor getPitchTurret() { return pitchTurret; }

private:
    static constexpr float PID_P_KP = 0.1f;
    static constexpr float PID_P_KI = 0.1f;
    static constexpr float PID_P_KD = 0.0f;
    static constexpr float PID_V_KP = 0.75f;
    static constexpr float PID_V_KI = 12.0f;
    static constexpr float PID_V_KD = 0.0f;

    float desiredYaw;
    float desiredPitch;

    Vector3f targetPosition = Vector3f(0.0f);
    Vector3f targetVelocity = Vector3f(0.0f);
    Vector3f targetAcceleration = Vector3f(0.0f);

    AimStrategy aimStrategy = AimStrategy::Manual;

    tap::motor::DjiMotor yawMotor;
    tap::motor::DjiMotor pitchMotor;
    TurretMotor yawTurret;
    TurretMotor pitchTurret;

    // From aruw-mcb, should test to find our own values: 
    // https://gitlab.com/aruw/controls/aruw-mcb/-/blob/develop/aruw-mcb-project/src/aruwsrc/robot/standard/standard_turret_constants.hpp
    static constexpr tap::algorithms::SmoothPidConfig YAW_PID_CONFIG = {
        .kp = 229'183.1f,
        .ki = 0.0f,
        .kd = 10'886.2f,
        .maxICumulative = 0.0f,
        .maxOutput = 32'000.0f,
        .tQDerivativeKalman = 1.0f,
        .tRDerivativeKalman = 30.0f,
        .tQProportionalKalman = 1.0f,
        .tRProportionalKalman = 0.0f,
        .errDeadzone = 0.0f,
        .errorDerivativeFloor = 0.0f,
    };

    static constexpr tap::algorithms::SmoothPidConfig PITCH_PID_CONFIG = {
        .kp = 229'183.1f,
        .ki = 0.0f,
        .kd = 7'448.5f,
        .maxICumulative = 0.0f,
        .maxOutput = 32000.0f,
        .tQDerivativeKalman = 1.0f,
        .tRDerivativeKalman = 10.0f,
        .tQProportionalKalman = 1.0f,
        .tRProportionalKalman = 2.0f,
        .errDeadzone = 0.0f,
        .errorDerivativeFloor = 0.0f,
    };
};

}  // namespace turret
}  // namespace subsystems

#endif
