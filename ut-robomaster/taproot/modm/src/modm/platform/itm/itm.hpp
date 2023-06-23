/*
 * Copyright (c) 2019, Niklas Hauser
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

/**
 * Instruction Trace Macrocell (ITM) Uart Interface
 *
 * @author		Niklas Hauser
 * @ingroup		modm_platform_itm
 */
class Itm : public ::modm::Uart
{
public:
	static constexpr size_t RxBufferSize = 0;
	static constexpr size_t TxBufferSize = 0;

public:
	static void
	initialize();

	static void
	writeBlocking(uint8_t data);

	static inline void
	writeBlocking(const uint8_t *data, std::size_t length)
	{ while (length--) writeBlocking(*data++); }

	static void
	flushWriteBuffer();

	static bool
	write(uint8_t data);

	static std::size_t
	write(const uint8_t *data, std::size_t length);

	static bool
	isWriteFinished();

	static std::size_t
	discardTransmitBuffer();

	static inline bool
	read(uint8_t&)
	{ return false; }

	static inline std::size_t
	read(uint8_t*, std::size_t)
	{ return 0; }

	static inline std::size_t
	discardReceiveBuffer()
	{ return 0; }

	static void
	update();

protected:
	static void
	enable(uint8_t prescaler);

	static bool
	write_itm(uint32_t data, uint8_t size=1);
};

}	// namespace modm::platform