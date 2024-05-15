#pragma once

#include "tap/algorithms/contiguous_float.hpp"
#include "tap/control/subsystem.hpp"

#include "modm/math/filter/moving_average.hpp"
#include "modm/math/geometry.hpp"
#include "robots/robot_constants.hpp"

#include "drivers.hpp"
#include "turret_motor.hpp"

// include double yaw motor, encoder
#include "drivers/as5600.hpp"
#include "double_yaw_motor.hpp"

using modm::Vector3f;

namespace subsystems
{
namespace turret
{
using driver::As5600;
using modm::platform::I2cMaster2;
using tap::algorithms::ContiguousFloat;
using tap::motor::DjiMotor;

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

    DjiMotor yawMotor;
    DjiMotor pitchMotor;



    TurretMotor yawTurret;
    
    TurretMotor pitchTurret;

    float isCalibrated = false;
    float basePitch = 0.0f;
    float baseYaw = 0.0f;

    ContiguousFloat turretOffset;

   #if defined(TARGET_STANDARD) || defined(TARGET_HERO)
        DoubleYawMotor doubleYawTurret;
        As5600<I2cMaster2> encoder;
   #endif
    
};

}  // namespace turret
}  // namespace subsystems

