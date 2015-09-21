#ifndef INCLUDE_TESTS_H
#define INCLUDE_TESTS_H

#include <stdexcept>
#include <string>
#include <gtest/gtest.h>
#include "random.h"
#include "skip_list.h"

using namespace algic;

random<float>  SRand;

struct SkipListFixture : public ::testing::Test
{
    SkipListFixture()
        : mSList1(SRand)
        , mSList2(SRand)
    {
    }

    void fillRandomly(size_t count)
    {
        for (size_t i = 0; i < count; ++i)
        {
            int num = mRand();
            mSList1.insert(num);
            mSet1.insert(num);
            num = mRand();
            mSList2.insert(num);
            mSet2.insert(num);
        }
    }

    void fillRandomlyRange(size_t count)
    {
        std::list<int> listInt;
        for (size_t i = 0; i < count; ++i)
        {
            int const num = mRand();
            listInt.push_back(num);
        }
        mSList1.insert(std::cbegin(listInt), std::cend(listInt));
        mSet1.insert(std::cbegin(listInt), std::cend(listInt));
    }

    void clear()
    {
        mSList1.clear();
        mSList2.clear();
        mSet1.clear();
        mSet2.clear();
    }

    random<int>  mRand;
    skip_list<int, random<float>>  mSList1;
    skip_list<int, random<float>>  mSList2;
    std::set<int>  mSet1;
    std::set<int>  mSet2;
};

struct RandomSkipListFixture : public ::testing::Test
{
    RandomSkipListFixture()
        : mSList(SRand)
    {
        for (size_t i = 0; i < 1000; ++i)
        {
            int const num = mRand();
            mSList.insert(num);
            mSet.insert(num);
        }
    }

    random<int>  mRand;
    skip_list<int, random<float>>  mSList;
    std::set<int> mSet;
};

algic::skip_list<int, random<float>> createSListInt(float prob)
{
    return algic::skip_list<int, random<float>>(SRand, prob);
}

void compareSize(std::set<int> const& st, algic::skip_list<int, random<float>> const& sl)
{
    EXPECT_EQ(st.size(), sl.size());
}

void compare(std::set<int> const& st, algic::skip_list<int, random<float>> const& sl)
{
    auto sIt = std::cbegin(st);
    auto slIt = std::cbegin(sl);
    size_t counter = 0;
    bool result = true;
    for (; sIt != std::cend(st) && slIt != std::cend(sl);)
    {
        EXPECT_EQ(*sIt, *slIt);
        ++sIt;
        ++slIt;
        ++counter;
    }
    EXPECT_EQ(st.size(), counter);
}

TEST(SkipListTest, WrongArgument)
{
    EXPECT_THROW(createSListInt(-0.00001f), std::invalid_argument);
    EXPECT_THROW(createSListInt(1.00001f), std::invalid_argument);
}

TEST_F(RandomSkipListFixture, IterateOver)
{
    {
        ASSERT_EQ(mSet.size(), mSList.size());
        skip_list<int, random<float>>::iterator slIt = std::begin(mSList);
        auto sIt = std::begin(mSet);
        size_t counter = 0;
        for (; slIt != std::end(mSList) && sIt != std::end(mSet);)
        {
            EXPECT_EQ(*sIt, *slIt);
            ++sIt;
            ++slIt;
            ++counter;
        }
        ASSERT_EQ(mSet.size(), counter);
    }
    {
        ASSERT_EQ(mSet.size(), mSList.size());
        skip_list<int, random<float>>::const_iterator slIt = std::begin(mSList);
        auto sIt = std::begin(mSet);
        size_t counter = 0;
        for (; slIt != std::end(mSList) && sIt != std::end(mSet);)
        {
            EXPECT_EQ(*sIt, *slIt);
            ++sIt;
            ++slIt;
            ++counter;
        }
        ASSERT_EQ(mSet.size(), counter);
    }
    {
        ASSERT_EQ(mSet.size(), mSList.size());
        auto slIt = std::cbegin(mSList);
        auto sIt = std::cbegin(mSet);
        size_t counter = 0;
        for (; slIt != std::cend(mSList) && sIt != std::cend(mSet);)
        {
            EXPECT_EQ(*sIt, *slIt);
            ++sIt;
            ++slIt;
            ++counter;
        }
        ASSERT_EQ(mSet.size(), counter);
    }
}

TEST_F(SkipListFixture, Swap)
{
    fillRandomly(100);
    compareSize(mSet1, mSList1);
    compareSize(mSet2, mSList2);
    compare(mSet1, mSList1);
    compare(mSet2, mSList2);

    std::swap(mSList1, mSList2);
    compareSize(mSet1, mSList2);
    compareSize(mSet2, mSList1);
    compare(mSet1, mSList2);
    compare(mSet2, mSList1);
}

TEST_F(SkipListFixture, ClearEmpty)
{
    for (int i = 0; i < 100; ++i)
    {
        EXPECT_TRUE(mSList1.empty());
        EXPECT_TRUE(mSList2.empty());
        fillRandomly(100);
        EXPECT_FALSE(mSList1.empty());
        EXPECT_FALSE(mSList2.empty());
        mSList1.clear();
        mSList2.clear();
    }
    EXPECT_TRUE(mSList1.empty());
    EXPECT_TRUE(mSList2.empty());
}

TEST_F(SkipListFixture, FillRange)
{
    fillRandomlyRange(100);
    compareSize(mSet1, mSList1);
    compare(mSet1, mSList1);

    std::swap(mSList1, mSList2);
    compareSize(mSet1, mSList2);
    compareSize(mSet2, mSList1);
    compare(mSet1, mSList2);
    compare(mSet2, mSList1);

    mSList1.clear();
    mSet1.clear();
    auto initList = { 1, 2, 3, 4, 5, 12, -1 };
    mSList1.insert(initList);
    mSet1.insert(initList);
    compareSize(mSet1, mSList1);
    compare(mSet1, mSList1);
}

TEST_F(SkipListFixture, Erase)
{
    for (int i = 0; i < 100; ++i)
    {
        fillRandomly(100);
        {
            skip_list<int, random<float>>::iterator slIt = std::begin(mSList1);
            while (!mSList1.empty())
            {
                slIt = mSList1.erase(slIt);
            }
            EXPECT_TRUE(mSList1.empty());
            EXPECT_EQ(0, mSList1.size());
        }
        clear();
    }
}

TEST_F(SkipListFixture, EraseRandomly)
{
    for (int i = 0; i < 100; ++i)
    {
        fillRandomly(500);

        std::vector<int> vect;
        std::copy(std::cbegin(mSList1), std::cend(mSList1), std::back_inserter(vect));

        while (!vect.empty())
        {
            int const idx = mRand() % vect.size();
            mSList1.erase(vect[idx]);
            vect.erase((std::cbegin(vect) + idx));
            EXPECT_EQ(vect.size(), mSList1.size());
        }
    }
}

#endif
