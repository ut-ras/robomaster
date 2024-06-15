#include "modm/io.hpp"
#include "modm/platform/rtt/rtt.hpp"

namespace communication
{
using modm::platform::Rtt;

class RttStream : public modm::IOStream
{
public:
    RttStream() : IOStream(text_device) {}

    /// @brief Send raw binary data to RTT channel for plotting and analysis
    /// @param channel Binary RTT channel to send to (1, 2, or 3)
    void plot(uint8_t channel, float32_t val)
    {
        Rtt* rtt = nullptr;

        switch (channel)
        {
            case 1:
                rtt = &rtt_plot1;
                break;
            case 2:
                rtt = &rtt_plot2;
                break;
            case 3:
                rtt = &rtt_plot3;
                break;
            default:
                return;
        }

        rtt->write(reinterpret_cast<uint8_t const*>(&val), 4);
    }

private:
    Rtt rtt_text{0};
    modm::IODeviceObjectWrapper<Rtt, modm::IOBuffer::DiscardIfFull> text_device{rtt_text};

    Rtt rtt_plot1{1};
    Rtt rtt_plot2{2};
    Rtt rtt_plot3{3};
};
}  // namespace communication