#include "chassis_subsystem.hpp"

#include "robots/robot_constants.hpp"
#include "robots/standard/robot_comms.hpp"

namespace subsystems
{
namespace chassis
{
ChassisSubsystem::ChassisSubsystem(src::Drivers* drivers)
    : tap::control::Subsystem(drivers),
      drivers(drivers),
      wheels{
          {drivers, M3508, ID_WHEEL_LF, CAN_WHEELS, true, "left front", PID_KP, PID_KI, PID_KD},
          {drivers, M3508, ID_WHEEL_RF, CAN_WHEELS, false, "right front", PID_KP, PID_KI, PID_KD},
          {drivers, M3508, ID_WHEEL_LB, CAN_WHEELS, true, "left back", PID_KP, PID_KI, PID_KD},
          {drivers, M3508, ID_WHEEL_RB, CAN_WHEELS, false, "right back", PID_KP, PID_KI, PID_KD},
      },
      imu(drivers)
{
}

void ChassisSubsystem::initialize()
{
    for (int8_t i = 0; i < WHEELS; i++)
    {
        wheels[i].initialize();
    }
    imu.requestRecalibration();
}

void ChassisSubsystem::recalibrateIMU() { imu.requestRecalibration(); }

void ChassisSubsystem::refresh()
{
    for (int8_t i = 0; i < WHEELS; i++)
    {
        wheels[i].update(targetWheelVels[i] / M_TWOPI);  // rad/s to rev/s
    }
    comms::RobotCommsSingleton::print(
        "Imu yaw: %f, pitch: %f, roll: %f",
        imu.getYaw(),
        imu.getPitch(),
        imu.getRoll());
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
}  // namespace chassis
}  // namespace subsystems
