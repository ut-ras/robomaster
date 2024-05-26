/*
 * Copyright (c) 2021, Niklas Hauser
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#include <modm/platform/device.hpp>
#include "rtt.hpp"
#include <algorithm>

namespace modm::platform
{

struct RttBuffer
{
	const char* name;
	uint8_t* const buffer;
	const uint32_t size;
	volatile uint32_t head;
	volatile uint32_t tail;
	const uint32_t flags;

	bool write(uint8_t data)
	{
		const uint32_t rhead{head};
		const uint32_t rtail{tail};
		const uint32_t rhead_next{(rhead + 1) % size};
		if (not size or rhead_next == rtail) return false;
		buffer[rhead] = data;
		head = rhead_next;
		return true;
	}
	bool read(uint8_t &data)
	{
		const uint32_t rhead{head};
		const uint32_t rtail{tail};
		if (not size or rtail == rhead) return false;
		data = buffer[rtail];
		tail = (rtail + 1) % size;
		return true;
	}
	bool isEmpty() const { return (head == tail); }
	uint32_t getSize() const
	{
		const uint32_t rhead{head};
		const uint32_t rtail{tail};
		return ((rhead >= rtail) ? 0 : size) + rhead - rtail;
	}
} modm_packed;

static uint8_t tx_data_buffer_0[512];

struct RttControlBlock
{
	const char identifier[16];
	const int32_t tx_buffer_count;
	const int32_t rx_buffer_count;
	RttBuffer tx_buffers[1];
	RttBuffer rx_buffers[1];
} modm_packed;

// Explicitly constructed as constinit to force *only* copying via .data section.
// This prevents the "modm.rtt.modm" identifier leaking into the stack and being
// found by OpenOCD accidentally instead of the real RTT control block.
static constinit RttControlBlock rtt_control{
	"modm.rtt.modm",
	1,
	1,
	{
		{"tx0", tx_data_buffer_0, 512, 0,0,0 },
	},{
		{"rx0", nullptr, 0, 0,0,0 },
	}
};


Rtt::Rtt(uint8_t channel)
:	tx_buffer(rtt_control.tx_buffers[std::min<uint8_t>(channel, 0)]),
	rx_buffer(rtt_control.rx_buffers[std::min<uint8_t>(channel, 0)])
{
}

bool
Rtt::write(uint8_t data)
{
	return tx_buffer.write(data);
}

std::size_t
Rtt::write(const uint8_t *data, std::size_t length)
{
	std::size_t sent = 0;
	for (; sent < length; sent++)
		if (not write(*data++))
			return sent;
	return sent;
}

bool
Rtt::isWriteFinished()
{
	return tx_buffer.isEmpty();
}

std::size_t
Rtt::transmitBufferSize()
{
	return tx_buffer.getSize();
}

bool
Rtt::read(uint8_t& data)
{
	return rx_buffer.read(data);
}

std::size_t
Rtt::read(uint8_t *data, std::size_t length)
{
	uint32_t i = 0;
	for (; i < length; ++i)
		if (not rx_buffer.read(*data++))
			return i;
	return i;
}

std::size_t
Rtt::receiveBufferSize()
{
	return rx_buffer.getSize();
}



}	// namespace modm::platform