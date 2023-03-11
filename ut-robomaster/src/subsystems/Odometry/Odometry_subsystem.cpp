#include "Odometry_subsystem.hpp"
#include "robots/robot_constants.hpp"
namespace subsystems
{
    namespace Odometry
    {
        OdometrySubsystem::OdometrySubsystem(src::Drivers* drivers) : tap::control::Subsystem(drivers){};
        void OdometrySubsystem::initialize()
        {
            xPosOffset = 0;
            yPosOffset = 0;
            location = modm::Location2D(0.0f, 0.0f, 0.0f);
        };
        void OdometrySubsystem::refresh()
        {
            modm::Location2D<float> newLoc = odom2DTracker.getCurrentLocation2D(); //location from the IMU


            //Get chassis displacement
            //get location
            //update global locaiton variable based on offset and chassis displacement
            //update();
            
        };
    };
}