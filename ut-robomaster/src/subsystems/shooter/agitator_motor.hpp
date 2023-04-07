#ifndef AGITATOR_MOTOR_HPP_
#define AGITATOR_MOTOR_HPP_

#include "tap/motor/dji_motor.hpp"

#include "utils/motor_controller/motor_constants.hpp"

#include "drivers.hpp"

using tap::can::CanBus;
using tap::motor::DjiMotor;
using tap::motor::MotorId;

namespace subsystems::shooter
{
using motor_controller::MotorConstants;

class AgitatorMotor
{
public:
    AgitatorMotor(
        src::Drivers* drivers,
        const MotorConstants& constants,
        const MotorId motorId,
        const CanBus motorCanBus,
        const bool motorInverted,
        const char* motorName)
        : constants(constants),
          motor(drivers, motorId, motorCanBus, motorInverted, motorName)
    {
    }

    void initialize() { motor.initialize(); }
    void update(float target);
    float measure();

protected:
    const MotorConstants constants;
    DjiMotor motor;
};
}  // namespace subsystems::shooter

#endif