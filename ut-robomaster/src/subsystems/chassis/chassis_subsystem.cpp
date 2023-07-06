#include "chassis_subsystem.hpp"

#include "tap/algorithms/math_user_utils.hpp"

#include "robots/robot_constants.hpp"

using namespace tap::algorithms;

namespace subsystems
{
namespace chassis
{
ChassisSubsystem::ChassisSubsystem(src::Drivers* drivers)
    : tap::control::Subsystem(drivers),
      drivers(drivers),
      powerLimiter(drivers, ENERGY_BUFFER_LIMIT_THRESHOLD, ENERGY_BUFFER_CRIT_THRESHOLD),
      wheels{
          {drivers, M3508, ID_WHEEL_LF, CAN_WHEELS, true, "left front", PID_WHEELS},
          {drivers, M3508, ID_WHEEL_RF, CAN_WHEELS, false, "right front", PID_WHEELS},
          {drivers, M3508, ID_WHEEL_LB, CAN_WHEELS, true, "left back", PID_WHEELS},
          {drivers, M3508, ID_WHEEL_RB, CAN_WHEELS, false, "right back", PID_WHEELS},
      } {};

void ChassisSubsystem::initialize()
{
    for (int8_t i = 0; i < WHEELS; i++)
    {
        wheels[i].initialize();
    }
}

void ChassisSubsystem::refresh()
{
    for (int8_t i = 0; i < WHEELS; i++)
    {
        wheels[i].setActive(!drivers->isKillSwitched());
        wheels[i].update(targetWheelVels[i] / M_TWOPI);  // rad/s to rev/s
    }

    limitChassisPower();
}

void ChassisSubsystem::limitChassisPower()
{
    float powerScalar = powerLimiter.getPowerLimitRatio();
    if (compareFloatClose(1.0f, powerScalar, 1E-3))
    {
        powerScalar = 1.0f;
    }

    float totalError = 0.0f;
    for (size_t i = 0; i < WHEELS; i++)
    {
        totalError += abs(wheels[i].getPid().getLastError());
    }

    for (int8_t i = 0; i < WHEELS; i++)
    {
        bool totalErrorZero = compareFloatClose(0.0f, totalError, 1E-3);
        float velocityErrorScalar = totalErrorZero
                                        ? (1.0f / WHEELS)
                                        : (abs(wheels[i].getPid().getLastError()) / totalError);
        float modifiedPowerScalar =
            limitVal(WHEELS * powerScalar * velocityErrorScalar, 0.0f, 1.0f);

        wheels[i].setActive(!drivers->isKillSwitched());
        wheels[i].applyPowerScalar(modifiedPowerScalar);
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

    // angular velocity correction
    if (angularTerm > 0.0f)
    {
        float correction = min(angularTerm, overdrive);
        wZ *= 1.0f - correction / angularTerm;
        overdrive -= correction;
    }

    // linear velocity correction
    if (linearTerm > 0.0f)
    {
        float correction = min(linearTerm, overdrive);
        v *= 1.0f - correction / linearTerm;
        overdrive -= correction;
    }

    setMecanumWheelVelocities(v, wZ);
}

void ChassisSubsystem::setMecanumWheelVelocities(Vector2f v, float wZ)
{
    // our velocity is rotated 90 deg, so y is forward/back and x is left/right
    targetWheelVels[0] = (-v.y - v.x - wZ * WHEEL_LXY) / WHEEL_RADIUS;  // rad/s
    targetWheelVels[1] = (-v.y + v.x + wZ * WHEEL_LXY) / WHEEL_RADIUS;  // rad/s
    targetWheelVels[2] = (-v.y + v.x - wZ * WHEEL_LXY) / WHEEL_RADIUS;  // rad/s
    targetWheelVels[3] = (-v.y - v.x + wZ * WHEEL_LXY) / WHEEL_RADIUS;  // rad/s
}

Vector3f ChassisSubsystem::measureVelocity()
{
    float w1 = wheels[0].measure();  // rev/s
    float w2 = wheels[1].measure();  // rev/s
    float w3 = wheels[2].measure();  // rev/s
    float w4 = wheels[3].measure();  // rev/s

    float xa = (w1 + w2 + w3 + w4);
    float ya = (-w1 + w2 + w3 - w4);
    float wa = (-w1 + w2 - w3 + w4) / WHEEL_LXY;

    // Rotated -90 deg to match our reference frame
    return Vector3f(ya, -xa, wa) * WHEEL_RADIUS / 4.0f * M_TWOPI;
}
}  // namespace chassis
}  // namespace subsystems
