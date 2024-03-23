#pragma once
#include <stdint.h>

#include "tap/board/board.hpp"  //clock_src

#include "modm/architecture/interface/i2c_device.hpp"  //i2c implementation

namespace encoder
{

// make own read/write transaction
struct Encoder_I2cWriteReadTransaction : public modm::I2cWriteReadTransaction
{
public:
    Encoder_I2cWriteReadTransaction(uint8_t address);
};

template <class I2cMaster = modm::platform::I2cMaster2>
class Encoder
    : protected modm::
          I2cDevice<modm::platform::I2cMaster2, 10, encoder::Encoder_I2cWriteReadTransaction>
{
public:
    // init a i2c device from modm
    Encoder(uint8_t address = slave_address);

    void set_startAngle(uint16_t angle);

    void set_stopAngle(uint16_t angle);

    uint16_t read_Angle();

    ~Encoder(){};

protected:
    static const uint8_t slave_address = 0x36;  // address of the AS6500

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
    enum class Command_Bit : uint8_t
    {
        WRITE = 0x00,
        READ = 0x01
    };

private : 
    uint16_t curr_angle;
    const uint16_t max_angle = 360;  // specific to our application
    uint8_t dataBuffer[4];
};

// definition of the constructer to initalize I2cDevice
template <class I2cMaster>
encoder::Encoder<I2cMaster>::Encoder(uint8_t address)
    : I2cDevice<I2cMaster, 3, encoder::Encoder_I2cWriteReadTransaction>(address)
{
    // put into encoder_test (master)
    // modm::platform::I2cMaster2::initialize<Board::SystemClock>();
}

};  // namespace encoder