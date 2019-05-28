#include <gtest/gtest.h>
#include <PcmMixer.hpp>

class PcmMixerFixture : public ::testing::Test
{
public:
    PcmMixer mixer;

    PcmMixerFixture()
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

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
