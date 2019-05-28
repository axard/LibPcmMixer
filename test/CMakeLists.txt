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
