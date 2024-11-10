#include "TLife.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"


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
	TLife game(1000);

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
	TLife game(1000);

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
	TLife game(1000);

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
	TLife game(1000);

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
