#include "modm/io.hpp"
#include "modm/platform/rtt/rtt.hpp"

namespace communication
{
class RttStream : public modm::IOStream
{
public:
    RttStream() : rtt(0), device(rtt), IOStream(device) {}

private:
    modm::platform::Rtt rtt;
    modm::IODeviceObjectWrapper<modm::platform::Rtt, modm::IOBuffer::DiscardIfFull> device;
};
}  // namespace communication