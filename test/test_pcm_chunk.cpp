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

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
