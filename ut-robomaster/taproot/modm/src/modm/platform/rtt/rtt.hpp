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

#pragma once
#include <modm/architecture/interface/uart.hpp>

namespace modm::platform
{

struct RttBuffer;

/**
 * Real Time Transfer (RTT) Uart Interface
 *
 * @author		Niklas Hauser
 * @ingroup		modm_platform_rtt
 */
class Rtt : public ::modm::Uart
{
	RttBuffer& tx_buffer;
	RttBuffer& rx_buffer;

public:
	Rtt(uint8_t channel);

	inline void
	writeBlocking(uint8_t data)
	{ while(not write(data)) ; }

	inline void
	writeBlocking(const uint8_t *data, std::size_t length)
	{ while (length--) writeBlocking(*data++); }

	inline void
	flushWriteBuffer() {}

	bool
	write(uint8_t data);

	std::size_t
	write(const uint8_t *data, std::size_t length);

	bool
	isWriteFinished();

	std::size_t
	transmitBufferSize();

	inline std::size_t
	discardTransmitBuffer()
	{ return 0; }

	bool
	read(uint8_t &data);

	std::size_t
	read(uint8_t *data, std::size_t length);

	std::size_t
	receiveBufferSize();

	inline std::size_t
	discardReceiveBuffer()
	{ return 0; }

	inline bool
	hasError()
	{ return false; }

	inline void
	clearError() {}
};

}	// namespace modm::platform