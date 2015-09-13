#include <iostream>
#include <set>
#include <string>
#include "random.h"
#include "skip_list.h"
#include "tests.h"


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
    ::testing::InitGoogleTest(&argc, argv);

    int result = RUN_ALL_TESTS();

    getchar();
    return result;
}
