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

#include <modm/platform/device.hpp>
#include "itm.hpp"

namespace modm::platform
{

void
Itm::initialize()
{
	// Enable Tracing Debug Unit
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	// Enable the Trace SWO output
	DBGMCU->CR |= DBGMCU_CR_TRACE_IOEN;
}

void
Itm::enable(uint8_t prescaler)
{
	// Trace Port Interface Selected Pin Protocol Register
	TPI->ACPR = prescaler;

	// Trace Port Interface Selected Pin Protocol Register
	TPI->SPPR = (0b10 << TPI_SPPR_TXMODE_Pos);

	// Trace Port Interface Formatter and Flush Control Register
	TPI->FFCR = (1 << TPI_FFCR_TrigIn_Pos);

	// Trace Port Interface Current Parallel Port Size Register
	TPI->CSPSR = 0b1;

	// Enable write access to ITM registers
	ITM->LAR  = 0xC5ACCE55;

	// Trace Privilege Register
	ITM->TPR  = 0;

	// Trace Control Register
	ITM->TCR  = (1 << ITM_TCR_TraceBusID_Pos) |
				(1 << ITM_TCR_DWTENA_Pos)     |
				(1 << ITM_TCR_ITMENA_Pos);

	// Trace Enable Register
	ITM->TER = 0b1;
}

void
Itm::writeBlocking(uint8_t data)
{
	while(not write(data)) ;
}

void
Itm::flushWriteBuffer()
{
	return;
}

bool
Itm::write(uint8_t data)
{
	return write_itm(data);
}

std::size_t
Itm::write(const uint8_t *data, std::size_t length)
{
	std::size_t sent = 0;
	for (; sent < length; sent++)
		if (not write(*data++))
			return sent;
	return sent;
}

bool
Itm::isWriteFinished()
{
	return true;
}

std::size_t
Itm::discardTransmitBuffer()
{
	return 0;
}

bool
Itm::write_itm(uint32_t data, uint8_t size)
{
	if ((ITM->TCR & ITM_TCR_ITMENA_Msk) and (ITM->TER & 0b1))
	{
		// still busy with previous write
		if (ITM->PORT[0].u32 == 0)
			return false;

		switch(size)
		{
			case 1:
				ITM->PORT[0].u8 = data >> 24;
				break;
			case 2:
				ITM->PORT[0].u16 = data >> 16;
				break;
			case 4:
				ITM->PORT[0].u32 = data;
				break;
			default:
				return false;
		}
		return true;
	}
	// if ITM is disabled, just discard any writes
	return true;
}

void
Itm::update()
{
}

}	// namespace modm::platform
