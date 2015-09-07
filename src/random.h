#include <ctime>
#include <random>


template <class RealNumber>
struct random
{
    random(RealNumber lower = RealNumber(0), RealNumber upper = RealNumber(1))
        : mRandGen(static_cast<std::mt19937::result_type>(std::time(nullptr)))
        , mUniDist(lower, upper)
    {
    }

    RealNumber operator()()
    {
        return mUniDist(mRandGen);
    }

    void seed(std::mt19937::result_type val)
    {
        mRandGen.seed(val);
    }

private:
    std::mt19937  mRandGen;
    std::uniform_real_distribution<RealNumber>  mUniDist;
};
