#include <chrono>
#include <iomanip>
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
void fillSeqInt(size_t count);
void benchFill();
void benchFillRange();
void benchFillAsc();
void benchFillDesc();
void benchFind();


/*********** MAIN ***********/
int main(int argc, char* argv[])
{
    benchFill();
    benchFillAsc();
    benchFillDesc();
    benchFind();

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

void fillSeqInt(size_t count)
{
    sVectInts.clear();
    for (int i = 0; i < count; ++i)
    {
        sVectInts.push_back(i);
    }
}

void benchFill()
{
    std::cout << "\n\nBENCH FILL\t";
    std::vector<std::pair<double, double>> timesFill;
    std::vector<std::pair<double, double>> timesErase;

    for (int c = 1; c <= 100000; c *= 10)
    {
        std::cout << c << " ";
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
            timesFill.push_back(std::make_pair(diff.count(), 0.0));

            tpStart = high_resolution_clock::now();
            for (auto const& v : sVect)
            {
                set.erase(v);
            }
            tpFinish = high_resolution_clock::now();
            diff = tpFinish - tpStart;
            timesErase.push_back(std::make_pair(diff.count(), 0.0));
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
            timesFill.back().second = diff.count();

            tpStart = high_resolution_clock::now();
            for (auto const& v : sVect)
            {
                sl.erase(v);
            }
            tpFinish = high_resolution_clock::now();
            diff = tpFinish - tpStart;
            timesErase.back().second = diff.count();
        }
    }

    std::cout << "\n  Fill times for 1, 10, 100, ... elements:\n  ";
    for (auto const& t : timesFill)
        std::cout << std::setprecision(4) << t.first << ",\t";
    std::cout << "\n  ";
    for (auto const& t : timesFill)
        std::cout << std::setprecision(4) << t.second << ",\t";
    std::cout << std::endl;
}

void benchFillAsc()
{
    std::cout << "\n\nBENCH FILL ASCENDING\t";
    std::vector<std::pair<double, double>> timesFill;
    std::vector<std::pair<double, double>> timesErase;

    for (int c = 1; c <= 100000; c *= 10)
    {
        std::cout << c << " ";
        fillSeqInt(c);
        {
            std::set<int>  set;
            auto tpStart = high_resolution_clock::now();
            for (auto const& v : sVectInts)
            {
                set.insert(v);
            }
            auto tpFinish = high_resolution_clock::now();
            std::chrono::duration<double> diff = tpFinish - tpStart;
            timesFill.push_back(std::make_pair(diff.count(), 0.0));

            tpStart = high_resolution_clock::now();
            for (auto const& v : sVectInts)
            {
                set.erase(v);
            }
            tpFinish = high_resolution_clock::now();
            diff = tpFinish - tpStart;
            timesErase.push_back(std::make_pair(diff.count(), 0.0));
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
            timesFill.back().second = diff.count();

            tpStart = high_resolution_clock::now();
            for (auto const& v : sVectInts)
            {
                sl.erase(v);
            }
            tpFinish = high_resolution_clock::now();
            diff = tpFinish - tpStart;
            timesErase.back().second = diff.count();
        }
    }

    std::cout << "\n  Fill times for 1, 10, 100, ... elements:\n  ";
    for (auto const& t : timesFill)
        std::cout << std::setprecision(4) << t.first << ",\t";
    std::cout << "\n  ";
    for (auto const& t : timesFill)
        std::cout << std::setprecision(4) << t.second << ",\t";
    std::cout << std::endl;
}

void benchFillDesc()
{
    std::cout << "\n\nBENCH FILL DESCENDING\t";
    std::vector<std::pair<double, double>> timesFill;
    std::vector<std::pair<double, double>> timesErase;

    for (int c = 1; c <= 100000; c *= 10)
    {
        std::cout << c << " ";
        fillSeqInt(c);
        {
            std::set<int>  set;
            auto tpStart = high_resolution_clock::now();
            for (auto v = std::crbegin(sVectInts); v != std::crend(sVectInts); ++v)
            {
                set.insert(*v);
            }
            auto tpFinish = high_resolution_clock::now();
            std::chrono::duration<double> diff = tpFinish - tpStart;
            timesFill.push_back(std::make_pair(diff.count(), 0.0));

            tpStart = high_resolution_clock::now();
            for (auto v = std::crbegin(sVectInts); v != std::crend(sVectInts); ++v)
            {
                set.erase(*v);
            }
            tpFinish = high_resolution_clock::now();
            diff = tpFinish - tpStart;
            timesErase.push_back(std::make_pair(diff.count(), 0.0));
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
            timesFill.back().second = diff.count();

            tpStart = high_resolution_clock::now();
            for (auto v = std::crbegin(sVectInts); v != std::crend(sVectInts); ++v)
            {
                sl.erase(*v);
            }
            tpFinish = high_resolution_clock::now();
            diff = tpFinish - tpStart;
            timesErase.back().second = diff.count();
        }
    }

    std::cout << "\n  Fill times for 1, 10, 100, ... elements:\n\t";
    for (auto const& t : timesFill)
        std::cout << t.first << ",  ";
    std::cout << "\n  ";
    for (auto const& t : timesFill)
        std::cout << t.second << ",\t";
    std::cout << std::endl;
}

void benchFind()
{
    std::cout << "\n\nBENCH FIND\t";
    std::vector<std::pair<double, double>> timesFind;

    for (int c = 1; c <= 100000; c *= 10)
    {
        std::cout << c << " ";
        fillSeqInt(c);
        {
            std::set<int>  set;
            for (auto v = std::crbegin(sVectInts); v != std::crend(sVectInts); ++v)
            {
                set.insert(*v);
            }

            auto tpStart = high_resolution_clock::now();
            for (int i = 0; i < c; ++i)
            {
                set.find(i);
            }
            auto tpFinish = high_resolution_clock::now();
            std::chrono::duration<double> diff = tpFinish - tpStart;
            timesFind.push_back(std::make_pair(diff.count(), 0.0));
        }

        {
            random<float> floatRand;
            algic::skip_list<int, random<float>>  sl(floatRand);
            for (auto v = std::crbegin(sVectInts); v != std::crend(sVectInts); ++v)
            {
                sl.insert(*v);
            }

            auto tpStart = high_resolution_clock::now();
            for (int i = 0; i < c; ++i)
            {
                sl.find(i);
            }
            auto tpFinish = high_resolution_clock::now();
            std::chrono::duration<double> diff = tpFinish - tpStart;
            timesFind.back().second = diff.count();
        }
    }

    std::cout << "\n  Find times for 1, 10, 100, ... elements:\n\t";
    for (auto const& t : timesFind)
        std::cout << t.first << ",  ";
    std::cout << "\n  ";
    for (auto const& t : timesFind)
        std::cout << t.second << ",\t";
    std::cout << std::endl;
}