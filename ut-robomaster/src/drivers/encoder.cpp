#include "encoder.hpp"

using namespace encoder;
using namespace modm;
uint16_t calculate_Angle(uint16_t angle);
// write to the start angle (ZPOS)
// input: angle from 0 to max_angle

void Encoder::set_startAngle(uint16_t angle)
{
    Encoder::Encoder_R address = Encoder::Encoder_R::ZPOS;
    uint8_t data[3] = {0};
    data[0] = (uint8_t)address;

    uint16_t N = calculate_Angle(angle);
    data[1] = N & 0x0F00;  // bits [11:8]
    data[0] = N & 0x00FF;  // bits [7:0]

    startWrite(data, sizeof(data));
}

// write to the stop angle (MPOS)
// input: angle from 0 to max_angle, must be greater than start
void Encoder::set_stopAngle(uint16_t angle)
{
    Encoder::Encoder_R address = Encoder::Encoder_R::MPOS;
    uint8_t data[3] = {0};
    data[0] = (uint8_t)address;
    uint16_t N = calculate_Angle(angle);
    data[1] = N & 0x0F00;  // bits [11:8]
    data[2] = N & 0x00FF;  // bits [7:0]
    startWrite(data, sizeof(data));
}

// read_Angle
// input: none
// output: reads the
uint16_t Encoder::read_Angle()
{
    Encoder::Encoder_R address = Encoder::Encoder_R::ANGLE;
    uint8_t data[3] = {0};
    data[0] = (uint8_t)address;
    startRead(data, sizeof(data));

    curr_angle = (data[1] << 2) | data[2];
    return curr_angle;
}
// convert angle to 1.5 bytes
//(angle / 360) * 4096 = N
// value written = N
inline uint16_t calculate_Angle(uint16_t angle) { return (((angle * 1000) / 360) * 4096) / 1000; }