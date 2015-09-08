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
        slist.insert(1);
        slist.insert(2);
        slist.insert(3);
        slist.insert(4);
        slist.insert(5);
        slist.insert(12);
        bool const contains = slist.contains(12);
    }
    foo f;
    return 0;
}
