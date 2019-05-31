#include <gtest/gtest.h>
#include <PcmChannel.hpp>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <algorithm>

constexpr auto DEFAULT_VALUE = 1;
constexpr auto DEFAULT_SIZE  = 14;

TEST(test_pcm_channel, push_to_empty_channel_lt_chunk_size_and_zero_timestamp)
{
    uint8_t data[ DEFAULT_SIZE ];
    size_t  size      = sizeof(data);
    size_t  timestamp = 0;

    std::fill(data, data + size, DEFAULT_VALUE);

    Pcm::Channel channel;
    channel.Push(data, size, timestamp);

    auto& chunk = channel.Queue.front();

    ASSERT_EQ(chunk.Data.size(), size);
    ASSERT_EQ(chunk.Timestamp, timestamp);
    ASSERT_TRUE(memcmp(chunk.Data.data(), data, size) == 0);
}

TEST(test_pcm_channel, push_to_empty_channel_lt_chunk_size_and_nonzero_timestamp_lt_chunk_size)
{
    uint8_t data[ DEFAULT_SIZE ];
    size_t  size      = sizeof(data);
    size_t  timestamp = DEFAULT_SIZE;

    uint8_t quiet[ DEFAULT_SIZE * Pcm::SAMPLE_SIZE ];
    size_t  quietSize = sizeof(quiet);

    std::fill(data, data + size, DEFAULT_VALUE);
    std::fill(quiet, quiet + quietSize, 0);

    Pcm::Channel channel;
    channel.Push(data, size, timestamp);

    auto& chunk = channel.Queue.front();

    ASSERT_EQ(chunk.Data.size(), size + timestamp * Pcm::SAMPLE_SIZE);
    ASSERT_EQ(chunk.Timestamp, 0);
    ASSERT_TRUE(memcmp(chunk.Data.data(), quiet, quietSize) == 0);
    ASSERT_TRUE(memcmp(chunk.Data.data() + timestamp * Pcm::SAMPLE_SIZE, data, size) == 0);
}

TEST(test_pcm_channel, push_to_empty_channel_lt_chunk_size_and_nonzero_timestamp_gt_chunk_size)
{
    uint8_t data[ DEFAULT_SIZE ];
    size_t  size      = sizeof(data);
    size_t  timestamp = Pcm::CHUNK_SIZE / Pcm::SAMPLE_SIZE + DEFAULT_SIZE;

    std::fill(data, data + size, DEFAULT_VALUE);

    Pcm::Channel channel;
    channel.Push(data, size, timestamp);

    auto isZero = [](uint8_t val) { return val == 0; };

    auto& frontChunk = channel.Queue.front();
    ASSERT_EQ(frontChunk.Data.size(), Pcm::CHUNK_SIZE);
    ASSERT_EQ(frontChunk.Timestamp, 0);
    ASSERT_TRUE(std::all_of(frontChunk.Data.begin(), frontChunk.Data.end(), isZero));

    auto& backChunk = channel.Queue.back();
    ASSERT_EQ(backChunk.Data.size(), size + DEFAULT_SIZE * Pcm::SAMPLE_SIZE);
    ASSERT_EQ(backChunk.Timestamp, Pcm::CHUNK_SIZE / Pcm::SAMPLE_SIZE);
    ASSERT_TRUE(std::all_of(backChunk.Data.begin(),
                            backChunk.Data.begin() + DEFAULT_SIZE * Pcm::SAMPLE_SIZE, isZero));
    ASSERT_TRUE(memcmp(backChunk.Data.data() + DEFAULT_SIZE * Pcm::SAMPLE_SIZE, data, size) == 0);
}

TEST(test_pcm_channel, push_to_empty_channel_gt_chunk_size_and_zero_timestamp)
{
    uint8_t data[ Pcm::CHUNK_SIZE + DEFAULT_SIZE ];
    size_t  size      = sizeof(data);
    size_t  timestamp = 0;

    std::fill(data, data + size, DEFAULT_VALUE);

    Pcm::Channel channel;
    channel.Push(data, size, timestamp);

    auto& frontChunk = channel.Queue.front();
    ASSERT_EQ(frontChunk.Data.size(), Pcm::CHUNK_SIZE);
    ASSERT_EQ(frontChunk.Timestamp, timestamp);
    ASSERT_TRUE(memcmp(frontChunk.Data.data(), data, Pcm::CHUNK_SIZE) == 0);

    auto& backChunk = channel.Queue.back();
    ASSERT_EQ(backChunk.Data.size(), DEFAULT_SIZE);
    ASSERT_EQ(backChunk.Timestamp, Pcm::CHUNK_SIZE / Pcm::SAMPLE_SIZE);
    ASSERT_TRUE(memcmp(backChunk.Data.data(), data + Pcm::CHUNK_SIZE, DEFAULT_SIZE) == 0);
}

TEST(test_pcm_channel, push_to_filled_channel_lt_chunk_size_and_without_missing)
{
    Pcm::Channel channel;

    uint8_t data1[ DEFAULT_SIZE ];
    size_t  size1      = sizeof(data1);
    size_t  timestamp1 = 0;
    std::fill(data1, data1 + size1, DEFAULT_VALUE);

    channel.Push(data1, size1, timestamp1);

    uint8_t data2[ Pcm::CHUNK_SIZE - DEFAULT_SIZE ];
    size_t  size2      = sizeof(data2);
    size_t  timestamp2 = DEFAULT_SIZE / Pcm::SAMPLE_SIZE;
    std::fill(data2, data2 + size2, DEFAULT_VALUE);

    channel.Push(data2, size2, timestamp2);

    auto isValue = [](uint8_t val) { return val == DEFAULT_VALUE; };

    auto& chunk = channel.Queue.front();
    ASSERT_EQ(channel.Queue.size(), 1);
    ASSERT_EQ(chunk.Data.size(), Pcm::CHUNK_SIZE);
    ASSERT_EQ(chunk.Timestamp, timestamp1);
    ASSERT_TRUE(std::all_of(chunk.Data.begin(), chunk.Data.end(), isValue));
}

TEST(test_pcm_channel, push_to_filled_channel_lt_chunk_size_and_with_missing)
{
    Pcm::Channel channel;

    uint8_t data1[ DEFAULT_SIZE ];
    size_t  size1      = sizeof(data1);
    size_t  timestamp1 = 0;
    std::fill(data1, data1 + size1, DEFAULT_VALUE);

    channel.Push(data1, size1, timestamp1);

    uint8_t data2[ Pcm::CHUNK_SIZE - DEFAULT_SIZE ];
    size_t  size2      = sizeof(data2);
    size_t  timestamp2 = Pcm::CHUNK_SIZE / Pcm::SAMPLE_SIZE;
    std::fill(data2, data2 + size2, DEFAULT_VALUE);

    channel.Push(data2, size2, timestamp2);

    auto isValue = [](uint8_t val) { return val == DEFAULT_VALUE; };
    auto isZero  = [](uint8_t val) { return val == 0; };

    auto& frontChunk = channel.Queue.front();
    ASSERT_EQ(channel.Queue.size(), 2);
    ASSERT_EQ(frontChunk.Data.size(), Pcm::CHUNK_SIZE);
    ASSERT_EQ(frontChunk.Timestamp, timestamp1);
    ASSERT_TRUE(std::all_of(frontChunk.Data.begin(), frontChunk.Data.begin() + size1, isValue));
    ASSERT_TRUE(std::all_of(frontChunk.Data.begin() + size1, frontChunk.Data.end(), isZero));

    auto& backChunk = channel.Queue.back();
    ASSERT_EQ(backChunk.Data.size(), Pcm::CHUNK_SIZE - DEFAULT_SIZE);
    ASSERT_EQ(backChunk.Timestamp, timestamp2);
    ASSERT_TRUE(std::all_of(backChunk.Data.begin(), backChunk.Data.begin() + size2, isValue));
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
