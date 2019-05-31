#include <PcmChunk.hpp>

namespace Pcm {

Chunk::Chunk(const std::uint8_t* data, std::size_t size, std::size_t timestamp)
    : Timestamp(timestamp)
{
    Data.reserve(CHUNK_SIZE);
    std::copy(data, data + size, std::back_inserter(Data));
}

Chunk::Chunk(std::size_t size, std::size_t timestamp)
    : Timestamp(timestamp)
{
    Data.reserve(CHUNK_SIZE);
    while (size > 0) {
        Data.push_back(0);
        size--;
    }
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

std::size_t Chunk::Size() const
{
    return Data.size();
}

bool operator<(const Chunk& a, const Chunk& b)
{
    return a.Size() < b.Size();
}

} // namespace Pcm
