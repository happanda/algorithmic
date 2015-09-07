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
        algic::node<foo, 5> node(fu);
        algic::skip_list<foo, random<float>>  slist(randGen);
    }
    foo f;
    return 0;
}
