#include "chassis_subsystem.hpp"

#include "tap/algorithms/math_user_utils.hpp"

#include "drivers.hpp"

using tap::algorithms::limitVal;

namespace control::chassis
{
// STEP 1 (Tank Drive): create constructor
ChassisSubsystem::ChassisSubsystem(Drivers &drivers, const ChassisConfig &config)
    : tap::control::Subsystem(&drivers),
      desiredOutput{},
      pidControllers{},
      motors{
          Motor(&drivers, config.leftFrontId, config.canBus, false, "LF"),
          Motor(&drivers, config.leftBackId, config.canBus, false, "LB"),
          Motor(&drivers, config.rightBackId, config.canBus, true, "RB"),
          Motor(&drivers, config.rightFrontId, config.canBus, true, "RF"),
      }
{
    for (auto &controller : pidControllers)
    {
        controller.setParameter(config.wheelVelocityPidConfig);
    }
}

// STEP 2 (Tank Drive): initialize function
void ChassisSubsystem::initialize()
{
    for (auto &motor : motors)
    {
        motor.initialize();
    }
}

// STEP 4 (Tank Drive): setVelocityTankDrive function
void ChassisSubsystem::setVelocityTankDrive(float left, float right)
{
    left = mpsToRpm(left);
    right = mpsToRpm(right);

    left = limitVal(left, -MAX_WHEELSPEED_RPM, MAX_WHEELSPEED_RPM);
    right = limitVal(right, -MAX_WHEELSPEED_RPM, MAX_WHEELSPEED_RPM);

    desiredOutput[static_cast<uint8_t>(MotorId::LF)] = left;
    desiredOutput[static_cast<uint8_t>(MotorId::LB)] = left;
    desiredOutput[static_cast<uint8_t>(MotorId::RB)] = right;
    desiredOutput[static_cast<uint8_t>(MotorId::RF)] = right;
}

// STEP 5 (Tank Drive): refresh function
void ChassisSubsystem::refresh()
{
    auto runPid = [](Pid &pid, Motor &motor, float desiredOutput) {
        pid.update(desiredOutput - motor.getShaftRPM());
        motor.setDesiredOutput(pid.getValue());
    };

    for (size_t ii = 0; ii < motors.size(); ii++)
    {
        runPid(pidControllers[ii], motors[ii], desiredOutput[ii]);
    }
}
}  // namespace control::chassis
