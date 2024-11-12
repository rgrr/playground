#include "gtest/gtest.h"
#include "gmock/gmock.h"


#define LIFE_TYPE  2

#if (LIFE_TYPE == 0)
    #include "TLife.h"
    #define TLIFE TLife
#elif (LIFE_TYPE == 1)
    #include "TLifeFaster.h"
    #define TLIFE TLifeFaster
#elif (LIFE_TYPE == 2)
    #include "TLifeFasterMulti.h"
    #define TLIFE TLifeFasterMulti
#else
    #error "Wrong LIFE_TYPE"
#endif


class LifeTest : public testing::Test
{
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};


MATCHER_P(MatchesFieldOf, compare_field, "")
{
    uint32_t row_cnt = 0;
    bool matches = true;

    for (const auto & row : compare_field)
    {
        uint32_t col_cnt = 0;
        for (const auto & val : row)
        {
            if (val != arg[row_cnt][col_cnt])
            {
                return false;
            }
            ++col_cnt;
        }
        ++row_cnt;
    }
    // actually it should be tested that the remainder of "arg" is empty

    return matches;
}


TEST_F(LifeTest, EmptyField)
{
    TLifeField initialState = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
    };
    TLIFE game(1000);

    game.InitField(initialState);
    game.NextGeneration();
    EXPECT_THAT(game.GetField(), MatchesFieldOf(initialState));
}


TEST_F(LifeTest, LifeBlock)
{
    TLifeField initialState = {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
    };
    TLIFE game(1000);

    game.InitField(initialState);
    game.NextGeneration();
    EXPECT_THAT(game.GetField(), MatchesFieldOf(initialState));
}


TEST_F(LifeTest, Blinker)
{
    TLifeField initialState = {
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}
    };
    TLifeField expected1 = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
    };
    TLIFE game(1000);

    game.InitField(initialState);
    game.NextGeneration();
    EXPECT_THAT(game.GetField(), MatchesFieldOf(expected1));
    game.NextGeneration();
    EXPECT_THAT(game.GetField(), MatchesFieldOf(initialState));
}


TEST_F(LifeTest, Glider)
{
    TLifeField initialState = {
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {0, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };
    TLifeField expected1 = {
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 1, 0, 0, 0},
        {0, 0, 1, 1, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };
    TLifeField expected2 = {
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {0, 1, 0, 1, 0, 0, 0},
        {0, 0, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };
    TLifeField expected3 = {
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 0, 0},
        {0, 0, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };
    // like initialState, but the glider has moved
    TLifeField expected4 = {
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };
    TLIFE game(1000);

    game.InitField(initialState);
    game.NextGeneration();
    EXPECT_THAT(game.GetField(), MatchesFieldOf(expected1));
    game.NextGeneration();
    EXPECT_THAT(game.GetField(), MatchesFieldOf(expected2));
    game.NextGeneration();
    EXPECT_THAT(game.GetField(), MatchesFieldOf(expected3));
    game.NextGeneration();
    EXPECT_THAT(game.GetField(), MatchesFieldOf(expected4));
}
