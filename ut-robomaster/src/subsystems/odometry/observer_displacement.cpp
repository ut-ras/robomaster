#include "subsystems/odometry/observer_displacement.hpp"

#include "tap/architecture/clock.hpp"

/*
 * Notes:
 * https://gitlab.com/aruw/controls/aruw-mcb/-/tree/develop/aruw-mcb-project/src/aruwsrc/algorithms/odometry?ref_type=heads
 * */

namespace subsystems::odometry
{

ChassisDisplacementObserver::ChassisDisplacementObserver(
    src::Drivers* drivers,
    chassis::ChassisSubsystem* chassis)
    : ChassisDisplacementObserverInterface(),
      drivers(drivers),
      chassis(chassis),
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
    uint32_t currTime = tap::arch::clock::getTimeMicroseconds();
    if (prevTime != 0)
    {
        modm::Vector3f myv = chassis->measureVelocity();
        velocity->set(myv.x, myv.y, myv.z);

        //  velocity.set(chassisRelVel.x, chassisRelVel.y);

        // displacement->move(tickDisp);
        float deltaT = (static_cast<float>(currTime - prevTime)) / 1'000'000.0f;
        modm::Vector3f myDisp = myv * deltaT;
        displacement->set(myDisp.x, myDisp.y, myDisp.z);
        // displacement += velocity * (static_cast<float>(currTime - prevTime) / 1'000'000.0f);
    }

    prevTime = currTime;
    // bmi088::Bmi088* imu = &drivers->bmi088;

    // Attempt integration with Velocity Verlet
    // a(t) = last acceleration, a(t + dt) = current acceleration
    // x(t + dt) = x(t) + v(t)dt + 1/2 a(t)(dt)^2
    // v(t + dt) = v(t) + (a(t) + a(t + dt))dt/2

    // TODO: Depending on when this subsystem gets initialized,
    //   the first time this function runs, deltaT might be large
    // auto nowTime = imu->getPrevIMUDataReceivedTime();  // Units of us
    // auto dt = (nowTime - lastTime) / 1e6f;             // Want units of s

    // // z is 0 since we're moving on the x-y plane and gravity affects z
    // Vector3f nowAcc{imu->getAx(), imu->getAy(), 0.0f};
    // Vector3f nowDisp = lastDisp + lastVel * dt + lastAcc * dt * dt / 2.0f;
    // Vector3f nowVel = lastVel + (lastAcc + nowAcc) * dt / 2.0f;

    // // Update by copy
    // lastTime = nowTime;
    // lastAcc = nowAcc;
    // lastVel = nowVel;
    // lastDisp = nowDisp;

    // // Return
    // *velocity = nowVel;
    // *displacement = nowDisp;

    // return imu->getImuState() == ImuInterface::ImuState::IMU_CALIBRATED;
    return true;
}

}  // namespace subsystems::odometry