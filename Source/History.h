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

#include <cstddef>
#include <limits>

template <typename TYPE,unsigned SIZE>
class History
{
public:
   History() = default;

   void push(TYPE value)
   {
      buffer[head++] = value;

      if (head == BUF_SIZE)
         head = 0;

      if (head == tail)
         if (++tail == BUF_SIZE)
            tail = 0;
   }

   size_t size() const
   {
      return head >= tail ? head - tail
                          : head + BUF_SIZE - tail;
   }

   bool empty() const { return size() == 0; }

   const TYPE& at(size_t index) const
   {
      size_t first = head > 0 ? head - 1 : BUF_SIZE - 1;

      return first >= index ? buffer[first - index]
                            : buffer[first + BUF_SIZE - index];
   }

   TYPE& at(size_t index)
   {
      size_t first = head > 0 ? head - 1 : BUF_SIZE - 1;

      return first >= index ? buffer[first - index]
                            : buffer[first + BUF_SIZE - index];
   }

   const TYPE& operator[](size_t index) const { return at(index); }
   TYPE&       operator[](size_t index)       { return at(index); }

   TYPE min() const
   {
      TYPE lowest = std::numeric_limits<TYPE>::max();

      for(size_t i = 0; i < size(); ++i)
      {
         TYPE value = at(i);

         if (value < lowest)
            lowest = value;
      }

      return lowest;
   }

   TYPE max() const
   {
      TYPE highest = std::numeric_limits<TYPE>::lowest();

      for(size_t i = 0; i < size(); ++i)
      {
         TYPE value = at(i);

         if (value > highest)
            highest = value;
      }

      return highest;
   }

private:
   static const size_t BUF_SIZE = SIZE + 1;

   size_t head{0};
   size_t tail{0};
   TYPE   buffer[BUF_SIZE];
};

