#include "agitator_motor.hpp"

namespace subsystems::shooter
{
void AgitatorMotor::update(float target)
{
    float normalizedOutput = target * constants.gearRatio / constants.maxTorque;
    float clampedOutput = modm::min(modm::max(normalizedOutput, -1.0f), 1.0f);
    motor.setDesiredOutput(clampedOutput * constants.maxOutput);
}
float AgitatorMotor::measure() { return motor.getTorque(); }

}  // namespace subsystems::shooter