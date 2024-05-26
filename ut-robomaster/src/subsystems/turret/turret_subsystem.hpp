#pragma once

#include "tap/algorithms/contiguous_float.hpp"
#include "tap/control/subsystem.hpp"

#include "drivers/as5600.hpp"
#include "modm/math/filter/moving_average.hpp"
#include "modm/math/geometry.hpp"
#include "robots/robot_constants.hpp"

#include "double_yaw_motor.hpp"
#include "drivers.hpp"
#include "turret_motor.hpp"

using modm::Vector3f;

namespace subsystems
{
namespace turret
{
using driver::As5600;
using tap::algorithms::ContiguousFloat;

class TurretSubsystem : public tap::control::Subsystem
{
public:
    TurretSubsystem(src::Drivers* drivers);
    void initialize() override;

    /// @brief Input target data from CV (relative to camera)
    void inputTargetData(Vector3f position, Vector3f velocity, Vector3f acceleration);

    void setTargetWorldAngles(float yaw, float pitch);

    float getChassisYaw();

    float getTargetLocalYaw();

    float getTargetLocalPitch();

    float getTargetWorldYaw() { return targetWorldYaw; }

    float getTargetWorldPitch() { return targetWorldPitch; }

    float getCurrentLocalYaw();

    float getCurrentLocalPitch();

    bool getIsCalibrated();

    void refresh() override;

    void runHardwareTests() override;

    const char* getName() override { return "Turret subsystem"; }

private:
    src::Drivers* drivers;

    float targetWorldYaw = 0.0f;
    float targetWorldPitch = 0.0f;

    Vector3f targetPosition = Vector3f(0.0f);
    Vector3f targetVelocity = Vector3f(0.0f);
    Vector3f targetAcceleration = Vector3f(0.0f);

#if defined(TARGET_STANDARD) || defined(TARGET_HERO)
    As5600 yawEncoder;
    DoubleYawMotor yaw;
#else
    TurretMotor yaw;
#endif
    TurretMotor pitch;

    float isCalibrated = false;
    float basePitch = 0.0f;
    float baseYaw = 0.0f;

    ContiguousFloat turretOffset;
};

}  // namespace turret
}  // namespace subsystems
