#pragma once

#include "tap/control/command.hpp"

#include "subsystems/chassis/chassis_subsystem.hpp"
#include "subsystems/odometry/odometry_subsystem.hpp"
#include "communication/cv_message.hpp"
#include "utils/chassis_auto_align.hpp"


#include "drivers.hpp"

namespace commands
{
using namespace modm;
using subsystems::odometry::OdometrySubsystem;
using subsystems::chassis::ChassisSubsystem;

using communication::OdometryData;

class CommandOdomPoint : public tap::control::Command
{
public:
    CommandOdomPoint(src::Drivers *drivers, ChassisSubsystem* chassis, OdometrySubsystem *odom)
        : drivers(drivers),
        chassis(chassis),
        odometry(odom)
    {
        addSubsystemRequirement(chassis);
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "rotate odom command"; }

private:
    src::Drivers *drivers;
    ChassisSubsystem *chassis;
    OdometrySubsystem *odometry;
    Vector2f currentPosition;

    const float p = 1.0f;
    const float i = 1.0f;
    const float d = 1.0f;
    const float dist_threshold = 0.1f;  //m
    const float angle_threshold = 0.3f; //rads
    float prevError = 0.0f;

    float currX;
    float currY;

    float desiredX;
    float desiredY;

    float currYaw;
    float desiredYaw;

    tap::arch::PeriodicMilliTimer refreshTimer{10};

};  // class CommandOdomPoint
}  // namespace commands