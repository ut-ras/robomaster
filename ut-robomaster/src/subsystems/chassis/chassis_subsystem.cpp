#include "chassis_subsystem.hpp"

namespace subsystems
{
namespace chassis
{
ChassisSubsystem::ChassisSubsystem(
    tap::Drivers* drivers,
    DjiMotor* yawMotor,
    MotorId leftFrontMotorId,
    MotorId rightBackMotorId,
    MotorId rightFrontMotorId,
    MotorId leftBackMotorId)
    : drivers(drivers),
      tap::control::Subsystem(drivers),
      targetWheelVels({0.0f, 0.0f, 0.0f, 0.0f}),
      wheelMotors{
          DjiMotor(drivers, leftFrontMotorId, CAN_BUS_MOTORS, true, "left front motor"),
          DjiMotor(drivers, leftBackMotorId, CAN_BUS_MOTORS, true, "left back motor"),
          DjiMotor(drivers, rightFrontMotorId, CAN_BUS_MOTORS, true, "right front motor"),
          DjiMotor(drivers, rightBackMotorId, CAN_BUS_MOTORS, true, "right back motor"),
      },
      yawMotor(yawMotor),
      pids{
          modm::Pid<float>(PID_KP, PID_KI, PID_KD, PID_MAX_ERROR_SUM, PID_MAX_OUTPUT),
          modm::Pid<float>(PID_KP, PID_KI, PID_KD, PID_MAX_ERROR_SUM, PID_MAX_OUTPUT),
          modm::Pid<float>(PID_KP, PID_KI, PID_KD, PID_MAX_ERROR_SUM, PID_MAX_OUTPUT),
          modm::Pid<float>(PID_KP, PID_KI, PID_KD, PID_MAX_ERROR_SUM, PID_MAX_OUTPUT)},
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

void ChassisSubsystem::setVelocities(float vX, float vY, float wZ)
{
    ImuInterface::ImuState imuState = drivers->bmi088.getImuState();

    if (imuState == ImuInterface::ImuState::IMU_CALIBRATING)  // if the 6020 is set to the
                                                              // calibrated value, set all RPMs to 0
    {
        for (uint16_t i = 0; i < WHEELS; i++)
        {
            targetWheelVels[i] = 0;
        }

        return;
    }

    modm::Vector2f vector(vX, vY);

    if (imuDrive && imuState == ImuInterface::ImuState::IMU_CALIBRATED)
    {
        if (!setStartTurret && yawMotor->isMotorOnline())
        {
            startTurretLoc = yawMotor->getEncoderUnwrapped();
            setStartTurret = true;
        }
        float turretOffset =
            modm::toRadian((yawMotor->getEncoderUnwrapped() - startTurretLoc) * 360 / 8192);
        vector.rotate(turretOffset);
    }

    float theta = vector.getAngle();
    float power = vector.getLength();

    float sin = sinf(theta - M_PI_4);
    float cos = cosf(theta - M_PI_4);
    float max = modm::max(std::abs(sin), std::abs(cos));

    targetWheelVels[0] = power * cos / max + wZ;  // right front wheel
    targetWheelVels[1] = power * sin / max - wZ;  // left front wheel
    targetWheelVels[2] = power * cos / max - wZ;  // left back wheel
    targetWheelVels[3] = power * sin / max + wZ;  // right back wheel

    if ((power + abs(wZ)) > 1)
    {
        for (int8_t i = 0; i < WHEELS; i++)
        {
            targetWheelVels[i] /= power + abs(wZ);
        }
    }

    for (uint16_t i = 0; i < WHEELS; i++)
    {
        targetWheelVels[i] *= MAX_SPEED;
    }
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
    motor->setDesiredOutput(static_cast<int32_t>(pid->getValue()));
}

void ChassisSubsystem::runHardwareTests()
{
    // TODO
}
}  // namespace chassis
}  // namespace subsystems
