#include "tap/communication/serial/terminal_serial.hpp"
#include "tap/control/command_scheduler.hpp"
#include "tap/control/subsystem.hpp"
#include "tap/motor/dji_motor.hpp"

#include "modm/math/filter/pid.hpp"
#include "utils/motor_controller/motor_controller.hpp"

#include "drivers.hpp"
#include "test_terminal_callback.hpp"

using namespace tap::communication::serial;
using namespace tap::motor;
using namespace motor_controller;

namespace testing
{
class TestSubsystem : public tap::control::Subsystem
{
public:
    TestSubsystem(tap::Drivers* drivers);

    void initialize() override;

    void input(float a, float b);
    void refresh() override;

    void runHardwareTests() override;

    const char* getName() override { return "Test subsystem"; }

private:
    static constexpr tap::can::CanBus CBUS = tap::can::CanBus::CAN_BUS1;
    MotorVelocityController agitator;
    TestTerminalCallback terminalCallback;

    float angle = 0.0f;
    float velocity = 0.0f;
    uint32_t lastTime = 0;
};

}  // namespace testing