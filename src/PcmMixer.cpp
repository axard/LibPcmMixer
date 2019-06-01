#include <utility>
#include <algorithm>
#include <PcmMixer.hpp>
#include <PcmMix.hpp>

namespace Pcm {

Mixer::Mixer(ByteOrder byteOrder)
    : byteOrder_(byteOrder)
{}

Mixer::~Mixer()
{}

void Mixer::SetOnChunkReadyCallback(const std::function<OnChunkReadyCallback>& callback)
{
    callback_ = callback;
}

const std::function<Mixer::OnChunkReadyCallback>& Mixer::GetOnChunkReadyCallback() const
{
    return callback_;
}

void Mixer::PushChunk(const uint8_t* data, size_t size, size_t timestamp, int channelId)
{
    if (channelId < 0 || channelId >= CHANNEL_NUMBER) {
        auto msg = std::string{ "Argument 'channelId' of Mixer::PushChunk has invalid value: " }
                   + std::to_string(channelId);
        throw std::invalid_argument{ msg };
    }

    if (size % Pcm::SAMPLE_SIZE != 0) {
        auto msg = std::string{ "Argument 'size' of Mixer::PushChunk has invalid value: " }
                   + std::to_string(size);
        throw std::invalid_argument{ msg };
    }

    if (!callback_) {
        throw std::runtime_error{ "Mixer::OnChunkReadyCallback not defined" };
    }

    mutex_.lock();

    auto& channel = buffer_[ channelId ];
    channel.Push(data, size, timestamp);

    auto complete = [](const Channel& ch) {
        return !ch.Queue.empty() && ch.Queue.front().Complete();
    };

    std::queue<std::pair<Chunk, Chunk>> queue;

    while (complete(buffer_[ 0 ]) && complete(buffer_[ 1 ])) {
        queue.emplace(std::make_pair(std::move(buffer_[ 0 ].Queue.front()),
                                     std::move(buffer_[ 1 ].Queue.front())));

        buffer_[ 0 ].Queue.pop();
        buffer_[ 1 ].Queue.pop();
    }

    mutex_.unlock();

    while (!queue.empty()) {
        auto a = std::move(queue.front().first);
        auto b = std::move(queue.front().second);
        queue.pop();
        auto m = Mix(a, b, byteOrder_);
        callback_(m.Data.data(), m.Data.size(), m.Timestamp);
    }
}

void Mixer::Flush()
{
    if (!callback_) {
        throw std::runtime_error{ "Mixer::OnChunkReadyCallback not defined" };
    }

    std::lock_guard<std::mutex> lock(mutex_);

    auto empty = [](const Channel& ch) { return ch.Queue.empty(); };
    if (empty(buffer_[ 0 ]) && empty(buffer_[ 1 ])) {
        return;
    }

    alignChunks();

    while (!empty(buffer_[ 0 ]) && !empty(buffer_[ 1 ])) {
        auto a = std::move(buffer_[ 0 ].Queue.front());
        auto b = std::move(buffer_[ 1 ].Queue.front());

        buffer_[ 0 ].Queue.pop();
        buffer_[ 1 ].Queue.pop();

        auto m = Mix(a, b, byteOrder_);

        callback_(m.Data.data(), m.Data.size(), m.Timestamp);
    }
}

ByteOrder Mixer::byteOrder() const
{
    return byteOrder_;
}

void Mixer::setByteOrder(ByteOrder byteOrder)
{
    byteOrder_ = byteOrder;
}

void Mixer::alignChunks()
{
    // Выравнием кол-во чанков в очереди
    auto result = std::minmax(buffer_[ 0 ], buffer_[ 1 ], [](const Channel& a, const Channel& b) {
        return a.Queue.size() < b.Queue.size();
    });

    auto  diff      = result.second.Queue.size() - result.first.Queue.size();
    auto& queue     = const_cast<Channel&>(result.first).Queue;
    auto  timestamp = (queue.empty()) ? result.second.Queue.front().Timestamp
                                     : queue.front().Timestamp + CHUNK_SIZE / SAMPLE_SIZE;
    for (auto i = diff; i > 0; --i) {
        queue.emplace(CHUNK_SIZE, timestamp);
        timestamp += CHUNK_SIZE / SAMPLE_SIZE;
    }

    // Выравниваем первые в очереди чанки, т.к. они могут быть не полными
    alignChunks(buffer_[ 0 ].Queue.front(), buffer_[ 1 ].Queue.front());
}

void Mixer::alignChunks(Chunk& a, Chunk& b)
{
    if (a.Size() != b.Size()) {
        auto minmax = std::minmax(a, b);
        auto diff   = minmax.second.Size() - minmax.first.Size();

        auto& data = const_cast<Chunk&>(minmax.first).Data;
        for (auto i = diff; i > 0; --i) {
            data.push_back(0);
        }
    }
}

} // namespace Pcm
