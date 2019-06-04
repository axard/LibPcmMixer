#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <utility>
#include <memory>
#include <algorithm>

#include <PcmMixer.hpp>

// Структура, описывающая заголовок WAV файла.
struct WavHeader
{
    // WAV-формат начинается с RIFF-заголовка:

    // Содержит символы "RIFF" в ASCII кодировке
    // (0x52494646 в big-endian представлении)
    char chunkId[ 4 ];

    // 36 + subchunk2Size, или более точно:
    // 4 + (8 + subchunk1Size) + (8 + subchunk2Size)
    // Это оставшийся размер цепочки, начиная с этой позиции.
    // Иначе говоря, это размер файла - 8, то есть,
    // исключены поля chunkId и chunkSize.
    uint32_t chunkSize;

    // Содержит символы "WAVE"
    // (0x57415645 в big-endian представлении)
    char format[ 4 ];

    // Формат "WAVE" состоит из двух подцепочек: "fmt " и "data":
    // Подцепочка "fmt " описывает формат звуковых данных:

    // Содержит символы "fmt "
    // (0x666d7420 в big-endian представлении)
    char subchunk1Id[ 4 ];

    // 16 для формата PCM.
    // Это оставшийся размер подцепочки, начиная с этой позиции.
    uint32_t subchunk1Size;

    // Аудио формат, полный список можно получить здесь http://audiocoding.ru/wav_formats.txt
    // Для PCM = 1 (то есть, Линейное квантование).
    // Значения, отличающиеся от 1, обозначают некоторый формат сжатия.
    uint16_t audioFormat;

    // Количество каналов. Моно = 1, Стерео = 2 и т.д.
    uint16_t numChannels;

    // Частота дискретизации. 8000 Гц, 44100 Гц и т.д.
    uint32_t sampleRate;

    // sampleRate * numChannels * bitsPerSample/8
    uint32_t byteRate;

    // numChannels * bitsPerSample/8
    // Количество байт для одного сэмпла, включая все каналы.
    uint16_t blockAlign;

    // Так называемая "глубиная" или точность звучания. 8 бит, 16 бит и т.д.
    uint16_t bitsPerSample;

    // Подцепочка "data" содержит аудио-данные и их размер.

    // Содержит символы "data"
    // (0x64617461 в big-endian представлении)
    char subchunk2Id[ 4 ];

    // numSamples * numChannels * bitsPerSample/8
    // Количество байт в области данных.
    uint32_t subchunk2Size;

    float length() const
    {
        return 1.f * subchunk2Size / (bitsPerSample / 8) / numChannels / sampleRate;
    }
};

std::ostream& operator<<(std::ostream& os, const WavHeader& header)
{
    os << "Chunk ID:        ";
    os.write(header.chunkId, 4);
    os << std::endl;

    os << "Сhunk size:      " << header.chunkSize << std::endl;
    os << "Format:          ";
    os.write(header.format, 4);
    os << std::endl;

    os << "Subchunk1 ID:    ";
    os.write(header.subchunk1Id, 4);
    os << std::endl;

    os << "Subchunk1Size:   " << header.subchunk1Size << std::endl
       << "Audio format:    " << header.audioFormat << std::endl
       << "Sample rate:     " << header.sampleRate << std::endl
       << "Channels:        " << header.numChannels << std::endl
       << "Bits per sample: " << header.bitsPerSample << std::endl
       << "Length, s:       " << header.length() << std::endl;

    return os;
}

struct Wav
{
    WavHeader                  header;
    std::unique_ptr<uint8_t[]> data;

    static Wav fromFile(const char* fileName)
    {
        std::ifstream wavFile{ fileName, std::ios_base::binary | std::ios_base::in };

        if (!wavFile) {
            throw std::runtime_error{ std::string{ "Ошибка открытия файла " } + fileName };
        }

        Wav wav;
        wavFile.read(reinterpret_cast<char*>(&wav.header), sizeof(WavHeader));
        wav.data = std::unique_ptr<uint8_t[]>(new uint8_t[ wav.header.subchunk2Size ]);
        wavFile.read(reinterpret_cast<char*>(wav.data.get()), wav.header.subchunk2Size);

        return wav;
    }

    void toFile(const char* fileName) const
    {
        std::ofstream wavFile{ fileName, std::ios_base::binary | std::ios_base::out };

        if (!wavFile) {
            throw std::runtime_error{ std::string{ "Ошибка открытия файла " } + fileName };
        }

        wavFile.write(reinterpret_cast<const char*>(&header), sizeof(WavHeader));
        wavFile.write(reinterpret_cast<const char*>(data.get()), header.subchunk2Size);
    }
};

int main(int argc, char* argv[])
{
    if (argc < 4) {
        std::cout << "Недостаточно аргументов." << std::endl;
    }

    auto left  = Wav::fromFile(argv[ 1 ]);
    auto right = Wav::fromFile(argv[ 2 ]);

    auto size = std::min(left.header.subchunk2Size, right.header.subchunk2Size);
    std::cout << size << std::endl;

    Wav mix;
    mix.data = std::unique_ptr<uint8_t[]>(new uint8_t[ size ]);

    std::cout << "Файл '" << argv[ 1 ] << "':" << std::endl << left.header << std::endl;
    std::cout << "Файл '" << argv[ 2 ] << "':" << std::endl << right.header << std::endl;

    Pcm::Mixer mixer{ Pcm::ByteOrder::LittleEndian };
    mixer.SetOnChunkReadyCallback([&mix](const uint8_t* data, size_t size, size_t timestamp) {
        std::copy(data, data + size, mix.data.get() + timestamp * Pcm::SAMPLE_SIZE);
    });

    mixer.PushChunk(left.data.get(), size, 0, 0);
    mixer.PushChunk(right.data.get(), size, 0, 1);
    mixer.Flush();

    std::cout << "Mixing done" << std::endl;

    mix.header               = left.header;
    mix.header.subchunk2Size = size;
    mix.header.chunkSize     = 36 + size;

    std::cout << "Файл '" << argv[ 3 ] << "':" << std::endl << mix.header << std::endl;

    mix.toFile(argv[ 3 ]);

    std::cout << "Saving done" << std::endl;

    return 0;
}
