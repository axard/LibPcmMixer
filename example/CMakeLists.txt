add_executable(wav_mixer
  wav_mixer.cpp
  )

target_compile_options(wav_mixer
  PRIVATE
  -Wall -std=c++11 -O0)

target_include_directories(wav_mixer
  PRIVATE
  ${PROJECT_INCLUDE_DIR}
  )

target_link_libraries(wav_mixer
  PRIVATE
  pcmmix
  )
