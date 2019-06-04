#!/usr/bin/env bash

if ! [ -n "$1" ]
then
   echo "Укажите сборочную папку"
   exit 1
fi

"$1"/bin/wav_mixer data/groove-stereo.wav data/blue-stereo.wav groove-blue-stereo.wav
"$1"/bin/wav_mixer data/starface-stereo.wav data/blue-stereo.wav starface-blue-stereo.wav
"$1"/bin/wav_mixer data/groove-mono.wav data/blue-mono.wav groove-blue-mono.wav
"$1"/bin/wav_mixer data/starface-mono.wav data/blue-mono.wav starface-blue-mono.wav
