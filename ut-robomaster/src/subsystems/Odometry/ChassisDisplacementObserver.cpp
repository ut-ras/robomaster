#include "subsystems/Odometry/ChassisDisplacementObserver.hpp"

namespace subsystems::odometry
{

ChassisDisplacementObserver::ChassisDisplacementObserver(src::Drivers* drivers)
    : ChassisDisplacementObserverInterface(), imu(&drivers->bmi088),
      lastAcc({0.0f, 0.0f, 0.0f}), lastVel({0.0f, 0.0f, 0.0f}), lastDisp({0.0f, 0.0f, 0.0f}), lastTime(0)
{}

bool ChassisDisplacementObserver::getVelocityChassisDisplacement(
    modm::Vector3f* const velocity,
    modm::Vector3f* const displacement) const
{
    if (imu->getImuState() != ImuInterface::ImuState::IMU_CALIBRATED) {
        // Not calibrated
        return false;
    }

    // Attempt integration with Velocity Verlet
    // a(t) = last acceleration, a(t + dt) = current acceleration
    // x(t + dt) = x(t) + v(t)dt + 1/2 a(t)(dt)^2
    // v(t + dt) = v(t) + (a(t) + a(t + dt))dt/2
    
    // TODO: Depending on when this subsystem gets initialized,
    //   the first time this function runs, deltaT might be large
    auto nowTime = imu->getPrevIMUDataReceivedTime(); // Units of ms
    auto deltaT = (nowTime - lastTime) / 1000.0f; // Want units of s

    modm::Vector3f nowAcc{imu->getAx(), imu->getAy(), imu->getAz()};
    modm::Vector3f nowDisp = lastDisp + lastVel * deltaT + lastAcc * deltaT * deltaT / 2.0f;
    modm::Vector3f nowVel = lastVel + (lastAcc + nowAcc) * deltaT / 2.0f;

    // Update by copy
    lastTime = nowTime;
    lastAcc = nowAcc;
    lastVel = nowVel;
    lastDisp = nowDisp;

    // Return
    *velocity = nowVel;
    *displacement = nowDisp;
}

}