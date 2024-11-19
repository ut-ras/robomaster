#pragma once

#include "tap/algorithms/odometry/chassis_displacement_observer_interface.hpp"
#include "tap/algorithms/odometry/odometry_2d_interface.hpp"
#include "tap/algorithms/odometry/odometry_2d_tracker.hpp"
#include "tap/control/subsystem.hpp"

#include "modm/math/geometry.hpp"
#include "modm/math/geometry/location_2d.hpp"
#include "subsystems/chassis/chassis_subsystem.hpp"
#include "subsystems/odometry/observer_displacement.hpp"
#include "subsystems/odometry/observer_yaw_world.hpp"
#include "subsystems/subsystem.hpp"
#include "subsystems/turret/turret_subsystem.hpp"
#include "utils/robot_comms.hpp"

#include "drivers.hpp"

namespace subsystems::odometry
{
using chassis::ChassisSubsystem;
using modm::Vector2f;
using tap::algorithms::odometry::Odometry2DTracker;
using turret::TurretSubsystem;

class OdometrySubsystem : public Subsystem
{
public:
    OdometrySubsystem(src::Drivers* drivers, ChassisSubsystem* chassis, TurretSubsystem* turret);
    void initialize() override;
    void refresh() override;
    bool hardwareOk() override;

    Vector2f getPosition();
    Vector2f getLinearVelocity();

    float getChassisYaw();
    float getChassisAngularVelocity();

    float getTurretLocalYaw();
    float getTurretLocalPitch();

private:
    src::Drivers* drivers;
    ChassisSubsystem* chassis;
    TurretSubsystem* turret;

    ChassisDisplacementObserver chassisDisplacement;
    ChassisWorldYawObserver chassisYaw;
    Odometry2DTracker chassisTracker;
};
}  // namespace subsystems::odometry