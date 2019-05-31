#include <PcmChunk.hpp>

namespace Pcm {

Chunk::Chunk(const std::uint8_t* data, std::size_t size, std::size_t timestamp)
    : Timestamp(timestamp)
{
    Data.reserve(CHUNK_SIZE);
    std::copy(data, data + size, std::back_inserter(Data));
}

Chunk::Chunk(Chunk&& other) noexcept
    : Data(std::move(other.Data))
    , Timestamp(other.Timestamp)
{}

bool Chunk::Complete() const
{
    return Data.size() == Data.capacity();
}

std::size_t Chunk::Space() const
{
    return Data.capacity() - Data.size();
}

bool operator<(const Chunk& a, const Chunk& b)
{
    return a.Timestamp < b.Timestamp;
}

} // namespace Pcm
