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
#ifndef TURRET_SUBSYSTEM_HPP_
#define TURRET_SUBSYSTEM_HPP_

#include "tap/control/subsystem.hpp"
#include "tap/motor/dji_motor.hpp"
#include "tap/util_macros.hpp"
#include "modm/math/filter/pid.hpp"
#include "modm/math/geometry/vector.hpp"
#include "tap/architecture/periodic_timer.hpp"

#define MOUSE_SCALAR 1.0f
#define PITCH_RANGE 125000.0f

namespace control
{
namespace turret
{
class TurretSubsystem : public tap::control::Subsystem
{
public:
    TurretSubsystem(tap::Drivers *drivers)
        : tap::control::Subsystem(drivers),
          drivers(drivers),
          yawMotor(
              drivers,
              YAW_MOTOR_ID,
              CAN_BUS_MOTORS,
              true,
              "yaw motor"),
          pitchMotor(
              drivers,
              PITCH_MOTOR_ID,
              CAN_BUS_MOTORS,
              false,
              "pitch motor"),
          pid(10.0f, 0.2f, 0.0f, 5000.0f, 16000.0f), // from aruw solider_chassis_constants.hpp
          pitchEncoderToRPM(1.0f, 0.0f, 0.0f, 5000.0f, 16000.0f),
          yawEncoderToRPM(5.0f, 0.0f, 50.0f, 5000.0f, 16000.0f)
    {
        for (uint16_t i = 0; i < MODM_ARRAY_SIZE(desiredRPM); i++)
        {
            desiredRPM[i] = 0.0f;
        }

    }

    TurretSubsystem(const TurretSubsystem &other) = delete;

    TurretSubsystem &operator=(const TurretSubsystem &other) = delete;

    ~TurretSubsystem() = default;

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
    float maxRPM = 500.0f;    // physical max approximately 8000.0f
    modm::Pid<float> pid;
    modm::Pid<float> pitchEncoderToRPM;
    modm::Pid<float> yawEncoderToRPM;

    float startYaw;
    float startPitch;

    bool yawIsSet;
    bool pitchIsSet;
    float yawSetValue;
    float pitchSetValue;

    float prevPosition;

    float offset;
    bool ledTest;
    tap::arch::PeriodicMilliTimer timer;
};  // class TurretSubsystem

 
}  // namespace turret

}  // namespace control

#endif  // TURRET_SUBSYSTEM_HPP_
