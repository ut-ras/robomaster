#ifndef CHASSIS_SUBSYSTEM_HPP_
#define CHASSIS_SUBSYSTEM_HPP_

#include "tap/control/subsystem.hpp"
#include "tap/motor/dji_motor.hpp"

#include "modm/math/filter/pid.hpp"
#include "modm/math/geometry.hpp"

#include "drivers.hpp"

using namespace tap::motor;
using namespace tap::communication::sensors::imu;
using namespace modm;

namespace subsystems
{
namespace chassis
{
class ChassisSubsystem : public tap::control::Subsystem
{
public:
    ChassisSubsystem(
        tap::Drivers* drivers,
        MotorId leftFrontMotorId = MOTOR2,
        MotorId rightFrontMotorId = MOTOR1,
        MotorId leftBackMotorId = MOTOR3,
        MotorId rightBackMotorId = MOTOR4);

    void initialize() override;
    void refresh() override;
    void runHardwareTests() override;

    /// @brief Update robot motion based on simple input controls. Inputs are scaled and corrected
    /// to avoid over-driving motors. This logic can be adjusted to create various input schemes.
    /// @param move Linear movement (magnitude should be within [0,1])
    /// @param spin Angular rotation (value should be within [-1,1])
    void input(Vector2f move, float spin);

    const char* getName() override { return "Chassis subsystem"; }

private:
    tap::Drivers* drivers;
    static constexpr tap::can::CanBus CAN_BUS_WHEELS = tap::can::CanBus::CAN_BUS1;

    static constexpr float PID_KP = 22.0f;
    static constexpr float PID_KI = 0.2f;
    static constexpr float PID_KD = 0.0f;
    static constexpr float PID_MAX_ERROR_SUM = 5000.0f;
    static constexpr float PID_MAX_OUTPUT = 16000.0f;
    static constexpr float PID_MIN_OUTPUT = 1000.0f;

    static constexpr int WHEELS = 4;
    static constexpr float WHEEL_DISTANCE_X = 0.4064f;  // meters
    static constexpr float WHEEL_DISTANCE_Y = 0.508f;   // meters
    static constexpr float WHEEL_RADIUS = 0.1524f;      // meters
    static constexpr float WHEEL_LXY = (WHEEL_DISTANCE_X + WHEEL_DISTANCE_Y) / 2.0f;
    static constexpr float WHEEL_MAX_VEL = 200.0f;                                      // rad/s
    static constexpr float MAX_LINEAR_VEL = WHEEL_MAX_VEL * WHEEL_RADIUS;               // m/s
    static constexpr float MAX_ANGULAR_VEL = WHEEL_MAX_VEL * WHEEL_RADIUS / WHEEL_LXY;  // rad/s

    DjiMotor wheelMotors[4];
    float targetWheelVels[4];
    modm::Pid<float> pids[4];

    bool imuDrive;
    bool setStartTurret;
    float startTurretLoc;

    /// @brief Calculate and set wheel velocities for desired robot motion (based on
    /// https://research.ijcaonline.org/volume113/number3/pxc3901586.pdf).
    /// @param v Linear velocity (m/s)
    /// @param wZ Angular velocity (rad/s)
    void setMecanumWheelVelocities(Vector2f v, float wZ);

    void updateMotor(modm::Pid<float>* pid, DjiMotor* motor, float target);
};
}  // namespace chassis
}  // namespace subsystems

#endif
