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

#include "GUI/Canvas.h"
#include "PLT/Frame.h"

template <unsigned SCALE, unsigned BORDER = 0>
class ScaledFrame : public GUI::Canvas
{
public:
   ScaledFrame(const char* title_, unsigned width_, unsigned height_)
      : GUI::Canvas(width_, height_)
      , frame(title_, width_ * SCALE + 2 * BORDER, height_ * SCALE + 2 * BORDER)
   {
   }

private:
   void canvasClear(STB::Colour colour) override
   {
      for(int32_t y = 0; y < frame.getHeight(); y++)
      { 
         frame.span(colour, 0, y, frame.getWidth());
      }
   }

   void canvasPoint(STB::Colour colour, int32_t x_, int32_t y_) override
   {
      for(unsigned i = 0; i < SCALE; ++i)
      {
         frame.span(colour, BORDER + x_ * SCALE, BORDER + y_ * SCALE + i, BORDER + (x_ + 1) * SCALE);
      }
   }

   void canvasRefresh(int32_t x1, int32_t y1, int32_t x2, int32_t y2) override
   {
      frame.refresh();
   }

   PLT::Frame frame;
};

