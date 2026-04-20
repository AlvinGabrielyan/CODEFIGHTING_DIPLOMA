# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\DiplomaLauncher_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\DiplomaLauncher_autogen.dir\\ParseCache.txt"
  "DiplomaLauncher_autogen"
  )
endif()
