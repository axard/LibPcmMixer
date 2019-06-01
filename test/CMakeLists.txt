# Pcm::Mixer
# ------------------------------------------------------------------------------

add_executable(test_pcm_mixer
  test_pcm_mixer.cpp
  )

target_compile_options(test_pcm_mixer
  PRIVATE
  -Wall -std=c++11 -O0)

target_include_directories(test_pcm_mixer
  PRIVATE
  ${PROJECT_INCLUDE_DIR}
  ${GTEST_INCLUDE_DIR}
  )

target_link_libraries(test_pcm_mixer
  PRIVATE
  pcmmix
  ${GTEST_LIBRARIES}
  Threads::Threads
  )

add_test(NAME test_pcm_mixer
  WORKING_DIRECTORY bin
  COMMAND test_pcm_mixer
  )

# Pcm::Chunk
# ------------------------------------------------------------------------------

add_executable(test_pcm_chunk
  test_pcm_chunk.cpp
  )

target_compile_options(test_pcm_chunk
  PRIVATE
  -Wall -std=c++11 -O0)

target_include_directories(test_pcm_chunk
  PRIVATE
  ${PROJECT_INCLUDE_DIR}
  ${GTEST_INCLUDE_DIR}
  )

target_link_libraries(test_pcm_chunk
  PRIVATE
  pcmmix
  ${GTEST_LIBRARIES}
  Threads::Threads
  )

add_test(NAME test_pcm_chunk
  WORKING_DIRECTORY bin
  COMMAND test_pcm_chunk
  )

# Pcm::Channel
# ------------------------------------------------------------------------------

add_executable(test_pcm_channel
  test_pcm_channel.cpp
  )

target_compile_options(test_pcm_channel
  PRIVATE
  -Wall -std=c++11 -O0)

target_include_directories(test_pcm_channel
  PRIVATE
  ${PROJECT_INCLUDE_DIR}
  ${GTEST_INCLUDE_DIR}
  )

target_link_libraries(test_pcm_channel
  PRIVATE
  pcmmix
  ${GTEST_LIBRARIES}
  Threads::Threads
  )

add_test(NAME test_pcm_channel
  WORKING_DIRECTORY bin
  COMMAND test_pcm_channel
  )

# Pcm::Mix
# ------------------------------------------------------------------------------

add_executable(test_pcm_mix
  test_pcm_mix.cpp
  )

target_compile_options(test_pcm_mix
  PRIVATE
  -Wall -std=c++11 -O0)

target_include_directories(test_pcm_mix
  PRIVATE
  ${PROJECT_INCLUDE_DIR}
  ${GTEST_INCLUDE_DIR}
  )

target_link_libraries(test_pcm_mix
  PRIVATE
  pcmmix
  ${GTEST_LIBRARIES}
  Threads::Threads
  )

add_test(NAME test_pcm_mix
  WORKING_DIRECTORY bin
  COMMAND test_pcm_mix
  )
