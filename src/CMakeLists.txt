add_library(pcmmix STATIC SHARED
  PcmMixer.cpp
  PcmChannel.cpp
  PcmMix.cpp
  PcmChunk.cpp)

target_include_directories(pcmmix
  PRIVATE
  ${PROJECT_INCLUDE_DIR}
  ${GTEST_INCLUDE_DIR}
  )

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(pcmmix
    PRIVATE
    -Wall -std=c++11 -O0 -g
    )
else()
  target_compile_options(pcmmix
    PRIVATE
    -Wall -std=c++11 -O2
    )
endif()


