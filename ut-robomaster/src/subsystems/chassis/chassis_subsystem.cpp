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
          DjiMotor(drivers, leftFrontMotorId, CAN_BUS_MOTORS, true, "left front motor"),
          DjiMotor(drivers, rightFrontMotorId, CAN_BUS_MOTORS, false, "right front motor"),
          DjiMotor(drivers, leftBackMotorId, CAN_BUS_MOTORS, true, "left back motor"),
          DjiMotor(drivers, rightBackMotorId, CAN_BUS_MOTORS, false, "right back motor"),
      },
      targetWheelVels{0.0f, 0.0f, 0.0f, 0.0f},
      pids{
          modm::Pid<float>(PID_KP, PID_KI, PID_KD, PID_MAX_ERROR_SUM, PID_MAX_OUTPUT),
          modm::Pid<float>(PID_KP, PID_KI, PID_KD, PID_MAX_ERROR_SUM, PID_MAX_OUTPUT),
          modm::Pid<float>(PID_KP, PID_KI, PID_KD, PID_MAX_ERROR_SUM, PID_MAX_OUTPUT),
          modm::Pid<float>(PID_KP, PID_KI, PID_KD, PID_MAX_ERROR_SUM, PID_MAX_OUTPUT)},
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

// math from https://research.ijcaonline.org/volume113/number3/pxc3901586.pdf
void ChassisSubsystem::setVelocities(Vector2f v, float wZ)
{
    // ImuInterface::ImuState imuState = drivers->bmi088.getImuState();

    // if (imuDrive && imuState == ImuInterface::ImuState::IMU_CALIBRATED)
    // {
    //     if (!setStartTurret && yawMotor->isMotorOnline())
    //     {
    //         startTurretLoc = yawMotor->getEncoderUnwrapped();
    //         setStartTurret = true;
    //     }
    //     float turretOffset =
    //         modm::toRadian((yawMotor->getEncoderUnwrapped() - startTurretLoc) * 360 / 8192);
    //     vector.rotate(turretOffset);
    // }

    float lxy = (WHEEL_DISTANCE_X + WHEEL_DISTANCE_Y) / 2.0f;

    // x and y are flipped so that y is forward/back and x is left/right
    float w1 = (v.y - v.x - lxy * wZ) / WHEEL_RADIUS;  // rad/s
    float w2 = (v.y + v.x + lxy * wZ) / WHEEL_RADIUS;  // rad/s
    float w3 = (v.y + v.x - lxy * wZ) / WHEEL_RADIUS;  // rad/s
    float w4 = (v.y - v.x + lxy * wZ) / WHEEL_RADIUS;  // rad/s

    static constexpr float RPS_TO_RPM = 30.0f / M_PI;
    targetWheelVels[0] = w1 * RPS_TO_RPM;
    targetWheelVels[1] = w2 * RPS_TO_RPM;
    targetWheelVels[2] = w3 * RPS_TO_RPM;
    targetWheelVels[3] = w4 * RPS_TO_RPM;
}

void ChassisSubsystem::refresh()
{
    for (int8_t i = 0; i < WHEELS; i++)
    {
        updateMotor(&pids[i], &wheelMotors[i], targetWheelVels[i]);
    }
}

void ChassisSubsystem::updateMotor(modm::Pid<float>* pid, DjiMotor* motor, float targetVelocity)
{
    pid->update(targetVelocity - motor->getShaftRPM());

    float val = pid->getValue();

    if (abs(val) < PID_MIN_OUTPUT)
    {
        val = 0.0f;
    }

    motor->setDesiredOutput(static_cast<int32_t>(val));
}

void ChassisSubsystem::runHardwareTests()
{
    // TODO
}
}  // namespace chassis
}  // namespace subsystems
