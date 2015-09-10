#include <iostream>
#include <string>
#include "random.h"
#include "skip_list.h"

struct foo
{
    foo()
    {
        std::cout << "Construct foo" << std::endl;
    }

    foo(foo const& f)
    {
        std::cout << "Copy construct foo" << std::endl;
    }
    
    ~foo()
    {
        std::cout << "Destroy foo" << std::endl;
    }
};

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

    slist.print();

    std::cout << std::endl << "List constains 12: " << slist.contains(12) << std::endl;

    getchar();
    return 0;
}
