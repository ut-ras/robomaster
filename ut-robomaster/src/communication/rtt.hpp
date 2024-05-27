#include "modm/io.hpp"
#include "modm/platform/rtt/rtt.hpp"

namespace communication
{
class RttStream : public modm::IOStream
{
public:
    RttStream() : IOStream(device) {}

private:
    modm::platform::Rtt rtt{0};
    modm::IODeviceObjectWrapper<modm::platform::Rtt, modm::IOBuffer::DiscardIfFull> device{rtt};
};
}  // namespace communication