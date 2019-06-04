#include <cmath>
#include <stdexcept>
#include <PcmMix.hpp>
#include <algorithm>

namespace Pcm {

Chunk Mix(const Chunk& a, const Chunk& b, ByteOrder byteOrder)
{
    // По алгоритму Pcm::Mixer::PushChunk() получается, что таймштампы у чанков в канале будут
    // синхронизироваться, но лучше провеерить
    if (a.Timestamp != b.Timestamp) {
        throw std::invalid_argument{ "Timestamp mismatch on mix state" };
    }

    Chunk result{ a.Timestamp };

    auto ia = a.Data.begin();
    auto ib = b.Data.begin();

    while (ia != a.Data.end() && ib != b.Data.end()) {
        int16_t sa = 0;
        int16_t sb = 0;

        switch (byteOrder) {
        case ByteOrder::BigEndian:

            sa = *ia << 8;
            ++ia;
            sa |= *ia;
            ++ia;

            sb = *ib << 8;
            ++ib;
            sb |= *ib;
            ++ib;

            break;

        case ByteOrder::LittleEndian:

            sa = *ia;
            ++ia;
            sa |= *ia << 8;
            ++ia;

            sb = *ib;
            ++ib;
            sb |= *ib << 8;
            ++ib;

            break;
        }

        int16_t sm = Mix(sa, sb);

        const uint8_t* buf = reinterpret_cast<const uint8_t*>(&sm);

        switch (byteOrder) {
        case ByteOrder::BigEndian:
            result.Data.push_back(buf[ 1 ]);
            result.Data.push_back(buf[ 0 ]);
            break;

        case ByteOrder::LittleEndian:
            result.Data.push_back(buf[ 0 ]);
            result.Data.push_back(buf[ 1 ]);
            break;
        }
    }

    return result;
}

std::int16_t Mix(std::int16_t a, std::int16_t b)
{
    double af = static_cast<double>(a);
    double bf = static_cast<double>(b);

    double mf = (af + bf) / 2;

    std::int32_t m = std::round(mf);

    if (m > INT16_MAX) {
        m = INT16_MAX;
    }

    if (m < INT16_MIN) {
        m = INT16_MIN;
    }

    return static_cast<std::int16_t>(m);
}

} // namespace Pcm
