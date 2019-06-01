#ifndef PCMMIXER_H
#define PCMMIXER_H

#include <cstdint>
#include <cstddef>
#include <functional>
#include <mutex>

#include <Pcm.hpp>
#include <PcmChunk.hpp>
#include <PcmChannel.hpp>

namespace Pcm {

class Mixer
{
public:
    // Оставил параметр size, потому что возможна ситуация, где стримы уже закончились,
    // но суммарно их не хватает до 1024 семплов.
    using OnChunkReadyCallback = void(const uint8_t* data, size_t size, size_t timestamp);

    explicit Mixer(ByteOrder byteOrder);
    ~Mixer();

    void SetOnChunkReadyCallback(const std::function<OnChunkReadyCallback>& callback);

    const std::function<OnChunkReadyCallback>& GetOnChunkReadyCallback() const;

    void PushChunk(const uint8_t* data, size_t size, size_t timestamp, int channelId);

    void Flush();

    ByteOrder byteOrder() const;

    void setByteOrder(ByteOrder byteOrder);

private:
    std::function<OnChunkReadyCallback> callback_;

    std::mutex mutex_;

    ByteOrder byteOrder_;
    Channel   buffer_[ CHANNEL_NUMBER ];

    void alignChunks();
    void alignChunks(Chunk& a, Chunk& b);
};

} // namespace Pcm

#endif /* PCMMIXER_H */
