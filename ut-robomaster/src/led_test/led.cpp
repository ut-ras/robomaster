#include "tap/communication/gpio/leds.hpp"

using namespace tap;
using namespace tap::gpio;

namespace led_test
{

void ledOn(){
    tap::gpio::Leds::init();
    tap::gpio::Leds::set(Leds::LedPin::Green, false);
}
// void LedTest::ledOff(){

// } 
}
