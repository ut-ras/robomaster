#pragma once

#include "modm/architecture/interface/i2c_device.hpp"
#include "modm/processing/protothread/protothread.hpp"

namespace encoder
{
template <class I2cMaster>
class Encoder : public modm::I2cDevice<I2cMaster, 10, modm::I2cWriteReadTransaction>,
                public modm::pt::Protothread
{
public:
    Encoder(uint8_t address = slave_address)
        : modm::I2cDevice<I2cMaster, 10, modm::I2cWriteReadTransaction>(address){};

    void update() { run(); }

    bool run()
    {
        PT_BEGIN();

        while (true)
        {
            PT_WAIT_UNTIL(this->startWriteRead(writeBuf, 1, buffer, 2));
            PT_WAIT_WHILE(this->isTransactionRunning());

            if (this->wasTransactionSuccessful())
            {
                angle = (buffer[0] << 8) | buffer[1];
            }

            PT_YIELD();
        }

        PT_END();
    }

    float getAngle() { return angle / 4096.0f; }

protected:
    static const uint8_t slave_address = 0x36;  // address of the AS5600

    enum class Register : uint8_t
    {
        ZPOS = 0x01,
        MPOS = 0x03,
        CONF = 0x07,

        RAWANGLE = 0x0C,
        ANGLE = 0x0E,

        STATUS = 0x0B,
        AGC = 0x1A,
        MAG = 0x1B,
    };

private:
    uint16_t angle;
    uint8_t buffer[2];
    const uint8_t writeBuf[1] = {0x0E};
};

};  // namespace encoder