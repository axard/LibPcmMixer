#ifndef PCMCHUNK_H
#define PCMCHUNK_H

#include <vector>
#include <cstdint>
#include <algorithm>

#include <Pcm.hpp>

namespace Pcm {

struct Chunk
{
    std::vector<std::uint8_t> Data;
    std::size_t               Timestamp;

    Chunk(const std::uint8_t* data, std::size_t size, std::size_t timestamp);

    Chunk(Chunk&& other) noexcept;

    bool Complete() const;

    std::size_t Space() const;
};

bool operator<(const Chunk& a, const Chunk& b);

} // namespace Pcm

#endif /* PCMCHUNK_H */
