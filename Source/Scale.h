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

class Scale
{
public:
   Scale(unsigned start_, unsigned end_)
      : pos_min(start_)
      , pos_max(end_)
   {
   }

   unsigned getMinPos() const { return pos_min; }
   unsigned getMaxPos() const { return pos_max; }

   signed getMinVal() const { return val_min; }
   signed getMaxVal() const { return val_max; }

   void setRange(signed min_, signed max_)
   {
      val_min  = min_;
      val_max  = max_;
      scale_fp = ((signed(pos_max) - signed(pos_min)) << FIXP_SHIFT) / (val_max - val_min);
   }

   unsigned getPos(signed value_) const
   {
      return pos_min + (((value_ - val_min) * scale_fp) >> FIXP_SHIFT);
   }

private:
   static const unsigned FIXP_SHIFT = 16;

   unsigned pos_min;
   unsigned pos_max;
   signed   val_min;
   signed   val_max;
   signed   scale_fp;
};

