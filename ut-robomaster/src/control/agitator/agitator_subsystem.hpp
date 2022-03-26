#ifndef AGITATOR_SUBSYSTEM_HPP_
#define AGITATOR_SUBSYSTEM_HPP_

#include "tap/control/subsystem.hpp"
#include "tap/motor/dji_motor.hpp"
#include "tap/util_macros.hpp"

#include "tap/communication/gpio/leds.hpp"

namespace control
{
namespace agitator
{

class AgitatorSubsystem : public tap::control::Subsystem
{
public:
    static constexpr float MAX_CURRENT_OUTPUT = 8000.0f;

    AgitatorSubsystem(tap::Drivers *drivers)
        : tap::control::Subsystem(drivers),
          agitatorMotor(
              drivers,
              AGITATOR_MOTOR_ID,
              CAN_BUS_MOTORS,
              false,
              "agitator motor"),
              drivers(drivers),
          agitatorMotorOutput(0.0f)
    {
    }    

    ~AgitatorSubsystem() = default;

    void initialize() override;

    void setDesiredOutput(float output);

    void refresh() override;

    void setLed();

    const tap::motor::DjiMotor &getAgitatorMotor() const {return agitatorMotor;} 

private:
    static constexpr tap::motor::MotorId AGITATOR_MOTOR_ID = tap::motor::MOTOR5;
    static constexpr tap::can::CanBus CAN_BUS_MOTORS = tap::can::CanBus::CAN_BUS1;

    tap::motor::DjiMotor agitatorMotor;
    // Drivers 
    tap::Drivers* drivers;

    uint16_t agitatorMotorOutput;
};

}

}

#endif