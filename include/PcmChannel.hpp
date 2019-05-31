#ifndef PCMCHANNEL_H
#define PCMCHANNEL_H

#include <queue>
#include <Pcm.hpp>
#include <PcmChunk.hpp>

namespace Pcm {

struct Channel
{
    std::queue<Chunk> Queue;

    void Push(const uint8_t* data, size_t size, size_t timestamp);
};

} // namespace Pcm

#endif /* PCMCHANNEL_H */
