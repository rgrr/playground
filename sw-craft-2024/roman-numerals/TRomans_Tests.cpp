#include "gtest/gtest.h"
#include "TRomans.h"



class RomansTest : public testing::Test
{
protected:
	TRomans c;

	RomansTest() {};
};


TEST_F(RomansTest, Convert1)
{
	EXPECT_EQ(c.ConvertToRomanNum(1), "I");
}


TEST_F(RomansTest, Convert2)
{
	EXPECT_EQ(c.ConvertToRomanNum(2), "II");
}


TEST_F(RomansTest, Convert3)
{
	EXPECT_EQ(c.ConvertToRomanNum(3), "III");
}


TEST_F(RomansTest, Convert4)
{
	EXPECT_EQ(c.ConvertToRomanNum(4), "IV");
}


TEST_F(RomansTest, Convert9)
{
	EXPECT_EQ(c.ConvertToRomanNum(9), "IX");
}


TEST_F(RomansTest, Convert23)
{
	EXPECT_EQ(c.ConvertToRomanNum(23), "XXIII");
}


TEST_F(RomansTest, Convert58)
{
	EXPECT_EQ(c.ConvertToRomanNum(58), "LVIII");
}


TEST_F(RomansTest, Convert1994)
{
	EXPECT_EQ(c.ConvertToRomanNum(1994), "MCMXCIV");
}


TEST_F(RomansTest, Convert3549)
{
	EXPECT_EQ(c.ConvertToRomanNum(3549), "MMMDXLIX");
}
