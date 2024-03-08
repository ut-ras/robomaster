#pragma once
#include <stdint.h>

#include "tap/board/board.hpp"  //clock_src

#include "modm/architecture/interface/i2c_device.hpp"  //i2c implementation

using namespace modm;

namespace encoder
{
class Encoder : protected modm::I2cDevice<modm::I2cMaster>
{
public:

    // enum of the different register address
    enum class Encoder_R : uint8_t
    {

        // WRITE CONFIGURATION INPUTS i.e. bits[3:0] indicates a...

        ZPOS = 0x01,
        MPOS = 0x03,
        CONF = 0x07,

        RwANGLE = 0x0C,
        ANGLE = 0x0E,

        STATUS = 0x0B,
        AGC = 0x1A,
        MAG = 0x1B,
    };
    // init a i2c device from modm
    Encoder() : driver(I2cDevice<modm::I2cMaster>(slave_address))
    {
        // need to I2cMaster::connect (?), need to know specific pins for SCL/SDA
        
        modm::I2cMaster::initialize<Board::SystemClock>();
    };

    void set_startAngle(uint16_t angle);

    void set_stopAngle(uint16_t angle);

    // read the current angle on the encoder
    uint16_t read_Angle();

    ~Encoder(){};

protected:
    static const uint8_t slave_address = 0x36;  // address of the AS6500

private:
    uint16_t curr_angle;
    const uint16_t max_angle = 360; //specific to our application
    modm::I2cDevice<modm::I2cMaster> driver;

};

}