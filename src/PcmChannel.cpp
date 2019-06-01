#include <stdexcept>
#include <PcmChannel.hpp>

namespace Pcm {

void Channel::Push(const uint8_t* data, size_t size, size_t timestamp)
{
    if (Queue.empty()) {
        Queue.emplace(0, 0);
    }

    auto expectedTimestamp = Queue.back().Timestamp + Queue.back().Data.size() / SAMPLE_SIZE;
    if (expectedTimestamp > timestamp) {
        throw std::invalid_argument{ "Timestamp mismatch" };
    }

    auto quietSize = SAMPLE_SIZE * (timestamp - expectedTimestamp);

    if (quietSize > 0) {
        auto space    = Queue.back().Space();
        auto oversize = quietSize > space;
        auto sz       = oversize ? space : quietSize;

        for (auto i = sz; i > 0; --i) {
            Queue.back().Data.push_back(0);
        }

        auto restQuietSize = quietSize - sz;
        auto tmpTimestamp  = Queue.back().Timestamp + CHUNK_SIZE / SAMPLE_SIZE;
        for (auto i = restQuietSize / CHUNK_SIZE; i > 0; --i) {
            Queue.emplace(CHUNK_SIZE, tmpTimestamp);
            tmpTimestamp += CHUNK_SIZE / SAMPLE_SIZE;
            restQuietSize -= CHUNK_SIZE;
        }
        if (restQuietSize > 0) {
            Queue.emplace(restQuietSize, tmpTimestamp);
        }
    }

    auto space    = Queue.back().Space();
    auto oversize = size > space;
    auto sz       = oversize ? space : size;

    for (auto i = sz; i > 0; --i) {
        Queue.back().Data.push_back(*data);
        data++;
    }

    auto restSize     = size - sz;
    auto tmpTimestamp = Queue.back().Timestamp + CHUNK_SIZE / SAMPLE_SIZE;
    for (auto i = restSize / CHUNK_SIZE; i > 0; --i) {
        Queue.emplace(data, CHUNK_SIZE, tmpTimestamp);
        tmpTimestamp += CHUNK_SIZE / SAMPLE_SIZE;
        restSize -= CHUNK_SIZE;
        data += CHUNK_SIZE;
    }
    if (restSize > 0) {
        Queue.emplace(data, restSize, tmpTimestamp);
    }
}

} // namespace Pcm
