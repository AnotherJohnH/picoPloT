#-------------------------------------------------------------------------------
# Copyright (c) 2024 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

if(${PLT_TARGET} STREQUAL rpipico)

   set(hw_config_list
       BADGER2040
       WAVESHARE_EPAPER)

   set(target_libs TNY MTL GUI)

elseif(${PLT_TARGET} STREQUAL native)

   set(hw_config_list
       NATIVE)

   set(target_libs GUI PLT STB)

endif()

foreach(hw_config ${hw_config_list})

   add_executable(picoPloT_${hw_config} picoPloT.cpp)

   target_compile_definitions(picoPloT_${hw_config} PRIVATE HW_${hw_config})

   target_include_directories(picoPloT_${hw_config} PRIVATE .)

   target_link_libraries(picoPloT_${hw_config} PRIVATE ${target_libs})

   set_target_properties(picoPloT_${hw_config} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

endforeach()
