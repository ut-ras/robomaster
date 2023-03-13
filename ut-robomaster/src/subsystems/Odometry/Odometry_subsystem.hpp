#include "tap/control/subsystem.hpp"

#include "modm/math/geometry.hpp"
#include "robots/standard/robot_comms.hpp"
#include "drivers.hpp"
#include "modm/math/geometry/location_2d.hpp"
#include "taproot/src/tap/algorithms/odometry/odometry_2d_tracker.hpp"
#include "taproot/src/tap/algorithms/odometry/odometry_2d_interface.hpp"
#include "taproot/src/tap/algorithms/odometry/chassis_displacement_observer_interface.hpp"
#include "subsystems/Odometry/ChassisDisplacementObserver.hpp"
namespace subsystems{
    namespace Odometry 
    {
        class odometrySubsystem : public tap::control::Subsystem
        {
            public:
                odometrySubsystem(src::Drivers* drivers);
                void initialize() override;
                void refresh() override;
                const char* getName() override { return "Odometry"; }
                void setIMULoc(modm::Location2D<> newLoc){};
                void setIMUOrientation(float& newOrienation){}
            private:
                src::Drivers* drivers;
                const int IMU_DESIRED_TEMPERATURE = tap::communication::sensors::imu_heater::ImuHeater::IMU_DESIRED_TEMPERATURE;
                modm::Location2D<float> location;
                float xPosOffset;
                float yPosOffset;
                subsystems::odometry::ChassisDisplacementObserver* chasssisDisplacement = &(subsystems::odometry::ChassisDisplacementObserver());
                subsystems::odometry::ChassisWorldYawObserver* chasssisYaw = &(subsystems::odometry::ChassisWorldYawObserver());
                tap::algorithms::odometry::odometry2DTracker odom2DTracker = (subsystems::odometry::Odometry2DTracker(chasssisYaw, chasssisDisplacement);
                /*
        ChassisWorldYawObserverInterface* chassisYawObserver,
        ChassisDisplacementObserverInterface* chassisDisplacementObserver)*/
        };
    }
}