/*-------------------------------------------------------------------------
NeoPixel library helper functions for DotStars using general Pins (APA102/LPD8806).

Written by Michael C. Miller.

I invest time and resources providing this open source code,
please support me by dontating (see https://github.com/Makuna/NeoPixelBus)

-------------------------------------------------------------------------
This file is part of the Makuna/NeoPixelBus library.

NeoPixelBus is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

NeoPixelBus is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with NeoPixel.  If not, see
<http://www.gnu.org/licenses/>.
-------------------------------------------------------------------------*/

#pragma once

#include <SPI.h>

class SpiSpeed80Mhz
{
public:
    static const uint32_t Clock = 80000000L;
};
class SpiSpeed40Mhz
{
public:
    static const uint32_t Clock = 40000000L;
};
class SpiSpeed30Mhz
{
public:
    static const uint32_t Clock = 30000000L;
};
class SpiSpeed20Mhz
{
public:
    static const uint32_t Clock = 20000000L;
};

class SpiSpeed10Mhz
{
public:
    static const uint32_t Clock = 10000000L;
};

class SpiSpeed2Mhz
{
public:
    static const uint32_t Clock = 2000000L;
};

template<typename T_SPISPEED> class TwoWireSpiImple
{
public:
	SPIClass * SPI = NULL;
    TwoWireSpiImple(uint8_t, uint8_t) // clock and data pins ignored for hardware SPI
    {
			SPI = new SPIClass(VSPI);

    }

    ~TwoWireSpiImple()
    {
        SPI->end();
    }

#if defined(ARDUINO_ARCH_ESP32)
    // for cases where hardware SPI can have pins changed
    void begin(int8_t sck, int8_t miso, int8_t mosi, int8_t ss)
    {
        SPI->begin(sck, miso, mosi, ss);
    }
#endif

    void begin()
    {
        SPI->begin();
    }

    void beginTransaction()
    {
        SPI->beginTransaction(SPISettings(T_SPISPEED::Clock, MSBFIRST, SPI_MODE0));
    }

    void endTransaction()
    {
        SPI->endTransaction();
    }

    void transmitByte(uint8_t data)
    {
        SPI->transfer(data);
    }

    void transmitBytes(const uint8_t* data, size_t dataSize)
    {
#if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)
        // ESPs have a method to write without inplace overwriting the send buffer
        // since we don't care what gets received, use it for performance
        // FIX: but for what ever reason on Esp32, its not const
        SPI->writeBytes(const_cast<uint8_t*>(data), dataSize);

#else
        // default ARDUINO transfer inplace overwrites the send buffer
        // which is bad in this case, so we have to send one byte at a time
        const uint8_t* endData = data + dataSize;
        while (data < endData)
        {
            SPI->transfer(*data++);
        }
#endif
    }

private:
};