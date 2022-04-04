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
    ChassisSubsystem(tap::Drivers *drivers)
        : tap::control::Subsystem(drivers),
          leftFrontMotor(
              drivers,
              LEFT_FRONT_MOTOR_ID,
              CAN_BUS_MOTORS,
              false,
              "left front drive motor"),
          leftBackMotor(
              drivers,
              LEFT_BACK_MOTOR_ID,
              CAN_BUS_MOTORS,
              false,
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
          leftFrontOutput(0),
          leftBackOutput(0),
          rightFrontOutput(0),
          rightBackOutput(0)
    {
    }

    ChassisSubsystem(const ChassisSubsystem &other) = delete;

    ChassisSubsystem &operator=(const ChassisSubsystem &other) = delete;

    ~ChassisSubsystem() = default;

    void initialize() override;

    /**
     * \todo implement this function
     *
     * Handles input for controlling the chassis.  Any input specified here will be
     * relayed directly to the motor.
     *
     * @param[in] leftFrontOutput the current output for the left front motor.  The
     *      current's sign is relative to the direction of movement of the chassis.  So,
     *      positive current to all motors should move the chassis forward, and negative
     *      to all motors should move the chassis backward.
     * @param[in] leftBackOutput the current output for the  left back motor.  See
     *      leftFrontOutput for more information.
     * @param[in] rightFrontOutput current output for the right front motor.  See
     *      leftFrontOutput for more information.
     * @param[in] rightBackOutput current output for the right back motor.  See
     *      leftFrontOutput for more information.
     */
    mockable void setDesiredOutput(int16_t leftSideOutput, int16_t rightSideOutput);

    /**
     * No-op function that is a placeholder because all interactions with motors are done
     * in setDesiredOutput.
     */
    void refresh() override;

    const tap::motor::DjiMotor &getLeftFrontMotor() const { return leftFrontMotor; }
    const tap::motor::DjiMotor &getLeftBackMotor() const { return leftBackMotor; }
    const tap::motor::DjiMotor &getRightFrontMotor() const { return rightFrontMotor; }
    const tap::motor::DjiMotor &getRightBackMotor() const { return rightBackMotor; }

private:
    ///< Hardware constants, not specific to any particular chassis.
    static constexpr tap::motor::MotorId RIGHT_FRONT_MOTOR_ID = tap::motor::MOTOR1;
    static constexpr tap::motor::MotorId LEFT_FRONT_MOTOR_ID = tap::motor::MOTOR2;
    static constexpr tap::motor::MotorId LEFT_BACK_MOTOR_ID = tap::motor::MOTOR3;
    static constexpr tap::motor::MotorId RIGHT_BACK_MOTOR_ID = tap::motor::MOTOR4;
    static constexpr tap::can::CanBus CAN_BUS_MOTORS = tap::can::CanBus::CAN_BUS2;

    ///< Motors.  Use these to interact with any dji style motors.
    tap::motor::DjiMotor leftFrontMotor;
    tap::motor::DjiMotor leftBackMotor;
    tap::motor::DjiMotor rightFrontMotor;
    tap::motor::DjiMotor rightBackMotor;

    ///< Any user input is translated into desired current for each motor.
    uint16_t leftFrontOutput;
    uint16_t leftBackOutput;
    uint16_t rightFrontOutput;
    uint16_t rightBackOutput;
};  // class ChassisSubsystem

}  // namespace chassis

}  // namespace control

#endif  // CHASSIS_SUBSYSTEM_HPP_
