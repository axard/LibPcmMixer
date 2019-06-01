#include <gtest/gtest.h>
#include <PcmMixer.hpp>

constexpr auto DEFAULT_VALUE = 1;

class PcmMixerFixture : public ::testing::Test
{
public:
    Pcm::Mixer mixer;

    PcmMixerFixture()
        : mixer(Pcm::ByteOrder::LittleEndian)
    {}

    ~PcmMixerFixture()
    {}

    void callback(const uint8_t* data, size_t size, size_t timestamp)
    {}
};

void callback(const uint8_t* data, size_t size, size_t timestamp)
{}

TEST_F(PcmMixerFixture, test_set_on_chunk_ready_callback__with_function)
{
    mixer.SetOnChunkReadyCallback(::callback);

    auto target = mixer.GetOnChunkReadyCallback()
                      .target<void (*)(const uint8_t*, size_t, size_t)>();

    ASSERT_NE(target, nullptr);
    ASSERT_EQ(*target, ::callback);
}

TEST_F(PcmMixerFixture, test_set_on_chunk_ready_callback__with_method)
{
    using namespace std::placeholders;

    mixer.SetOnChunkReadyCallback(std::bind(&PcmMixerFixture::callback, this, _1, _2, _3));
    auto callback = mixer.GetOnChunkReadyCallback();

    ASSERT_TRUE(callback);
}

TEST(test_pcm_mixer, push_two_full_chunks_with_eq_timestamp)
{
    Pcm::Mixer mixer(Pcm::ByteOrder::LittleEndian);

    uint8_t data[ Pcm::CHUNK_SIZE ];
    size_t  size      = sizeof(data);
    size_t  timestamp = 0;
    size_t  chunks    = 0;

    std::fill(data, data + size, DEFAULT_VALUE);

    mixer.SetOnChunkReadyCallback([&chunks](const uint8_t*, size_t, size_t) { chunks++; });

    mixer.PushChunk(data, size, timestamp, 0);
    mixer.PushChunk(data, size, timestamp, 1);
    mixer.Flush();

    ASSERT_EQ(chunks, 1);
}

TEST(test_pcm_mixer, push_two_full_chunks_with_ne_timestamp)
{
    Pcm::Mixer mixer(Pcm::ByteOrder::LittleEndian);

    uint8_t data[ Pcm::CHUNK_SIZE ];
    size_t  size       = sizeof(data);
    size_t  timestamp0 = 0;
    size_t  timestamp1 = 100;
    size_t  chunks     = 0;

    std::fill(data, data + size, DEFAULT_VALUE);

    mixer.SetOnChunkReadyCallback([&chunks](const uint8_t*, size_t, size_t) { chunks++; });

    mixer.PushChunk(data, size, timestamp0, 0);
    mixer.PushChunk(data, size, timestamp1, 1);
    mixer.Flush();

    ASSERT_EQ(chunks, 2);
}

TEST(test_pcm_mixer, push_chunks_and_test_callback_sequense)
{
    Pcm::Mixer mixer(Pcm::ByteOrder::LittleEndian);

    uint8_t data[ Pcm::CHUNK_SIZE ];
    size_t  size      = sizeof(data);
    size_t  timestamp = 0;
    size_t  chunks    = 0;

    std::fill(data, data + size, DEFAULT_VALUE);

    mixer.SetOnChunkReadyCallback([&chunks](const uint8_t*, size_t, size_t) { chunks++; });

    mixer.PushChunk(data, size, timestamp, 0);
    ASSERT_EQ(chunks, 0);

    mixer.PushChunk(data, size, timestamp + Pcm::CHUNK_SIZE / Pcm::SAMPLE_SIZE, 0);
    ASSERT_EQ(chunks, 0);

    mixer.PushChunk(data, size, timestamp + 100, 1);
    ASSERT_EQ(chunks, 1);

    mixer.Flush();
    ASSERT_EQ(chunks, 2);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
