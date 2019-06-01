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
    // Алгоритм смешивания взял здесь: https://stackoverflow.com/a/25102339
    // и здесь http://www.vttoth.com/CMS/index.php/technical-notes/68

    static constexpr std::int32_t int16_max  = INT16_MAX + 1;
    static constexpr std::int32_t int16_half = (INT16_MAX + 1) / 2;
    static constexpr std::int32_t uint16_max = UINT16_MAX + 1;

    std::int32_t au = a;
    std::int32_t bu = b;

    au += int16_max;
    bu += int16_max;

    double af = static_cast<double>(au);
    double bf = static_cast<double>(bu);

    double mf = 0;

    auto isQuiet = [&](std::int32_t value) {
        return (value > int16_half) && (value < int16_max + int16_half);
    };

    if (isQuiet(au) && isQuiet(bu)) {
        mf = af * bf / int16_max;
    } else {
        // Без этого доп условия "громкие" полностью противофазные семплы не будут обнуляться
        auto ru  = std::minmax(au, bu);
        auto dau = int16_max - ru.first;
        auto dbu = ru.second - int16_max;
        if (dau == dbu) {
            mf = int16_max;
        } else {
            mf = 2 * (af + bf) - (af * bf) / int16_max - uint16_max;
        }
    }

    std::int32_t m = std::round(mf);

    if (m >= uint16_max) {
        m = UINT16_MAX;
    }

    m -= int16_max;

    return static_cast<std::int16_t>(m);
}

} // namespace Pcm
