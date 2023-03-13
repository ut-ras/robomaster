#ifndef SUBSYSTEMS_ODOMETRY_CHASSIS_DISPLACEMENT_OBSERVER_HPP_
#define SUBSYSTEMS_ODOMETRY_CHASSIS_DISPLACEMENT_OBSERVER_HPP_

#include "taproot/src/tap/algorithms/odometry/chassis_world_yaw_observer_interface.hpp"

#include "modm/math/geometry.hpp"
#include "robots/standard/robot_comms.hpp"
#include "drivers.hpp"
#include "modm/math/geometry/location_2d.hpp"
#include "modm/math/geometry/angle.hpp"

namespace subsystems::odometry{
        using tap::algorithms::odometry::ChassisWorldYawObserverInterface;
        using namespace tap::communication::sensors::imu;
        class ChassisWorldYawOberver{
            public:
                ChassisWorldYawOberver(src::Drivers* drivers);
                const char* getName() { return "Chassis World Yaw Observer"; }
                virtual bool getChassisWorldYaw(float* yaw) {};
            private:
                src::Drivers* drivers;
                bmi088::Bmi088 *const imu;
        };

    };

#endif