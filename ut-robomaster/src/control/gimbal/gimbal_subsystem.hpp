/*
 * Copyright (c) 2021-2022 UT Robomaster
 *
 * This file is part of ut-robomaster.
 *
 * ut-robomaster is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ut-robomaster is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ut-robomaster.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef GIMBAL_SUBSYSTEM_HPP_
#define GIMBAL_SUBSYSTEM_HPP_

#include "tap/control/subsystem.hpp"
#include "tap/motor/dji_motor.hpp"
#include "tap/util_macros.hpp"
#include "modm/math/filter/pid.hpp"
#include "modm/math/geometry/vector.hpp"

// #include "control/control_operator_interface_edu.hpp"

namespace control
{
namespace gimbal
{
class GimbalSubsystem : public tap::control::Subsystem
{
public:
    GimbalSubsystem(tap::Drivers *drivers)
        : tap::control::Subsystem(drivers),
          drivers(drivers),
          yawMotor(
              drivers,
              YAW_MOTOR_ID,
              CAN_BUS_MOTORS,
              false,
              "yaw drive motor"),
          pitchMotor(
              drivers,
              PITCH_MOTOR_ID,
              CAN_BUS_MOTORS,
              false,
              "pitch drive motor"),
          pid(10.0f, 0.2f, 0.0f, 5000.0f, 16000.0f) // from aruw solider_chassis_constants.hpp
    {
        for (uint16_t i = 0; i < MODM_ARRAY_SIZE(desiredRPM); i++)
        {
            desiredRPM[i] = 0.0f;
        }

    }

    GimbalSubsystem(const GimbalSubsystem &other) = delete;

    GimbalSubsystem &operator=(const GimbalSubsystem &other) = delete;

    ~GimbalSubsystem() = default;

    void initialize() override;

    mockable void setDesiredOutput(float x, float y);

    void refresh() override;

    void updateMotorRpmPID(modm::Pid<float>* pid, tap::motor::DjiMotor* const motor, float desiredRpm);

    const tap::motor::DjiMotor &getYawMotor() const { return yawMotor; }
    const tap::motor::DjiMotor &getPitchMotor() const { return pitchMotor; }

private:
    tap::Drivers *drivers;
    
    ///< Hardware constants, not specific to any particular chassis.
    static constexpr tap::motor::MotorId YAW_MOTOR_ID = tap::motor::MOTOR6;
    static constexpr tap::motor::MotorId PITCH_MOTOR_ID = tap::motor::MOTOR7;
    static constexpr tap::can::CanBus CAN_BUS_MOTORS = tap::can::CanBus::CAN_BUS1;

    ///< Motors.  Use these to interact with any dji style motors.
    tap::motor::DjiMotor yawMotor;
    tap::motor::DjiMotor pitchMotor;

    float desiredRPM[2];
    float maxRPM = 500.0f;    // assume 8000 to be max rpm
    modm::Pid<float> pid;
    float startYaw;
    float startPitch;

};  // class GimbalSubsystem

}  // namespace gimbal

}  // namespace control

#endif  // GIMBAL_SUBSYSTEM_HPP_
