#include "turret_motor.hpp"

#include "tap/motor/dji_motor.hpp"
#include "tap/algorithms/math_user_utils.hpp"

using namespace tap::motor;
using namespace tap::algorithms;

namespace subsystems::turret{

TurretMotor::TurretMotor(src::Drivers *drivers, tap::motor::MotorInterface *motor, const tap::algorithms::SmoothPidConfig &pidConfig)
    : drivers(drivers),
      motor(motor),
      pid(pidConfig),
      setpoint(0.0f, 0.0f, M_TWOPI),
      currentAngle(0.0f, 0.0f, M_TWOPI)
    {
    }

TurretMotor::TurretMotor(src::Drivers *drivers, tap::motor::MotorInterface *motor, const tap::algorithms::SmoothPidConfig &pidConfig, float startAngle)
    : drivers(drivers),
      motor(motor),
      pid(pidConfig),
      setpoint(startAngle, 0.0f, M_TWOPI),
      currentAngle(startAngle, 0.0f, M_TWOPI)
    {
    }

void TurretMotor::initialize() {
    motor->initialize();
}

void TurretMotor::updateMotorAngle() {
    uint16_t encoderValue = motor->getEncoderWrapped();
    if (lastUpdatedEncoderValue != encoderValue) {
        lastUpdatedEncoderValue = encoderValue;

        unwrappedAngle = static_cast<float>(encoderValue) * M_TWOPI / static_cast<float>(DjiMotor::ENC_RESOLUTION);
        currentAngle.setValue(unwrappedAngle);
    }
}

void TurretMotor::setAngle(float desiredAngle, uint32_t dt) {
    setpoint.setValue(desiredAngle);

    float positionControllerError = ContiguousFloat(currentAngle.getValue(), 0, M_TWOPI).difference(setpoint.getValue());
    // drivers->terminal << currentAngle.getValue() << "\n";
    // drivers->terminal << desiredAngle << "\n";

    // if (compareFloatClose(0.0f, positionControllerError, 1E-3)) {
    //     drivers->leds.set(tap::gpio::Leds::Red, true);
    //     drivers->leds.set(tap::gpio::Leds::Green, false);
    //     drivers->leds.set(tap::gpio::Leds::Blue, false);
    // }

    // else if (positionControllerError > 0.0f) {
    //     drivers->leds.set(tap::gpio::Leds::Red, false);
    //     drivers->leds.set(tap::gpio::Leds::Green, true);
    //     drivers->leds.set(tap::gpio::Leds::Blue, false);
    // }

    // else {
    //     drivers->leds.set(tap::gpio::Leds::Red, false);
    //     drivers->leds.set(tap::gpio::Leds::Green, false);
    //     drivers->leds.set(tap::gpio::Leds::Blue, true);
    // }

    float output = pid.runController(positionControllerError, (M_TWOPI / 60.0f) * motor->getShaftRPM(), dt);
    
    output = limitVal(output, -GM6020_MAX_OUTPUT, GM6020_MAX_OUTPUT);
    motor->setDesiredOutput(output);
}

float TurretMotor::getAngle() {
    return currentAngle.getValue();
}

float TurretMotor::getSetpoint() {
    return setpoint.getValue();
}
}