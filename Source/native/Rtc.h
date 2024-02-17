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

#pragma once

#include <cstdlib>

#include "PLT/Event.h"

extern "C" void IRQ_RTC();

namespace MTL {

class Rtc
{
public:
   Rtc() = default;

   unsigned getDOTW()   const { return dow; }
   unsigned getDay()    const { return day; }
   unsigned getHour()   const { return hour; }
   unsigned getMinute() const { return minute; }

   void setDate(unsigned year_, unsigned month_, unsigned day_)
   {
      year  = year_;
      month = month_;
      day   = day_;
   }

   void setTime(unsigned hour_, unsigned minute_, unsigned second_, unsigned dow_)
   {
      dow    = dow_;
      hour   = hour_;
      minute = minute_;
      second = second_;
   }

   void start() {}

   void sleep(unsigned seconds_)
   {
      IRQ_RTC();

      second += seconds_;
      if (second < 60) return;

      minute += second / 60;
      second  = second % 60;
      if (minute < 60) return;

      hour  += minute / 60;
      minute = minute % 60;
      if (hour < 24) return;
      
      day   += hour / 24;
      dow    = (dow + (hour / 24)) % 7;
      hour   = hour % 24;
   }

   void irq()
   {
      PLT::Event::Message event;
      PLT::Event::poll(event);

      if (count++ == 24 * 60 * 7)
      {
         PLT::Event::eventLoop();
         exit(0);
      }
   }

private:
   unsigned count {0};
   unsigned year;
   unsigned month;
   unsigned day;
   unsigned dow;
   unsigned hour;
   unsigned minute;
   unsigned second;
};

} // namespace MTL
