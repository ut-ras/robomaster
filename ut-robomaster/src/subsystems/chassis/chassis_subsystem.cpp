#include "chassis_subsystem.hpp"

namespace subsystems
{
namespace chassis
{
ChassisSubsystem::ChassisSubsystem(
    tap::Drivers* drivers,
    const DjiMotor* yawMotor,
    MotorId leftFrontMotorId,
    MotorId rightFrontMotorId,
    MotorId leftBackMotorId,
    MotorId rightBackMotorId)
    : tap::control::Subsystem(drivers),
      drivers(drivers),
      wheelMotors{
          DjiMotor(drivers, leftFrontMotorId, CAN_BUS_WHEELS, true, "left front motor"),
          DjiMotor(drivers, rightFrontMotorId, CAN_BUS_WHEELS, false, "right front motor"),
          DjiMotor(drivers, leftBackMotorId, CAN_BUS_WHEELS, true, "left back motor"),
          DjiMotor(drivers, rightBackMotorId, CAN_BUS_WHEELS, false, "right back motor"),
      },
      targetWheelVels{0.0f, 0.0f, 0.0f, 0.0f},
      pids{
          Pid<float>(PID_KP, PID_KI, PID_KD, PID_MAX_ERROR_SUM, PID_MAX_OUTPUT),
          Pid<float>(PID_KP, PID_KI, PID_KD, PID_MAX_ERROR_SUM, PID_MAX_OUTPUT),
          Pid<float>(PID_KP, PID_KI, PID_KD, PID_MAX_ERROR_SUM, PID_MAX_OUTPUT),
          Pid<float>(PID_KP, PID_KI, PID_KD, PID_MAX_ERROR_SUM, PID_MAX_OUTPUT)},
      yawMotor(yawMotor),
      imuDrive(false),
      setStartTurret(false),
      startTurretLoc(0.0f)
{
}

void ChassisSubsystem::initialize()
{
    for (int8_t i = 0; i < WHEELS; i++)
    {
        wheelMotors[i].initialize();
    }
}

void ChassisSubsystem::refresh()
{
    for (int8_t i = 0; i < WHEELS; i++)
    {
        updateMotor(&pids[i], &wheelMotors[i], targetWheelVels[i]);
    }
}

void ChassisSubsystem::runHardwareTests()
{
    // TODO
}

void ChassisSubsystem::input(Vector2f move, float spin)
{
    Vector2f v = move * MAX_LINEAR_VEL;
    float wZ = spin * MAX_ANGULAR_VEL;
    float linearTerm = (abs(v.x) + abs(v.y)) / WHEEL_RADIUS;
    float angularTerm = abs(wZ) * WHEEL_LXY / WHEEL_RADIUS;

    // overdrive error
    float overdrive = max(linearTerm + angularTerm - WHEEL_MAX_VEL, 0.0f);

    // linear velocity correction
    if (linearTerm > 0.0f)
    {
        float correction = min(linearTerm, overdrive);
        v *= 1.0f - correction / linearTerm;
        overdrive -= correction;
    }

    // angular velocity correction
    if (angularTerm > 0.0f)
    {
        float correction = min(angularTerm, overdrive);
        wZ *= 1.0f - correction / angularTerm;
        overdrive -= correction;
    }

    setMecanumWheelVelocities(v, wZ);
}

void ChassisSubsystem::setMecanumWheelVelocities(Vector2f v, float wZ)
{
    // if (imuDrive && yawMotor->isMotorOnline() &&
    //     drivers->bmi088.getImuState() == ImuInterface::ImuState::IMU_CALIBRATED)
    // {
    //     float yawAngle = yawMotor->getEncoderUnwrapped() / 8192 * M_TWOPI;

    //     if (!setStartTurret)
    //     {
    //         startTurretLoc = yawAngle;
    //         setStartTurret = true;
    //     }

    //     v.rotate(yawAngle - startTurretLoc);
    // }

    // x and y are flipped so that y is forward/back and x is left/right
    targetWheelVels[0] = (-v.y - v.x - wZ * WHEEL_LXY) / WHEEL_RADIUS;  // rad/s
    targetWheelVels[1] = (-v.y + v.x + wZ * WHEEL_LXY) / WHEEL_RADIUS;  // rad/s
    targetWheelVels[2] = (-v.y + v.x - wZ * WHEEL_LXY) / WHEEL_RADIUS;  // rad/s
    targetWheelVels[3] = (-v.y - v.x + wZ * WHEEL_LXY) / WHEEL_RADIUS;  // rad/s
}

void ChassisSubsystem::updateMotor(Pid<float>* pid, DjiMotor* motor, float targetVelocity)
{
    pid->update(targetVelocity * 30.0f / M_PI - motor->getShaftRPM());
    float val = pid->getValue();

    if (abs(val) < PID_MIN_OUTPUT)
    {
        val = 0.0f;
    }

    motor->setDesiredOutput(val);
}
}  // namespace chassis
}  // namespace subsystems
