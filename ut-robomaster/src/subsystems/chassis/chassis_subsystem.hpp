#ifndef CHASSIS_SUBSYSTEM_HPP_
#define CHASSIS_SUBSYSTEM_HPP_

#include "tap/control/subsystem.hpp"

#include "modm/math/geometry.hpp"
#include "utils/motor_controller/motor_controller.hpp"

#include "robots/robot_constants.hpp"

#include "drivers.hpp"

using namespace tap::communication::sensors::imu;
using namespace modm;
using namespace motor_controller;

namespace subsystems
{
namespace chassis
{
class ChassisSubsystem : public tap::control::Subsystem
{
public:
    ChassisSubsystem(src::Drivers* drivers);

    void initialize() override;

    void refresh() override;

    void runHardwareTests() override;

    /// @brief Update robot motion based on simple input controls. Inputs are scaled and corrected
    /// to avoid over-driving motors. This logic can be adjusted to create various input schemes.
    /// @param move Linear movement (magnitude should be within [0,1])
    /// @param spin Angular rotation (value should be within [-1,1])
    void input(Vector2f move, float spin);

    /// @brief Reconstruct current velocities based on measured wheel motion.
    /// @return x,y is linear velocity (m/s) and z is angular velocity (rad/s)
    Vector3f measureVelocity();

    const char* getName() override { return "Chassis subsystem"; }

private:
    src::Drivers* drivers;
    MotorVelocityController wheels[WHEELS];
    float targetWheelVels[WHEELS] = {0.0f, 0.0f, 0.0f, 0.0f};

    /// @brief Calculate and set wheel velocities for desired robot motion (based on
    /// https://research.ijcaonline.org/volume113/number3/pxc3901586.pdf).
    /// @param v Linear velocity (m/s)
    /// @param wZ Angular velocity (rad/s)
    void setMecanumWheelVelocities(Vector2f v, float wZ);
};
}  // namespace chassis
}  // namespace subsystems

#endif
