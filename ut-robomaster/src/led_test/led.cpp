#include "tap/communication/gpio/leds.hpp"
#include "led.hpp"

using namespace tap;
using namespace tap::gpio;

using namespace led_test;

tap::gpio::Leds test;

void led_test::init()
{
    test.init();
}

void led_test::ledOn(){
    test.set(Leds::LedPin::Green, true);
}

//testing pull requests
