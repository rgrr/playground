#include "TLife.h"
#include "gtest/gtest.h"
//#include "gtest/gtest-assertion-result.h"


class LifeTest : public testing::Test
{
protected:
	void SetUp() override {
		game = new TLife(1000);
	}

	void TearDown() override {
		delete game;
	}

	TLife *game;
};

namespace testing::internal {
bool operator==( const TLifeField & lh, const TLifeField & rh )
{
	return true;
}
}


testing::AssertionResult CmpHelperEQ(const char* lhs_expression,
                            const char* rhs_expression, const TLifeField& lhs,
                            const TLifeField& rhs) {
  if (lhs == rhs) {
    return testing::AssertionSuccess();
  }

  return testing::internal::CmpHelperEQFailure(lhs_expression, rhs_expression, lhs, rhs);
}



TEST_F(LifeTest, EmptyField) {
	TLifeField initialState = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
    };
    game->InitField(initialState);
    game->NextGeneration();
    EXPECT_EQ(game->GetField(), initialState);
//    EXPECT_EQ(initialState, game);
    EXPECT_TRUE(game->GetField() == initialState);
}


TEST_F(LifeTest, LifeBlock) {
	TLifeField initialState = {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
    };
    game->InitField(initialState);
    game->NextGeneration();
    EXPECT_EQ(game->GetField(), initialState);
}


TEST_F(LifeTest, Blinker) {
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
    game->InitField(initialState);
    game->NextGeneration();
    EXPECT_EQ(game->GetField(), expected1);
    game->NextGeneration();
    EXPECT_EQ(game->GetField(), initialState);
//    _sleep(20000);
}


TEST_F(LifeTest, Glider) {
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
    game->InitField(initialState);
    game->NextGeneration();
    EXPECT_EQ(game->GetField(), expected1);
    game->NextGeneration();
    EXPECT_EQ(game->GetField(), expected2);
    game->NextGeneration();
    EXPECT_EQ(game->GetField(), expected3);
    game->NextGeneration();
    EXPECT_EQ(game->GetField(), expected4);
}
