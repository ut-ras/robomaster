#ifndef TURRET_SUBSYSTEM_HPP_
#define TURRET_SUBSYSTEM_HPP_

#include "tap/algorithms/contiguous_float.hpp"
#include "tap/control/subsystem.hpp"

#include "modm/math/filter/moving_average.hpp"
#include "modm/math/geometry.hpp"

#include "drivers.hpp"
#include "turret_motor.hpp"
#include "robots/robot_constants.hpp"

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

    float getInputYaw() { return inputYaw; }

    float getInputPitch() { return inputPitch; }

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
};

}  // namespace turret
}  // namespace subsystems

#endif
