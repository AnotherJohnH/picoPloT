#-------------------------------------------------------------------------------
#  Copyright (c) 2024 John D. Haughton
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#  SOFTWARE.
#-------------------------------------------------------------------------------

if(${PLT_TARGET} STREQUAL rpipico)

   set(config_list
       BADGER2040
       WAVESHARE_EPAPER)

   set(target_libs TNY MTL TNY GUI)

elseif(${PLT_TARGET} STREQUAL native)

   set(config_list
       NATIVE)

   set(target_libs PLT STB GUI)

endif()

foreach(config ${config_list})

   add_executable(picoPloT_${config} picoPloT.cpp)

   target_compile_definitions(picoPloT_${config} PRIVATE HW_${config})

   target_include_directories(picoPloT_${config} PRIVATE .)

   target_link_libraries(picoPloT_${config} PRIVATE ${target_libs})

   set_target_properties(picoPloT_${config} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

endforeach()
