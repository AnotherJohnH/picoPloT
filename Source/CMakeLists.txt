#-------------------------------------------------------------------------------
# Copyright (c) 2024 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

foreach(hw_config ${hw_picoSense_config_list})

   add_executable(picoPloT_${hw_config} picoPloT.cpp)

   target_compile_definitions(picoPloT_${hw_config} PRIVATE HW_${hw_config})

   target_include_directories(picoPloT_${hw_config} PRIVATE .)

   target_link_libraries(picoPloT_${hw_config} PRIVATE GUI HW)

   set_target_properties(picoPloT_${hw_config} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

endforeach()
