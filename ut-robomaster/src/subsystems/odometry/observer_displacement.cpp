#include "subsystems/odometry/observer_displacement.hpp"

namespace subsystems::odometry
{

ChassisDisplacementObserver::ChassisDisplacementObserver(src::Drivers* drivers)
    : ChassisDisplacementObserverInterface(),
      drivers(drivers),
      lastAcc(0.0f),
      lastVel(0.0f),
      lastDisp(0.0f),
      lastTime(0)
{
}

bool ChassisDisplacementObserver::getVelocityChassisDisplacement(
    modm::Vector3f* const velocity,
    modm::Vector3f* const displacement) const
{
    bmi088::Bmi088* imu = &drivers->bmi088;

    // Attempt integration with Velocity Verlet
    // a(t) = last acceleration, a(t + dt) = current acceleration
    // x(t + dt) = x(t) + v(t)dt + 1/2 a(t)(dt)^2
    // v(t + dt) = v(t) + (a(t) + a(t + dt))dt/2

    // TODO: Depending on when this subsystem gets initialized,
    //   the first time this function runs, deltaT might be large
    auto nowTime = imu->getPrevIMUDataReceivedTime();  // Units of us
    auto dt = (nowTime - lastTime) / 1e6f;             // Want units of s

    Vector3f nowAcc{imu->getAx(), imu->getAy(), imu->getAz()};
    Vector3f nowDisp = lastDisp + lastVel * dt + lastAcc * dt * dt / 2.0f;
    Vector3f nowVel = lastVel + (lastAcc + nowAcc) * dt / 2.0f;

    // Update by copy
    lastTime = nowTime;
    lastAcc = nowAcc;
    lastVel = nowVel;
    lastDisp = nowDisp;

    // Return
    *velocity = nowVel;
    *displacement = nowDisp;

    return imu->getImuState() == ImuInterface::ImuState::IMU_CALIBRATED;
}

}  // namespace subsystems::odometry