# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\diploma_bucket_problem_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\diploma_bucket_problem_autogen.dir\\ParseCache.txt"
  "diploma_bucket_problem_autogen"
  )
endif()
