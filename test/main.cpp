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
    random<float> randGen;
    {
        foo fu;
        algic::node<foo> nd(5, fu);
        algic::skip_list<int, random<float>>  slist(randGen);
        std::cout << slist.insert(2) << std::endl;
        std::cout << slist.insert(1) << std::endl;
        for (int i = 0; i < 5000; ++i)
        {
            std::cout << slist.insert(std::rand()) << std::endl;
        }
        std::cout << std::endl << "Print list" << std::endl;
        slist.print();
    }
    foo f;
    getchar();
    return 0;
}
