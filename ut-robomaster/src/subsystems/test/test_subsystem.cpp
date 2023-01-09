#include "test_subsystem.hpp"

namespace subsystems
{
namespace test
{
TestSubsystem::TestSubsystem(tap::Drivers* drivers)
    : tap::control::Subsystem(drivers),
      // Ku = 0.45
      // Tu = 0.30
      agitator(
          drivers,
          M3508,
          MOTOR5,
          CBUS,
          false,
          "agitator",
          0.27f * M_TWOPI,
          1.8f * M_TWOPI,
          0.010125f * M_TWOPI),
      terminalCallback()
{
}

void TestSubsystem::initialize()
{
    agitator.initialize();
    drivers->terminalSerial.addHeader("abc", &terminalCallback);
}

void TestSubsystem::input(float a, float b) { angle = a * 0.5f; }

void TestSubsystem::refresh()
{
    uint32_t time = tap::arch::clock::getTimeMilliseconds();
    float dt = (time - lastTime) / 1000.0f;
    lastTime = time;

    agitator.update(angle, dt);
    terminalCallback.targetAngle = angle;
    terminalCallback.currentAngle = agitator.getAngle();
    terminalCallback.pidOutput = agitator.pid.output;
    terminalCallback.deltaTime = dt;
}

void TestSubsystem::runHardwareTests()
{
    // TODO
}

}  // namespace test
}  // namespace subsystems