#include "tap/algorithms/odometry/chassis_displacement_observer_interface.hpp"
#include "tap/algorithms/odometry/odometry_2d_interface.hpp"
#include "tap/algorithms/odometry/odometry_2d_tracker.hpp"
#include "tap/control/subsystem.hpp"

#include "modm/math/geometry.hpp"
#include "modm/math/geometry/location_2d.hpp"
#include "robots/standard/robot_comms.hpp"
#include "subsystems/odometry/observer_displacement.hpp"
#include "subsystems/odometry/observer_yaw_world.hpp"

#include "drivers.hpp"
namespace subsystems
{
namespace odometry
{

using tap::algorithms::odometry::Odometry2DTracker;

class OdometrySubsystem : public tap::control::Subsystem
{
public:
    OdometrySubsystem(src::Drivers* drivers);
    void initialize() override;
    void refresh() override;
    const char* getName() override { return "Odometry subsystem"; }
    void setIMULoc(modm::Location2D<> newLoc){};
    void setIMUOrientation(float& newOrienation) {}

private:
    src::Drivers* drivers;
    const int IMU_DESIRED_TEMPERATURE =
        tap::communication::sensors::imu_heater::ImuHeater::IMU_DESIRED_TEMPERATURE;
    modm::Location2D<float> location;
    float xPosOffset;
    float yPosOffset;
    ChassisDisplacementObserver chassisDisplacement;
    ChassisWorldYawObserver chassisYaw;
    Odometry2DTracker tracker;
    /*
ChassisWorldYawObserverInterface* chassisYawObserver,
ChassisDisplacementObserverInterface* chassisDisplacementObserver)*/
};
}  // namespace odometry
}  // namespace subsystems