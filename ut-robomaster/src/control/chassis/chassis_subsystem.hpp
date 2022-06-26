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
#ifndef CHASSIS_SUBSYSTEM_HPP_
#define CHASSIS_SUBSYSTEM_HPP_

#include "tap/control/subsystem.hpp"
#include "tap/motor/dji_motor.hpp"
#include "tap/util_macros.hpp"
#include "modm/math/filter/pid.hpp"
#include "modm/math/geometry/vector.hpp"

// #include "control/control_operator_interface_edu.hpp"

namespace control
{
namespace chassis
{
/**
 * A bare bones Subsystem for interacting with a 4 wheeled chassis.
 */
class ChassisSubsystem : public tap::control::Subsystem
{
public:
    /**
     * This max output is measured in the c620 robomaster translated current.
     * Per the datasheet, the controllable current range is -16384 ~ 0 ~ 16384.
     * The corresponding speed controller output torque current range is
     * -20 ~ 0 ~ 20 A.
     *
     * For this demo, we have capped the output at 8000. This should be more
     * than enough for what you are doing.
     */
    static constexpr float MAX_CURRENT_OUTPUT = 8000.0f;

    /**
     * Constructs a new ChassisSubsystem with default parameters specified in
     * the private section of this class.
     */
    ChassisSubsystem(tap::Drivers *drivers, const tap::motor::DjiMotor *yawMotor)
        : tap::control::Subsystem(drivers),
          drivers(drivers),
          yawMotor(yawMotor),
          leftFrontMotor(
              drivers,
              LEFT_FRONT_MOTOR_ID,
              CAN_BUS_MOTORS,
              true,
              "left front drive motor"),
          leftBackMotor(
              drivers,
              LEFT_BACK_MOTOR_ID,
              CAN_BUS_MOTORS,
              true,
              "left back drive motor"),
          rightFrontMotor(
              drivers,
              RIGHT_FRONT_MOTOR_ID,
              CAN_BUS_MOTORS,
              false,
              "right front drive motor"),
          rightBackMotor(
              drivers,
              RIGHT_BACK_MOTOR_ID,
              CAN_BUS_MOTORS,
              false,
              "right back drive motor"),
          pid(22.0f, 0.2f, 0.0f, 5000.0f, 16000.0f) // from aruw solider_chassis_constants.hpp
    {
        for (uint16_t i = 0; i < MODM_ARRAY_SIZE(desiredWheelRPM); i++)
        {
            desiredWheelRPM[i] = 0.0f;
        }

    }

    ChassisSubsystem(const ChassisSubsystem &other) = delete;

    ChassisSubsystem &operator=(const ChassisSubsystem &other) = delete;

    ~ChassisSubsystem() = default;

    void initialize() override;

    mockable void setDesiredOutput(float x, float y, float r);

    void refresh() override;

    void updateMotorRpmPID(modm::Pid<float>* pid, tap::motor::DjiMotor* const motor, float desiredRpm);

    const tap::motor::DjiMotor &getLeftFrontMotor() const { return leftFrontMotor; }
    const tap::motor::DjiMotor &getLeftBackMotor() const { return leftBackMotor; }
    const tap::motor::DjiMotor &getRightFrontMotor() const { return rightFrontMotor; }
    const tap::motor::DjiMotor &getRightBackMotor() const { return rightBackMotor; }

private:
    tap::Drivers *drivers;
    const tap::motor::DjiMotor *yawMotor;
    
    ///< Hardware constants, not specific to any particular chassis.
    static constexpr tap::motor::MotorId RIGHT_FRONT_MOTOR_ID = tap::motor::MOTOR1;
    static constexpr tap::motor::MotorId LEFT_FRONT_MOTOR_ID = tap::motor::MOTOR2;
    static constexpr tap::motor::MotorId LEFT_BACK_MOTOR_ID = tap::motor::MOTOR3;
    static constexpr tap::motor::MotorId RIGHT_BACK_MOTOR_ID = tap::motor::MOTOR4;
    static constexpr tap::can::CanBus CAN_BUS_MOTORS = tap::can::CanBus::CAN_BUS1;

    ///< Motors.  Use these to interact with any dji style motors.
    tap::motor::DjiMotor leftFrontMotor;
    tap::motor::DjiMotor leftBackMotor;
    tap::motor::DjiMotor rightFrontMotor;
    tap::motor::DjiMotor rightBackMotor;

    float desiredWheelRPM[4];
    float maxRPM = 8000;    // assume 8000 to be max rpm
    modm::Vector<float, 2> vector;
    modm::Pid<float> pid;
    float startYaw;
    bool imuDrive;

    bool setStartTurret;
    float startTurretLoc;

};  // class ChassisSubsystem

}  // namespace chassis

}  // namespace control

#endif  // CHASSIS_SUBSYSTEM_HPP_
