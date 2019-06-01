#ifndef PCMMIX_H
#define PCMMIX_H

#include <tuple>
#include <Pcm.hpp>
#include <PcmChunk.hpp>

namespace Pcm {

Chunk Mix(const Chunk& a, const Chunk& b, ByteOrder byteOrder);

int16_t Mix(int16_t a, int16_t b);

} // namespace Pcm

#endif /* PCMMIX_H */
