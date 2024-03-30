// the low-level commands of I2c

#include "encoder.hpp"
using std::end;
using std::fill;

uint16_t calculate_Angle(uint16_t angle);
// write to the start angle (ZPOS)
// input: angle from 0 to max_angle
template <class I2cMaster>
void encoder::Encoder<I2cMaster>::set_startAngle(uint16_t angle)
{
    Encoder::Encoder_R address = Encoder::Encoder_R::ZPOS;
    std::fill(std::begin(dataBuffer), std::end(dataBuffer), 0);
    dataBuffer[0] = (uint8_t)address;

    uint16_t N = calculate_Angle(angle);
    dataBuffer[1] = N & 0x0F00;  // bits [11:8]
    dataBuffer[2] = N & 0x00FF;  // bits [7:0]
    modm::I2cDevice<I2cMaster>::startWrite(dataBuffer, sizeof(dataBuffer));
}

// write to the stop angle (MPOS)
// input: angle from 0 to max_angle, must be greater than start
template <class I2cMaster>
void encoder::Encoder<I2cMaster>::set_stopAngle(uint16_t angle)
{
    Encoder::Encoder_R address = Encoder::Encoder_R::MPOS;
    std::fill(std::begin(dataBuffer), std::end(dataBuffer), 0);

    dataBuffer[0] = (uint8_t)address;
    uint16_t N = calculate_Angle(angle);
    dataBuffer[1] = N & 0x0F00;  // bits [11:8]
    dataBuffer[2] = N & 0x00FF;  // bits [7:0]

    modm::I2cDevice<I2cMaster>::startWrite(dataBuffer, sizeof(dataBuffer));
}

// read_Angle
// input: none
// output: reads the curr_angle in degrees
template <class I2cMaster>
uint16_t encoder::Encoder<I2cMaster>::read_Angle()
{
    Encoder::Encoder_R address = Encoder::Encoder_R::ANGLE;
    std::fill(std::begin(dataBuffer), std::end(dataBuffer), 0);
    dataBuffer[0] = (uint8_t)address;

    modm::I2cDevice<I2cMaster>::startRead(dataBuffer, sizeof(dataBuffer));

    // returns two bytes from MSB [1] to LSB [2],  always return MSB first.
    curr_angle = (dataBuffer[1] << 8) | dataBuffer[2];
    return curr_angle;
}

// convert angle to 1.5 bytes
//(angle / 360) * 4096 = N
// value written = N
inline uint16_t calculate_Angle(uint16_t angle) { return (((angle * 1000) / 360) * 4096) / 1000; }