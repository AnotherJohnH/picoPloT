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

template <typename TYPE,unsigned SIZE>
class History
{
public:
   History() = default;

   void push(TYPE value)
   {
      buffer[head++] = value;

      if (head == SIZE)
         head = 0;

      if (head == tail)
         if (++tail == SIZE)
            tail = 0;
   }

   size_t size() const
   {
      return head >= tail ? head - tail
                          : head + SIZE - tail;
   }

   TYPE operator[](size_t index) const
   {
      size_t first = head > 0 ? head - 1 : SIZE - 1;
              
      return first >= index ? buffer[first - index]
                            : buffer[first + SIZE - index];
   }

private:
   size_t head{0};
   size_t tail{0};
   TYPE   buffer[SIZE];
};

