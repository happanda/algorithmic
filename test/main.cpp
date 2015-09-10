#include <iostream>
#include <set>
#include <string>
#include "random.h"
#include "skip_list.h"


template <class T, class RandomGen>
void print(algic::skip_list<T, RandomGen> const& slist)
{
    for (auto it = std::cbegin(slist); it != std::cend(slist); ++it)
    {
        std::cout << *it << std::endl;
    }
}


int main(int argc, char* argv[])
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    random<float> randGen;
    algic::skip_list<int, random<float>>  slist(randGen);

    std::cout << std::endl << "List constains 12: " << slist.contains(12) << std::endl;

    for (int i = 0; i < 5000; ++i)
    {
        int const rnd = std::rand();
        slist.insert(rnd);
    }
    std::cout << std::endl << "Print list" << std::endl;
    print(slist);

    algic::skip_list<int, random<float>>  slistSwap(randGen);
    std::swap(slist, slistSwap);
    //print(slistSwap);

    std::cout << std::endl << "List constains 12: " << slist.contains(12) << std::endl;
    std::set<int> sint({ 1, 2, 3 });
    std::cout << *(sint.begin()) << std::endl;
    std::cout << *(++sint.begin()) << std::endl;
    std::cout << *(sint.begin()++) << std::endl;

    getchar();
    return 0;
}
