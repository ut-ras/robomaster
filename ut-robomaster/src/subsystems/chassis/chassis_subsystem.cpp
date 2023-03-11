#include "chassis_subsystem.hpp"

#include "robots/robot_constants.hpp"
int imuClock = 0;

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
      }, imu(drivers->bmi088){};
void ChassisSubsystem::initialize()
{
    talky.init(drivers);
    for (int8_t i = 0; i < WHEELS; i++)
    {
        wheels[i].initialize();
    }
    //imu.requestRecalibration();
    //drivers->bmi088.requestRecalibration();
}

//void ChassisSubsystem::recalibrateIMU() { imu.requestRecalibration(); }

void ChassisSubsystem::refresh()
{
    for (int8_t i = 0; i < WHEELS; i++)
    {
        wheels[i].update(targetWheelVels[i] / M_TWOPI);  // rad/s to rev/s
    }
    if(imuClock == 200)
    {
        modm::Location2D<float> lidarLocation = modm::Location2D(1.0f, 2.0f, 0);
        //lidarLocation.setOrientation(orientationRef); //set orientation based off lidar
        
        
    }
    if (imuClock == 500)
    {
        constexpr auto IMU_DESIRED_TEMPERATURE = tap::communication::sensors::imu_heater::ImuHeater::IMU_DESIRED_TEMPERATURE;
        float imuTemp = imu.getTemp();
        //talky.print("%s: %d, %f \n", "Talky Test:", 12, 12.5446);
        if(IMU_DESIRED_TEMPERATURE>=imuTemp)
        {
            talky.print("IMU is below desired temperature: Desired: %d, Current Temperate: %d", static_cast<int>(IMU_DESIRED_TEMPERATURE), static_cast<int>(imuTemp));
        }
        else
        {
            talky.print("IMU is above desired temperature: Desired: %d, Current Temperate: %d", static_cast<int>(IMU_DESIRED_TEMPERATURE), static_cast<int>(imuTemp));
        }  
            talky.print(
            "Imu state: %d, Ax: %d, Ay: %d, Az: %d\n",
            static_cast<int>(drivers->bmi088.getImuState()),
            static_cast<int>(drivers->bmi088.getAx()),
            static_cast<int>(drivers->bmi088.getAy()),
            static_cast<int>(drivers->bmi088.getAz()));
        imuClock = 0;
    }
    imuClock++;
    
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
