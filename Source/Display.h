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

// \brief

#pragma once

#include <cstdio>

#include "GUI/Font/Teletext.h"
#include "GUI/Canvas.h"

#include "Scale.h"
#include "History.h"

class Display
{
public:
   Display(GUI::Canvas& canvas_)
      : canvas(canvas_)
   {
      time_scale.setRange(-HIST_HOURS * 60, 0);
   }

   unsigned getSamplePeriodSecs() const
   {
      return MINS_PER_PIXEL * 60;
   }

   void setDay(unsigned dow_, unsigned dom_)
   {
      cur_dow = dow_;
      cur_dom = dom_;
   }

   void setTime(unsigned hours_, unsigned mins_)
   {
      cur_hours = hours_;
      cur_mins  = mins_;
   }

   void recordHumidity(unsigned value_)
   {
      history_humd.push(value_);
   }

   void recordTemp(signed value_)
   {
      history_temp.push((value_ * 10) / 256);
      history_temp.stats(min_temp, max_temp, avg_temp);
   }

   void draw(bool partial)
   {
      char text[10];

      canvas.clear(WHITE);

      printTemp(168,  2, &GUI::font_teletext18, history_temp[0], /* brief */ true);

      printText(180, 22, &GUI::font_teletext12, "mx");
      printTemp(196, 24, &GUI::font_teletext12, max_temp, /* brief */ true);

      printText(180, 36, &GUI::font_teletext12, "av");
      printTemp(196, 38, &GUI::font_teletext12, avg_temp, /* brief */ true);

      printText(180, 50, &GUI::font_teletext12, "mn");
      printTemp(196, 52, &GUI::font_teletext12, min_temp, /* brief */ true);

      snprintf(text, sizeof(text), "%02u:%02u", cur_hours, cur_mins);
      printText(180, 108, &GUI::font_teletext18, text);

      temp_scale.setRange(min_temp - 10, max_temp + 10);

      drawVertScale();
      drawHorzScale();

      for(signed i = 0; i < history_temp.size(); i++)
      {
         unsigned x = time_scale.getPos(-i * MINS_PER_PIXEL);
         unsigned y = temp_scale.getPos(history_temp[i]);

         canvas.drawPoint(BLACK, x, y);
         canvas.drawPoint(BLACK, x + 1, y);
         canvas.drawPoint(BLACK, x - 1, y);
         canvas.drawPoint(BLACK, x, y + 1);
         canvas.drawPoint(BLACK, x, y - 1);
      }

      snprintf(text, sizeof(text), "%2u.%u", history_humd[0] / 10, history_humd[0] % 10);
      printText(208, 70, &GUI::font_teletext12, text);
      printText(244, 70, &GUI::font_teletext12, "%%");

      static const char* dow[7] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
      printText(180, 90, &GUI::font_teletext15, dow[cur_dow]);

      snprintf(text, sizeof(text), "%2u", cur_dom);
      printText(219, 90, &GUI::font_teletext15, text);

      if (partial)
         canvas.partialRefresh();
      else
         canvas.refresh();
   }

private:
   void printText(unsigned x, unsigned y, const GUI::Font* font, const char* text)
   {
      canvas.drawText(BLACK, WHITE, x, y, font, text);
   }

   void printTemp(unsigned x, unsigned y, const GUI::Font* font, signed value, bool brief)
   {
      char text[8];

      char* s = text;

      if (value < 0)
         *s++ = '-';
      else if (value > 0)
         *s++ = ' ';
      else
         *s++ = ' ';

      snprintf(s, 3, "%2d", value / 10);
      s += 2;

      if (brief)
      {
         *s++ = '.';
         snprintf(s, 2, "%d", value % 10);
         s += 1;

         *s++ = 0x7F;
         *s++ = 'C';
      }

      *s = '\0';

      printText(x, y, font, text);
   }

   void drawVertScale()
   {
      canvas.drawLine(BLACK,
                      PLOT_X_LFT, temp_scale.getMinPos(),
                      PLOT_X_LFT, temp_scale.getMaxPos());

      canvas.drawLine(BLACK,
                      PLOT_X_RGT, temp_scale.getMaxPos(),
                      PLOT_X_RGT, temp_scale.getMinPos());

      signed   min = ((temp_scale.getMinVal() + 9) / 10) * 10;
      signed   max = ((temp_scale.getMaxVal() + 9) / 10) * 10;

      unsigned grid = (max - min) < 100 ? 10 : 50;
      unsigned tick = grid / 5;

      for(unsigned value = min; value < max; value += tick)
      {
         unsigned y = temp_scale.getPos(value);
         unsigned tick_len;

         if ((value % grid) == 0)
         {
            tick_len = 3;

            printTemp(PLOT_X_LFT - 20, y - 3, &GUI::font_teletext9, value, /* brief */ false);

            for(unsigned x = PLOT_X_LFT + 5; x < PLOT_X_RGT; x+= 5)
               canvas.drawPoint(BLACK, x, y);
         }
         else
         {
            tick_len = 2;
         }

         canvas.drawLine(BLACK, PLOT_X_LFT, y, PLOT_X_LFT - tick_len, y);
         canvas.drawLine(BLACK, PLOT_X_RGT, y, PLOT_X_RGT + tick_len, y);
      }
   }

   void drawHorzScale()
   {
      canvas.drawLine(BLACK,
                      time_scale.getMaxPos(), PLOT_Y_BTM,
                      time_scale.getMinPos(), PLOT_Y_BTM);

      canvas.drawLine(BLACK,
                      time_scale.getMinPos(), PLOT_Y_TOP,
                      time_scale.getMaxPos(), PLOT_Y_TOP);

      unsigned cur_time = cur_hours * 60 + cur_mins;

      for(unsigned tick_time = 0; tick_time < HIST_HOURS * 60; tick_time += 60)
      {
         signed mins_in_past = cur_time - tick_time;
         if (mins_in_past < 0)
            mins_in_past += 24 * 60;

         unsigned x = time_scale.getPos(-mins_in_past);
         unsigned tick_len;

         if ((tick_time % 180) == 0)
         {
            tick_len = 3;

            char text[3];
            snprintf(text, sizeof(text), "%2d", tick_time / 60);
            printText(x - 4, PLOT_Y_BTM + 3, &GUI::font_teletext9, text);

            for(unsigned y = PLOT_Y_TOP + 5; y < PLOT_Y_BTM; y+= 5)
               canvas.drawPoint(BLACK, x, y);

         }
         else
         {
            tick_len = 2;
         }

         canvas.drawLine(BLACK, x, PLOT_Y_BTM, x, PLOT_Y_BTM + tick_len);
         canvas.drawLine(BLACK, x, PLOT_Y_TOP, x, PLOT_Y_TOP - tick_len);
      }
   }

   static const unsigned HIST_HOURS     = 24;
   static const unsigned MINS_PER_PIXEL = 10;
   static const unsigned SAMPLES        = HIST_HOURS * 60 / MINS_PER_PIXEL;

   static const unsigned PLOT_X_LFT     = 20;
   static const unsigned PLOT_Y_TOP     = 3;
   static const unsigned PLOT_X_RGT     = PLOT_X_LFT + HIST_HOURS * 60 / MINS_PER_PIXEL;
   static const unsigned PLOT_Y_BTM     = 122 - 10;

   static const STB::Colour WHITE = STB::RGB(0xC0, 0xC0, 0xC0);
   static const STB::Colour BLACK = STB::RGB(0x00, 0x00, 0x00);

   unsigned                  cur_dow{};
   unsigned                  cur_dom{};
   unsigned                  cur_hours{};
   unsigned                  cur_mins{};
   History<signed,SAMPLES>   history_temp;
   History<unsigned,SAMPLES> history_humd;
   signed                    min_temp{};
   signed                    max_temp{};
   signed                    avg_temp{};

   GUI::Canvas& canvas;
   Scale        temp_scale{PLOT_Y_BTM, PLOT_Y_TOP};
   Scale        humd_scale{PLOT_Y_BTM, PLOT_Y_TOP};
   Scale        time_scale{PLOT_X_LFT, PLOT_X_RGT};
};

