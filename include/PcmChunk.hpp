#ifndef PCMCHUNK_H
#define PCMCHUNK_H

#include <vector>
#include <cstdint>
#include <algorithm>

namespace Pcm {

struct Chunk
{
    std::vector<std::uint8_t> Data;
    std::size_t               Timestamp;

    Chunk(const std::uint8_t* data, std::size_t size, std::size_t timestamp)
        : Data(data, data + size)
        , Timestamp(timestamp)
    {}

    Chunk(std::size_t size, std::size_t timestamp)
        : Data(size, 0)
        , Timestamp(timestamp)
    {}

    Chunk(Chunk&& other)
        : Data(std::move(other.Data))
        , Timestamp(other.Timestamp)
    {}
};

bool operator<(const Chunk& a, const Chunk& b)
{
    return a.Timestamp < b.Timestamp;
}

} // namespace Pcm

#endif /* PCMCHUNK_H */
