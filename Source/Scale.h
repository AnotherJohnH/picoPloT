//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

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

