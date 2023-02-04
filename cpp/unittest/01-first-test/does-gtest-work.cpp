#include <gtest/gtest.h>



// Limit 18,446,744,073,709,551,616 ~ 2e19
uint64_t Fac(uint64_t n)
{
	if (n < 0)
	{
		return 0xffff'ffff'ffff'ffff;
	}
	else if (n > 20)
	{
		return 0xffff'ffff'ffff'ffff;
	}
	else if (n == 0)
	{
		return 1;
	}
	else if (n == 1)
	{
		return 1;
	}
	return n * Fac(n-1);
}   // Fac



// Tests factorial of 0.
TEST(FactorialTest, HandlesZeroInput)
{
	EXPECT_EQ(Fac(0), 1);
}



TEST(FactorialTest, HandlesPositiveInput)
{
	EXPECT_EQ(Fac(1), 1);
	EXPECT_EQ(Fac(2), 2);
	EXPECT_EQ(Fac(3), 6);
	EXPECT_EQ(Fac(8), 40320);
	EXPECT_EQ(Fac(15), 1307674368000);
	EXPECT_EQ(Fac(18), 6402373705728000);
	EXPECT_EQ(Fac(19), 121645100408832000);
	EXPECT_EQ(Fac(20), 2432902008176640000);
	EXPECT_EQ(Fac(21), 0xffff'ffff'ffff'ffff);      // overflow
	EXPECT_EQ(Fac(22), 0xffff'ffff'ffff'ffff);      // overflow
	EXPECT_EQ(Fac(-1), 0xffff'ffff'ffff'ffff);      // more overflow
}



TEST(MachineTest, TypeSizes)
{
	unsigned int u = 0;
	unsigned long ul = 0;
	void *p = nullptr;

	EXPECT_EQ(sizeof(u), 4);

#ifdef _LP64
	EXPECT_EQ(sizeof(ul), 8);
	EXPECT_EQ(sizeof(p), 8);
#else
	EXPECT_EQ(sizeof(ul), 4);
	EXPECT_EQ(sizeof(p), 4);
#endif
}
