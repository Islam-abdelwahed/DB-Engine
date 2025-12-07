# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\DB-engine_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\DB-engine_autogen.dir\\ParseCache.txt"
  "DB-engine_autogen"
  )
endif()
