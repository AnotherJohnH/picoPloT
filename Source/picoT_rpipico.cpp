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

// \brief picoT - main program

#include "MTL/EPaper.h"
#include "MTL/EPaperCanvas.h"
#include "MTL/rp2040/Rtc.h"

#include "Display.h"

int MTL_main()
{
   MTL::EPaperCanvas<MTL::EPaper_213_V3, /* SWAP_XY */ true> epaper;
   MTL::Rtc                                                  rtc;

   rtc.setDate(2024, 1, 23, 2);
   rtc.setTime(19, 26, 0);
   rtc.start();

   Display display(epaper);

   signed temp = 103;

   for(unsigned i = 0; i < 144; ++i)
   {
      rtc.sample();
      display.setDay(rtc.getDOTW(), rtc.getDay());
      display.setTime(rtc.getHour(), rtc.getMinute());
      display.recordTemp(temp);
      temp++;

      display.draw();

      usleep(60000000);
   }

   return 0;
}

