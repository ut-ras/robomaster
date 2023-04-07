#ifndef TURRET_SUBSYSTEM_HPP_
#define TURRET_SUBSYSTEM_HPP_

#include "tap/control/subsystem.hpp"

#include "tap/algorithms/contiguous_float.hpp"

#include "modm/math/geometry.hpp"
#include "modm/math/filter/moving_average.hpp"
#include "turret_motor.hpp"

#include "drivers.hpp"

using modm::Vector3f;

namespace src {
    class Drivers;
}

// class TurretMotor;

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

    float getChassisOffset();
    float getTurretWithOffset();
    void refresh() override;

    void runHardwareTests() override;

    const char* getName() override { return "Turret subsystem"; }

    TurretMotor* getYawTurret() { return &yawTurret; }
    TurretMotor* getPitchTurret() { return &pitchTurret; }

    float getPreviousChassisRelativeYawSetpoint() { return previousYawSetpoint; }
    void setPreviousChassisRelativeYawSetpoint(float setpoint) { this->previousYawSetpoint = setpoint; } 

    static constexpr float BELT_RATIO = 2.0f;

    float getPreviousChassisRelativeYawSetpoint() { return previousYawSetpoint; }
    void setPreviousChassisRelativeYawSetpoint(float setpoint) { this->previousYawSetpoint = setpoint; } 

    static constexpr float BELT_RATIO = 2.0f;

private:
    float desiredYaw;
    float desiredPitch;

    float initialChassisYaw;
    float previousChassisYaw;   //  Terminating to check if current is equal to previous (we not moving)
    tap::algorithms::ContiguousFloat offset = tap::algorithms::ContiguousFloat(0.0f, 0.0f, M_TWOPI);

    Vector3f targetPosition = Vector3f(0.0f);
    Vector3f targetVelocity = Vector3f(0.0f);
    Vector3f targetAcceleration = Vector3f(0.0f);

    AimStrategy aimStrategy = AimStrategy::Manual;

    tap::motor::DjiMotor yawMotor;
    tap::motor::DjiMotor pitchMotor;
    
    TurretMotor yawTurret;
    TurretMotor pitchTurret;

    float initialChassisYaw;
    float initialTurretYaw;

    float previousYawSetpoint;

    tap::algorithms::ContiguousFloat turretOffset;

    // From aruw-mcb, should test to find our own values: 
    // https://gitlab.com/aruw/controls/aruw-mcb/-/blob/develop/aruw-mcb-project/src/aruwsrc/robot/standard/standard_turret_constants.hpp
    static constexpr tap::algorithms::SmoothPidConfig YAW_PID_CONFIG = {
        .kp = 229'183.1f,
        .ki = 0.0f,
        .kd = 2'500.0f,
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
};

}  // namespace turret
}  // namespace subsystems

#endif
