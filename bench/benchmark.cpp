#include <chrono>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include "random.h"
#include "skip_list.h"

size_t const StrMaxLen = 30;

std::vector<std::string>  sVect;
std::vector<int>  sVectInts;
random<int> sRand;
using std::chrono::high_resolution_clock;


void fillRandomStr(size_t count);
void fillRandomInt(size_t count);
void benchFill();
void benchFillRange();
void benchFillAsc();
void benchFillDesc();


/*********** MAIN ***********/
int main(int argc, char* argv[])
{
    //benchFill();
    //benchFillRange();
    benchFillAsc();
    benchFillDesc();

    sVect.clear();
    sVectInts.clear();
    std::cout << "\n\nFINISHED" << std::endl;
    getchar();
    return 0;
}


void fillRandomStr(size_t count)
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

void fillRandomInt(size_t count)
{
    sVectInts.clear();
    for (int i = 0; i < count; ++i)
    {
        sVectInts.push_back(i);
    }
}

void benchFill()
{
    std::cout << "\n\nBENCH FILL" << std::endl;
    for (int c = 1; c <= 100000; c *= 10)
    {
        std::cout << std::endl << "Number of elements: " << c << std::endl;
        fillRandomStr(c);
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

            tpStart = high_resolution_clock::now();
            for (auto const& v : sVect)
            {
                set.erase(v);
            }
            tpFinish = high_resolution_clock::now();
            diff = tpFinish - tpStart;
            std::cout << "  Set erase in " << diff.count() << std::endl;
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

            tpStart = high_resolution_clock::now();
            for (auto const& v : sVect)
            {
                sl.erase(v);
            }
            tpFinish = high_resolution_clock::now();
            diff = tpFinish - tpStart;
            std::cout << "  SkipList erase in " << diff.count() << std::endl;
        }
    }
}

void benchFillRange()
{
    std::cout << "\n\nBENCH FILL RANGE" << std::endl;
    for (int c = 1; c <= 100000; c *= 10)
    {
        std::cout << std::endl << "Number of elements: " << c << std::endl;
        fillRandomStr(c);
        {
            std::set<std::string>  set;
            auto tpStart = high_resolution_clock::now();
            set.insert(std::cbegin(sVect), std::cend(sVect));
            auto tpFinish = high_resolution_clock::now();
            std::chrono::duration<double> diff = tpFinish - tpStart;
            std::cout << "  Set fill in " << diff.count() << std::endl;

            tpStart = high_resolution_clock::now();
            for (auto const& v : sVect)
            {
                set.erase(v);
            }
            tpFinish = high_resolution_clock::now();
            diff = tpFinish - tpStart;
            std::cout << "  Set erase in " << diff.count() << std::endl;
        }

        {
            random<float> floatRand;
            algic::skip_list<std::string, random<float>>  sl(floatRand);
            auto tpStart = high_resolution_clock::now();
            sl.insert(std::cbegin(sVect), std::cend(sVect));
            auto tpFinish = high_resolution_clock::now();
            std::chrono::duration<double> diff = tpFinish - tpStart;
            std::cout << "  SkipList fill in " << diff.count() << std::endl;

            tpStart = high_resolution_clock::now();
            for (auto const& v : sVect)
            {
                sl.erase(v);
            }
            tpFinish = high_resolution_clock::now();
            diff = tpFinish - tpStart;
            std::cout << "  SkipList erase in " << diff.count() << std::endl;
        }
    }
}

void benchFillAsc()
{
    std::cout << "\n\nBENCH FILL ASCENDING" << std::endl;
    for (int c = 1; c <= 100000; c *= 10)
    {
        std::cout << std::endl << "Number of elements: " << c << std::endl;
        fillRandomInt(c);
        {
            std::set<int>  set;
            auto tpStart = high_resolution_clock::now();
            for (auto const& v : sVectInts)
            {
                set.insert(v);
            }
            auto tpFinish = high_resolution_clock::now();
            std::chrono::duration<double> diff = tpFinish - tpStart;
            std::cout << "  Set fill in " << diff.count() << std::endl;

            tpStart = high_resolution_clock::now();
            for (auto const& v : sVectInts)
            {
                set.erase(v);
            }
            tpFinish = high_resolution_clock::now();
            diff = tpFinish - tpStart;
            std::cout << "  Set erase in " << diff.count() << std::endl;
        }

        {
            random<float> floatRand;
            algic::skip_list<int, random<float>>  sl(floatRand);
            auto tpStart = high_resolution_clock::now();
            for (auto const& v : sVectInts)
            {
                sl.insert(v);
            }
            auto tpFinish = high_resolution_clock::now();
            std::chrono::duration<double> diff = tpFinish - tpStart;
            std::cout << "  SkipList fill in " << diff.count() << std::endl;

            tpStart = high_resolution_clock::now();
            for (auto const& v : sVectInts)
            {
                sl.erase(v);
            }
            tpFinish = high_resolution_clock::now();
            diff = tpFinish - tpStart;
            std::cout << "  SkipList erase in " << diff.count() << std::endl;
        }
    }
}

void benchFillDesc()
{
    std::cout << "\n\nBENCH FILL DESCENDING" << std::endl;
    for (int c = 1; c <= 100000; c *= 10)
    {
        std::cout << std::endl << "Number of elements: " << c << std::endl;
        fillRandomInt(c);
        {
            std::set<int>  set;
            auto tpStart = high_resolution_clock::now();
            for (auto v = std::crbegin(sVectInts); v != std::crend(sVectInts); ++v)
            {
                set.insert(*v);
            }
            auto tpFinish = high_resolution_clock::now();
            std::chrono::duration<double> diff = tpFinish - tpStart;
            std::cout << "  Set fill in " << diff.count() << std::endl;

            tpStart = high_resolution_clock::now();
            for (auto v = std::crbegin(sVectInts); v != std::crend(sVectInts); ++v)
            {
                set.erase(*v);
            }
            tpFinish = high_resolution_clock::now();
            diff = tpFinish - tpStart;
            std::cout << "  Set erase in " << diff.count() << std::endl;
        }

        {
            random<float> floatRand;
            algic::skip_list<int, random<float>>  sl(floatRand);
            auto tpStart = high_resolution_clock::now();
            for (auto v = std::crbegin(sVectInts); v != std::crend(sVectInts); ++v)
            {
                sl.insert(*v);
            }
            auto tpFinish = high_resolution_clock::now();
            std::chrono::duration<double> diff = tpFinish - tpStart;
            std::cout << "  SkipList fill in " << diff.count() << std::endl;

            tpStart = high_resolution_clock::now();
            for (auto v = std::crbegin(sVectInts); v != std::crend(sVectInts); ++v)
            {
                sl.erase(*v);
            }
            tpFinish = high_resolution_clock::now();
            diff = tpFinish - tpStart;
            std::cout << "  SkipList erase in " << diff.count() << std::endl;
        }
    }
}