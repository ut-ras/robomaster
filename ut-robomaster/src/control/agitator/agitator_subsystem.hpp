#ifndef AGITATOR_SUBSYSTEM_HPP_
#define AGITATOR_SUBSYSTEM_HPP_

#include "tap/control/subsystem.hpp"
#include "tap/motor/dji_motor.hpp"
#include "tap/util_macros.hpp"
#include "modm/math/filter/pid.hpp"

#include "tap/communication/gpio/leds.hpp"

namespace control
{
namespace agitator
{

class AgitatorSubsystem : public tap::control::Subsystem
{
public:
    static constexpr float MAX_CURRENT_OUTPUT = 8000.0f;

    AgitatorSubsystem(tap::Drivers *drivers);

    ~AgitatorSubsystem() = default;

    void initialize() override;

    void setDesiredRPM(int output);

    void refresh() override;

    void checkRegistered();

    const tap::motor::DjiMotor &getAgitatorMotor() const {return agitatorMotor;} 

private:
    static constexpr tap::motor::MotorId AGITATOR_MOTOR_ID = tap::motor::MOTOR5;
    static constexpr tap::can::CanBus CAN_BUS_MOTORS = tap::can::CanBus::CAN_BUS1;

    uint16_t agitatorMotorOutput;
    modm::Pid<float> agitatorPidController;
    tap::motor::DjiMotor agitatorMotor;

    // Drivers 
    tap::Drivers* drivers;
};

}

}

#endif