//------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

// \brief Temperature logger for Raspberry Pi Pico

#include <cstdio>

#define PRINTF if (0) printf

// --- Target Hardware Configuration --------------------------------------------------

#if defined(HW_WAVESHARE_EPAPER)

#define HW_EPAPER_WAVESHARE
#define HW_TEMP_SENSE_P21_P22_MCP9808

#elif defined(HW_BADGER2040)

#define HW_EPAPER_BADGER2040
#define HW_TEMP_SENSE_BADGER_MCP9808

#elif defined(HW_NATIVE)

#define HW_EPAPER_NATIVE
#define HW_TEMP_SENSE_FAKE

#else

#error "Target hardware not specified"

#endif


// --- EPAPER ------------------------------------------------------------------

#if defined(HW_EPAPER_WAVESHARE)

#include "MTL/EPaper_WS2_13_V3.h"

MTL::EPaper_WS2_13_V3::Canvas epaper;

constexpr unsigned WIDTH  = MTL::EPaper_WS2_13_V3::getWidth();
constexpr unsigned HEIGHT = MTL::EPaper_WS2_13_V3::getHeight();

#elif defined(HW_EPAPER_BADGER2040)

#include "MTL/badger2040.h"

MTL::badger2040::EPaper::Canvas epaper;

constexpr unsigned WIDTH  = MTL::badger2040::EPaper::getWidth();
constexpr unsigned HEIGHT = MTL::badger2040::EPaper::getHeight();

#elif defined(HW_EPAPER_NATIVE)

#include "fake/ScaledFrame.h"

constexpr unsigned WIDTH  = 296;
constexpr unsigned HEIGHT = 128;

ScaledFrame<3,16> epaper("picoPloT - simulated E-paper", WIDTH, HEIGHT);

#endif


// --- TEMP SENSE --------------------------------------------------------------

#if defined(HW_TEMP_SENSE_P21_P22_MCP9808)

#include "MTL/TempSens_MCP9808.h"
#include "MTL/rp2040/I2C.h"

MTL::TempSens_MCP9808<MTL::I2C0_P21_P22> temp_sensor;

#elif defined(HW_TEMP_SENSE_BADGER_MCP9808)

#include "MTL/TempSens_MCP9808.h"

MTL::TempSens_MCP9808<MTL::badger2040::I2C_QwSt> temp_sensor;

#elif defined(HW_TEMP_SENSE_FAKE)

#include "fake/TempSens.h"

MTL::TempSens temp_sensor;

#endif


// --- RTC ---------------------------------------------------------------------

#if defined(HW_NATIVE)

#include "fake/Rtc.h"

MTL::Rtc<7*24*60> rtc;

#else

#include "MTL/rp2040/Rtc.h"

MTL::Rtc rtc;

#endif


//------------------------------------------------------------------------------

#include "Display.h"

extern "C" void IRQ_RTC() { rtc.irq(); }

int main()
{
   PRINTF("picoPloT\n");

   temp_sensor.start();

   rtc.setDate(2024, 9, 5);
   rtc.setTime(18, 50, 0, 4);
   rtc.start();

   Display<WIDTH,HEIGHT> display(epaper);

   while(true)
   {
      // Current time NOTE previous rtc.sleep() will sample the time
      display.setDay(rtc.getDOTW(), rtc.getDay());
      display.setTime(rtc.getHour(), rtc.getMinute());

      signed temp = temp_sensor.read();
      display.setTemp(temp);

      PRINTF("DOW=%u DOM=%u\n", rtc.getDOTW(), rtc.getDay());
      PRINTF("%2u:%2u\n", rtc.getHour(), rtc.getMinute());
      PRINTF("%d.%u deg C\n", temp / 256, ((temp * 10) / 256) % 10);

      display.draw();

      // sleep for a minute since the last RTC event
      rtc.sleep(60);
   }

   return 0;
}
