#include <gtest/gtest.h>
#include <PcmMix.hpp>
#include <cstring>

#include <iostream>

TEST(test_pcm_mix_samples, with_both_silent)
{
    int16_t a = 0;
    int16_t b = 0;

    int16_t m = Pcm::Mix(a, b);

    ASSERT_EQ(m, 0);
}

TEST(test_pcm_mix_samples, with_both_loud_min)
{
    int16_t a = INT16_MIN;
    int16_t b = INT16_MIN;

    int16_t m = Pcm::Mix(a, b);

    ASSERT_EQ(m, INT16_MIN);
}

TEST(test_pcm_mix_samples, with_both_loud_max)
{
    int16_t a = INT16_MAX;
    int16_t b = INT16_MAX;

    int16_t m = Pcm::Mix(a, b);

    ASSERT_EQ(m, INT16_MAX);
}

TEST(test_pcm_mix_samples, with_opposite_loud_values)
{
    int16_t a = INT16_MAX;
    int16_t b = -a;

    int16_t m = Pcm::Mix(a, b);

    ASSERT_EQ(m, 0);
}

TEST(test_pcm_mix_samples, with_opposite_quiet_values)
{
    int16_t a = 100;
    int16_t b = -a;

    int16_t m = Pcm::Mix(a, b);

    ASSERT_EQ(m, 0);
}

TEST(test_pcm_mix_samples, with_one_silent_and_other_loud_max)
{
    int16_t a = INT16_MAX;
    int16_t b = 0;

    int16_t m = Pcm::Mix(a, b);

    ASSERT_EQ(m, a);
}

TEST(test_pcm_mix_samples, with_one_silent_and_other_loud_min)
{
    int16_t a = INT16_MIN;
    int16_t b = 0;

    int16_t m = Pcm::Mix(a, b);

    ASSERT_EQ(m, a);
}

TEST(test_pcm_mix_chunks, in_little_endian_and_min)
{
    const uint8_t data[] = { 0x00, 0x80, 0x00, 0x80 };

    auto a = Pcm::Chunk(data, sizeof(data), 0);
    auto b = Pcm::Chunk(data, sizeof(data), 0);

    auto m = Mix(a, b, Pcm::ByteOrder::LittleEndian);

    ASSERT_TRUE(memcmp(m.Data.data(), data, sizeof(data)) == 0);
}

TEST(test_pcm_mix_chunks, in_big_endian_and_min)
{
    const uint8_t data[] = { 0x80, 0x00, 0x80, 0x00 };

    auto a = Pcm::Chunk(data, sizeof(data), 0);
    auto b = Pcm::Chunk(data, sizeof(data), 0);

    auto m = Mix(a, b, Pcm::ByteOrder::BigEndian);

    ASSERT_TRUE(memcmp(m.Data.data(), data, sizeof(data)) == 0);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
