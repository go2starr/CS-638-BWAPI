#include "gtest/gtest.h"
#include "gmock/gmock.h"


int sumTest(int x, int y)
{
	return x + y;
}

TEST(MyTestCase, MyTest_Sum1)
{
    EXPECT_EQ(2, sumTest(1, 1));
}

TEST(MyTestCase, MyTest_Sum2)
{
    EXPECT_EQ(4, sumTest(-2, 6));
}