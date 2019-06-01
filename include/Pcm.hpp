#ifndef PCM_H
#define PCM_H

namespace Pcm {

constexpr auto SAMPLE_SIZE    = 2;
constexpr auto CHANNEL_NUMBER = 2;
constexpr auto CHUNK_SIZE     = 2048;

enum class ByteOrder
{
    BigEndian,
    LittleEndian
};

} // namespace Pcm

#endif /* PCM_H */
