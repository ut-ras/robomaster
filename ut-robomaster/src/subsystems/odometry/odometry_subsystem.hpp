#include "tap/algorithms/odometry/chassis_displacement_observer_interface.hpp"
#include "tap/algorithms/odometry/odometry_2d_interface.hpp"
#include "tap/algorithms/odometry/odometry_2d_tracker.hpp"
#include "tap/control/subsystem.hpp"

#include "modm/math/geometry.hpp"
#include "modm/math/geometry/location_2d.hpp"
#include "robots/standard/robot_comms.hpp"
#include "subsystems/chassis/chassis_subsystem.hpp"
#include "subsystems/odometry/observer_displacement.hpp"
#include "subsystems/odometry/observer_yaw_world.hpp"

#include "drivers.hpp"
namespace subsystems
{
namespace odometry
{

using chassis::ChassisSubsystem;
using tap::algorithms::odometry::Odometry2DTracker;

class OdometrySubsystem : public tap::control::Subsystem
{
public:
    OdometrySubsystem(src::Drivers* drivers, ChassisSubsystem* chassis);
    void initialize() override;
    void refresh() override;
    const char* getName() override { return "Odometry subsystem"; }
    void setIMULoc(modm::Location2D<> newLoc){};
    void setIMUOrientation(float& newOrienation) {}

private:
    src::Drivers* drivers;
    ChassisSubsystem* chassis;
    const int IMU_DESIRED_TEMPERATURE =
    tap::communication::sensors::imu_heater::ImuHeater::IMU_DESIRED_TEMPERATURE;
    modm::Location2D<float> location = modm::Location2D(0.0f, 0.0f, 0.0f);
    modm::Vector2f posOffset = modm::Vector2f(0.0f);
    ChassisDisplacementObserver chassisDisplacement;
    ChassisWorldYawObserver chassisYaw;
    Odometry2DTracker tracker;
    /*
ChassisWorldYawObserverInterface* chassisYawObserver,
ChassisDisplacementObserverInterface* chassisDisplacementObserver)*/
};
}  // namespace odometry
}  // namespace subsystems