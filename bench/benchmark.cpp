#include <chrono>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include "random.h"
#include "skip_list.h"

size_t const StrMaxLen = 30;

std::vector<std::string>  sVect;
random<int> sRand;
using std::chrono::high_resolution_clock;


void fillRandom(size_t count);
void benchFill();
void benchFillRange();


/*********** MAIN ***********/
int main(int argc, char* argv[])
{
    benchFill();
    benchFillRange();


    getchar();
    return 0;
}


void fillRandom(size_t count)
{
    sVect.clear();
    for (size_t i = 0; i < count; ++i)
    {
        size_t const strLen = sRand() % StrMaxLen;
        sVect.push_back(std::string());

        std::string& str = sVect.back();
        str.resize(strLen, 0);

        for (size_t k = 0; k < str.size(); ++k)
        {
            str[k] = sRand() % 128;
        }
    }
}

void benchFill()
{
    for (int c = 1; c <= 100000; c *= 10)
    {
        std::cout << std::endl << "Number of elements: " << c << std::endl;
        fillRandom(c);
        {
            std::set<std::string>  set;
            auto tpStart = high_resolution_clock::now();
            for (auto const& v : sVect)
            {
                set.insert(v);
            }
            auto tpFinish = high_resolution_clock::now();
            std::chrono::duration<double> diff = tpFinish - tpStart;
            std::cout << "  Set fill in " << diff.count() << std::endl;
        }

        {
            random<float> floatRand;
            algic::skip_list<std::string, random<float>>  sl(floatRand);
            auto tpStart = high_resolution_clock::now();
            for (auto const& v : sVect)
            {
                sl.insert(v);
            }
            auto tpFinish = high_resolution_clock::now();
            std::chrono::duration<double> diff = tpFinish - tpStart;
            std::cout << "  SkipList fill in " << diff.count() << std::endl;
        }
    }
}

void benchFillRange()
{
    for (int c = 1; c <= 100000; c *= 10)
    {
        std::cout << std::endl << "Number of elements: " << c << std::endl;
        fillRandom(c);
        {
            std::set<std::string>  set;
            auto tpStart = high_resolution_clock::now();
            set.insert(std::cbegin(sVect), std::cend(sVect));
            auto tpFinish = high_resolution_clock::now();
            std::chrono::duration<double> diff = tpFinish - tpStart;
            std::cout << "  Set fill in " << diff.count() << std::endl;
        }

        {
            random<float> floatRand;
            algic::skip_list<std::string, random<float>>  sl(floatRand);
            auto tpStart = high_resolution_clock::now();
            sl.insert(std::cbegin(sVect), std::cend(sVect));
            auto tpFinish = high_resolution_clock::now();
            std::chrono::duration<double> diff = tpFinish - tpStart;
            std::cout << "  SkipList fill in " << diff.count() << std::endl;
        }
    }
}
