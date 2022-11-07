#include "tap/control/subsystem.hpp"
#include "tap/motor/dji_motor.hpp"

#include "modm/math/filter/pid.hpp"
#include "modm/math/geometry.hpp"

#include "drivers.hpp"

using namespace tap::motor;
using namespace tap::communication::sensors::imu;

namespace subsystems
{
namespace chassis
{
class ChassisSubsystem : public tap::control::Subsystem
{
public:
    ChassisSubsystem(
        tap::Drivers* drivers,
        const DjiMotor* yawMotor,
        MotorId rightFrontMotorId = MOTOR1,
        MotorId leftFrontMotorId = MOTOR2,
        MotorId leftBackMotorId = MOTOR3,
        MotorId rightBackMotorId = MOTOR4);

    void initialize() override;
    void setVelocities(float vX, float vY, float wZ);
    void refresh() override;
    void runHardwareTests() override;

    const char* getName() override { return "Chassis subsystem"; }
    bool isBeyblading;
    static constexpr float TRANSLATION_TO_ROTATION_RATIO = 0.5f;

private:
    tap::Drivers* drivers;
    static constexpr tap::can::CanBus CAN_BUS_MOTORS = tap::can::CanBus::CAN_BUS1;
    static constexpr int WHEELS = 4;
    static constexpr float MAX_SPEED = 500;
    static constexpr float PID_KP = 22.0f;
    static constexpr float PID_KI = 0.2f;
    static constexpr float PID_KD = 0.0f;
    static constexpr float PID_MAX_ERROR_SUM = 5000.0f;
    static constexpr float PID_MAX_OUTPUT = 16000.0f;
    static constexpr float PID_MIN_OUTPUT = 1000.0f;
    tap::motor::DjiMotor wheelMotors[4];
    float targetWheelVels[4];
    modm::Pid<float> pids[4];

    bool imuDrive;

    const DjiMotor* yawMotor;

    bool setStartTurret;
    float startTurretLoc;
    void updateMotor(modm::Pid<float>* pid, DjiMotor* motor, float target);
};
}  // namespace chassis
}  // namespace subsystems