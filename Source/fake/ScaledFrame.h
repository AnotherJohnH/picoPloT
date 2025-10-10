//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

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

