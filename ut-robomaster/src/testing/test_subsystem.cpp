#include "test_subsystem.hpp"

namespace testing
{
TestSubsystem::TestSubsystem(tap::Drivers* drivers)
    : tap::control::Subsystem(drivers),
      // Ku = 0.45
      // Tu = 0.30
      //   agitator(
      //       drivers,
      //       M3508,
      //       MOTOR5,
      //       CBUS,
      //       false,
      //       "agitator",
      //       0.27f * M_TWOPI,
      //       1.8f * M_TWOPI,
      //       0.010125f * M_TWOPI),
      controller(drivers, M3508, MOTOR2, CBUS, false, "test motor", 1.0f, 1.0f, 0.0f),
      terminalCallback()
{
}

void TestSubsystem::initialize()
{
    controller.initialize();
    drivers->terminalSerial.addHeader("test", &terminalCallback);
}

void TestSubsystem::input(float a, float b)
{
    angle = a * 0.5f;
    velocity = b * 1.0f;
}

void TestSubsystem::refresh()
{
    uint32_t time = tap::arch::clock::getTimeMilliseconds();
    float dt = (time - lastTime) / 1000.0f;
    lastTime = time;

    controller.update(terminalCallback.argument * 8.0f, dt);  // velocity
    terminalCallback.output = controller.measure();

    // float current = 0.5f;  // A
    // float torque_constant = 0.3f;               // N*m / A
    // float gear_ratio = agitator.constants.gearRatio;
    // float inertia = 1.0f;  // kg*m^2 (1e-9 * g*mm^2)
    // float acceleration = torque_constant * gear_ratio / inertia * current;
    // agitator.motor.setDesiredOutput(current * 16384.0f / 20.0f);  // accelerating!
    // terminalCallback.pidOutput = agitator.pid.output;

    // agitator.update(angle, dt);
    // terminalCallback.targetAngle = angle;
    // terminalCallback.currentAngle = agitator.getAngle();
    // terminalCallback.pidOutput = agitator.pid.output;
    // terminalCallback.deltaTime = dt;
}

void TestSubsystem::runHardwareTests()
{
    // TODO
}

}  // namespace testing