#pragma once
#include <stdint.h>

#include "modm/architecture/interface/i2c.hpp"
#include "modm/architecture/interface/i2c_device.hpp"
#include "modm/architecture/interface/i2c_master.hpp"
#include "modm/architecture/interface/i2c_transaction.hpp"

using namespace modm;

namespace encoder
{
class Encoder : public modm::I2cDevice<modm::I2cMaster, 10, modm::I2cWriteReadTransaction>
{
public:
    // enum of the different register address

    enum mode
    {
        WRITE,
        READ
    };
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
    Encoder();

    // various functions to write/read from encoder

    // write to the start angle (ZPOS)
    // input: angle from 0 to max_angle
    void set_startAngle(uint16_t angle);
    // write to the stop angle (MPOS)
    // input: angle from 0 to max_angle, must be greater than start
    void set_stopAngle(uint16_t angle);

    // read the current angle on the encoder
    uint16_t read_Angle();

    ~Encoder(){};

protected:
    static const uint8_t slave_address = 0x36;  // address of the AS6500

private:
    static uint16_t curr_angle;
    static const uint16_t max_angle = 360;
};

}  // namespace encoder

// 7-bit slave address followed by a 1 bit for R/W operation
// Slave ack if correct address
// Suppose it is a write bit, the master transmits a word (byte)
// address of register on slave

// Read
// could read from slave with or without a specific register address
// Needs a NACK from the master to the slave to end transmission

// Need to check valid address for both read/write