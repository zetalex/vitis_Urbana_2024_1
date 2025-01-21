# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "C:\\Users\\Alejandro\\Documents\\git\\vitis_Urbana_2024_1\\genesys2\\microblaze_0\\standalone_microblaze_0\\bsp\\include\\sleep.h"
  "C:\\Users\\Alejandro\\Documents\\git\\vitis_Urbana_2024_1\\genesys2\\microblaze_0\\standalone_microblaze_0\\bsp\\include\\xiltimer.h"
  "C:\\Users\\Alejandro\\Documents\\git\\vitis_Urbana_2024_1\\genesys2\\microblaze_0\\standalone_microblaze_0\\bsp\\include\\xtimer_config.h"
  "C:\\Users\\Alejandro\\Documents\\git\\vitis_Urbana_2024_1\\genesys2\\microblaze_0\\standalone_microblaze_0\\bsp\\lib\\libxiltimer.a"
  )
endif()
