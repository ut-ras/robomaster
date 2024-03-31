#pragma once

#include "tap/board/board.hpp"  //clock_src

#include "modm/architecture/interface/i2c_device.hpp"  //i2c implementation
#include "modm/platform/i2c/i2c_master_2.hpp"          //i2c master

// convert angle to 1.5 bytes
//(angle / 360) * 4096 = N
// value written = N
inline uint16_t calculate_Angle(uint16_t angle) { return (((angle * 1000) / 360) * 4096) / 1000; }

namespace encoder
{

// make own read/write transaction
struct Encoder_I2cWriteReadTransaction : public modm::I2cWriteReadTransaction
{
public:
    Encoder_I2cWriteReadTransaction(uint8_t address) : modm::I2cWriteReadTransaction(address){};
};

template <class I2cMaster = modm::platform::I2cMaster2>
class Encoder : public modm::I2cDevice<I2cMaster, 10, Encoder_I2cWriteReadTransaction>
{
public:
    // init a i2c device from modm
    Encoder(uint8_t address = slave_address)
        : modm::I2cDevice<I2cMaster, 10, Encoder_I2cWriteReadTransaction>(address){};

    void set_startAngle(uint16_t angle)
    {
        Encoder::Encoder_R address = Encoder::Encoder_R::ZPOS;
        std::fill(std::begin(dataBuffer), std::end(dataBuffer), 0);
        dataBuffer[0] = (uint8_t)address;

        uint16_t N = calculate_Angle(angle);
        dataBuffer[1] = N & 0x0F00;  // bits [11:8]
        dataBuffer[2] = N & 0x00FF;  // bits [7:0]

        this->startWrite(dataBuffer, sizeof(dataBuffer));
    }

    void set_stopAngle(uint16_t angle)
    {
        Encoder::Encoder_R address = Encoder::Encoder_R::MPOS;
        std::fill(std::begin(dataBuffer), std::end(dataBuffer), 0);

        dataBuffer[0] = (uint8_t)address;
        uint16_t N = calculate_Angle(angle);
        dataBuffer[1] = N & 0x0F00;  // bits [11:8]
        dataBuffer[2] = N & 0x00FF;  // bits [7:0]

        this->startWrite(dataBuffer, sizeof(dataBuffer));
    }

    uint16_t read_Angle()
    {
        Encoder::Encoder_R address = Encoder::Encoder_R::ANGLE;
        std::fill(std::begin(dataBuffer), std::end(dataBuffer), 0);
        dataBuffer[0] = (uint8_t)address;

        this->startRead(dataBuffer, sizeof(dataBuffer));

        // returns two bytes from MSB [1] to LSB [2],  always return MSB first.
        curr_angle = (dataBuffer[1] << 8) | dataBuffer[2];
        return curr_angle;
    }

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

private:
    uint16_t curr_angle;
    const uint16_t max_angle = 360;  // specific to our application
    uint8_t dataBuffer[4];
};

};  // namespace encoder