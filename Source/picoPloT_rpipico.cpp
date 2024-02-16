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

// \brief picoPloT - main program

#include <cstdio>

#include "MTL/EPaper.h"
#include "MTL/EPaperCanvas.h"
#include "MTL/TempSens.h"
#include "MTL/rp2040/Rtc.h"

#include "Display.h"

#define PRINTF if (0) printf

MTL::EPaperCanvas<MTL::EPaper_213_V3, /* SWAP_XY */ true> epaper;
MTL::TempSens_MCP9808<MTL::I2C0_P21_P22>                  temp_sensor;
MTL::Rtc                                                  rtc;

extern "C" void IRQ_RTC() { rtc.irq(); }

int MTL_main()
{
   PRINTF("picoPloT\n");

   temp_sensor.start();

   rtc.setDate(2024, 2, 16);
   rtc.setTime(20, 50, 0, 5);
   rtc.start();

   Display display(epaper);

   unsigned temp_cycle = 0;

   while(true)
   {
      // Current time
      display.setDay(rtc.getDOTW(), rtc.getDay());
      display.setTime(rtc.getHour(), rtc.getMinute());

      if (temp_cycle-- == 0)
      {
         temp_cycle = display.getSamplePeriodMins() - 1;

         signed temp = temp_sensor.read();
         PRINTF("%d.%u deg C\n", temp / 256, ((temp * 10) / 256) % 10);

         display.recordTemp(temp);
         display.draw(/* partial */ false);

         // XXX Seems to take four partial draws to recover from a full draw
         for(unsigned i = 0; i < 3; ++i)
            display.draw(/* partial */ true);
      }
      else
      {
         display.draw(/* partial */ true);
      }

      // sleep for a minute since the last RTC event
      rtc.sleep(60);
   }

   return 0;
}

