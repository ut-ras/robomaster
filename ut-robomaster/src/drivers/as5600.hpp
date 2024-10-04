#pragma once

#include "modm/architecture/interface/i2c_device.hpp"
#include "modm/processing/protothread/protothread.hpp"

#include "board.hpp"
#include "encoder.hpp"

namespace driver
{
using Board::I2cMaster;
class As5600 : public Encoder, public modm::I2cDevice<I2cMaster, 1>, public modm::pt::Protothread
{
public:
    As5600() : modm::I2cDevice<I2cMaster, 1>(ADDRESS) {};

    void update() { run(); }

    bool run()
    {
        PT_BEGIN();

        while (true)
        {
            buffer[0] = uint8_t(Register::RAWANGLE);
            PT_WAIT_UNTIL(this->startWriteRead(buffer, 1, buffer, 2));
            PT_WAIT_WHILE(this->isTransactionRunning());

            if (this->wasTransactionSuccessful())
            {
                angle = (buffer[0] << 8) | buffer[1];
                online = true;
            }
        }

        PT_END();
    }

    float getAngle() override { return angle / 4096.0f; }

    bool isOnline() override { return online; }

protected:
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
    static const uint8_t ADDRESS = 0x36;
    uint16_t angle = 0;
    uint8_t buffer[2];
    bool online = false;
};

};  // namespace driver