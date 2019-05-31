#include <gtest/gtest.h>
#include <PcmChunk.hpp>

TEST(PcmChunk, test_pcm_chunk_operator_less)
{
    uint8_t aData[ 9 ];
    uint8_t bData[ 10 ];

    Pcm::Chunk a{ aData, sizeof(aData), 10 };
    Pcm::Chunk b{ bData, sizeof(bData), 15 };

    ASSERT_TRUE(a < b);
    ASSERT_FALSE(b < a);
}

TEST(PcmChunk, test_pcm_chunk_incomplete)
{
    uint8_t aData[ 9 ];

    Pcm::Chunk a{ aData, sizeof(aData), 10 };

    ASSERT_FALSE(a.Complete());
}

TEST(PcmChunk, test_pcm_chunk_complete)
{
    uint8_t aData[ Pcm::CHUNK_SIZE ];

    Pcm::Chunk a{ aData, sizeof(aData), 10 };

    ASSERT_TRUE(a.Complete());
}

TEST(PcmChunk, test_pcm_chunk_space)
{
    uint8_t aData[ 1 ];

    Pcm::Chunk a{ aData, sizeof(aData), 0 };

    ASSERT_EQ(a.Space(), Pcm::CHUNK_SIZE - 1);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
