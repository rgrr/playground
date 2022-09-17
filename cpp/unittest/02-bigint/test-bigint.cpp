#include <gtest/gtest.h>
#include "bigint.h"



TEST(BigInt, Constructor)
{
	EXPECT_EQ(BigInt(12345), 12345);
	EXPECT_EQ(BigInt("12345"), 12345);
}



TEST(BigInt, Equal)
{
	BigInt first("12345");
	BigInt second(12345);

	EXPECT_EQ(first, second);

	first++;
	EXPECT_EQ(first, 12346);

	++first;
	EXPECT_EQ(first, 12347);

	first -= 2;
	EXPECT_EQ(first, 12345);

	first -= 12344;
	EXPECT_EQ(first, 1);

	--first;
	EXPECT_EQ(first, 0);

	// now we expect an assertion
	EXPECT_ANY_THROW( --first);
}



TEST(BigInt, NotEqual)
{
	BigInt fourth("10000");

	EXPECT_NE(fourth, BigInt(10000) + 1);
	EXPECT_NE(fourth, BigInt(10000) - 1);
	EXPECT_NE(fourth, 2 * BigInt(10000));
	EXPECT_NE(fourth, BigInt(10000) / 2);
}



TEST(BigInt, SmallerLarger)
{
	BigInt third("1000");
	BigInt fourth("10000");
	BigInt fifth("100000");

	EXPECT_LT(third, fourth);
	EXPECT_GT(fifth, fourth);

	EXPECT_LT(third - 1, third);
	EXPECT_GT(third + 1, third);
	EXPECT_LT(third / 2, third);
	EXPECT_GT(2 * third, third);
}



TEST(BigInt, Multiply)
{
	BigInt first("12345");
	BigInt second(12345);
	BigInt third("10000");
	BigInt product;

	product = second * third;
	EXPECT_EQ(product, 123450000);
}



TEST(BigInt, Fibonacci)
{
	EXPECT_EQ(BigInt::NthFibonacci(0), BigInt("0"));
	EXPECT_EQ(BigInt::NthFibonacci(29), BigInt("514229"));
	EXPECT_EQ(BigInt::NthFibonacci(84), BigInt("160500643816367088"));
	EXPECT_EQ(BigInt::NthFibonacci(100), BigInt("354224848179261915075"));
}



TEST(BigInt, Catalan)
{
	EXPECT_EQ(BigInt::NthCatalan(0), BigInt("1"));
	EXPECT_EQ(BigInt::NthCatalan(29), BigInt("1002242216651368"));
	EXPECT_EQ(BigInt::NthCatalan(84), BigInt("270557451039395118028642463289168566420671280440"));
	EXPECT_EQ(BigInt::NthCatalan(100), BigInt("896519947090131496687170070074100632420837521538745909320"));
}



TEST(BigInt, Factorial)
{
	EXPECT_EQ(BigInt::Factorial(0), BigInt("1"));
	EXPECT_EQ(BigInt::Factorial(20), BigInt(2432902008176640000));
	EXPECT_EQ(BigInt::Factorial(29), BigInt("8841761993739701954543616000000"));
	EXPECT_EQ(BigInt::Factorial(84), BigInt("3314240134565353266999387579130131288000666286242049487118846032383059131291716864129885722968716753156177920000000000000000000"));
	EXPECT_EQ(BigInt::Factorial(100), BigInt("93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000"));
}



TEST(BigInt, Divide)
{
	BigInt first("12345");
	BigInt second(12345);
	BigInt third("10000");
	BigInt product;

	product = second * third;
	EXPECT_EQ(product, 123450000);

	EXPECT_EQ(product / 10000, 12345);

	EXPECT_EQ(BigInt::Factorial(100) / (BigInt(100) * BigInt(99)), BigInt::Factorial(98));
}



TEST(BigInt, MathOps)
{
	// biggest uint64_t =                     18,446,744,073,709,551,616
	const BigInt ndx_start = BigInt("710'000'000'045'800'000'000'000'123");
	const BigInt ndx_end   = ndx_start + 527;
	BigInt ndx;
	BigInt n_sqr;
	BigInt n_sqrt;

	for (ndx = ndx_start;  ndx < ndx_end;  ++ndx)
	{
		n_sqr = ndx * ndx;
		EXPECT_EQ(n_sqr / ndx, ndx);
		EXPECT_EQ(n_sqr % ndx, 0);
		EXPECT_EQ((n_sqr + 1) % ndx, 1);
		EXPECT_EQ((n_sqr - 1) % ndx, ndx - 1);
		EXPECT_EQ(sqrt(n_sqr), ndx);
	}
}
