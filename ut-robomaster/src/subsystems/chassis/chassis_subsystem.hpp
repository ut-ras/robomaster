#include "tap/control/chassis/chassis_subsystem_interface.hpp"
#include "tap/control/command_scheduler.hpp"
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
        DjiMotor* yawMotor,
        MotorId leftFrontMotorId = MOTOR1,
        MotorId rightFrontMotorId = MOTOR2,
        MotorId leftBackMotorId = MOTOR3,
        MotorId rightBackMotorId = MOTOR4);

    void initialize() override;
    void setVelocities(float vX, float vY, float wZ);
    void refresh() override;
    void runHardwareTests() override;

    const char* getName() override { return "Chassis subsystem"; }

private:
    static constexpr tap::can::CanBus CAN_BUS_MOTORS = tap::can::CanBus::CAN_BUS1;
    static const int WHEELS = 4;
    static const float MAX_SPEED = 8000;
    static const float PID_KP = 5.0f;
    static const float PID_KI = 0.0f;
    static const float PID_KD = 1.0f;
    static const float PID_MAX_ERROR_SUM = 0.0f;
    static const float PID_MAX_OUTPUT = 16000;

    DjiMotor wheelMotors[4];
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