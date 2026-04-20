# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\diploma_maze_problem_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\diploma_maze_problem_autogen.dir\\ParseCache.txt"
  "diploma_maze_problem_autogen"
  )
endif()
