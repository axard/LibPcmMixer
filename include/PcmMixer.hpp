#ifndef PCMMIXER_H
#define PCMMIXER_H

#include <cstdint>
#include <cstddef>
#include <functional>

namespace Pcm {

class Mixer
{
public:
    // Оставил параметр size, потому что возможна ситуация, где стримы уже закончились,
    // но суммарно их не хватает до 1024 семплов.
    using OnChunkReadyCallback = void(const uint8_t* data, size_t size, size_t timestamp);

    explicit Mixer();
    ~Mixer();

    void SetOnChunkReadyCallback(const std::function<OnChunkReadyCallback>& callback);

    const std::function<OnChunkReadyCallback>& GetOnChunkReadyCallback() const;

private:
    std::function<OnChunkReadyCallback> callback_;
};

} // namespace Pcm

#endif /* PCMMIXER_H */
