#pragma once

#include "tap/algorithms/wrapped_float.hpp"
#include "tap/control/subsystem.hpp"

#include "drivers/as5600.hpp"
#include "modm/math/geometry.hpp"
#include "robots/robot_constants.hpp"
#include "subsystems/subsystem.hpp"

#include "double_yaw_motor.hpp"
#include "drivers.hpp"
#include "turret_motor.hpp"

namespace subsystems::turret
{
using driver::As5600;
using modm::Vector3f;
using tap::algorithms::WrappedFloat;

class TurretSubsystem : public Subsystem
{
public:
    TurretSubsystem(src::Drivers* drivers);
    void initialize() override;
    void refresh() override;
    bool hardwareOk() override;

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
    float getMotorAngle();

    void setBaseYaw(float by);

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
    float baseYaw = 0.0f;
};
}  // namespace subsystems::turret
