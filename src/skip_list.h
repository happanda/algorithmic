#ifndef ALGORITHMIC_SKIP_LIST_H
#define ALGORITHMIC_SKIP_LIST_H
#include <iterator>


namespace algic
{
    struct node_base;
    
    
    /**********************************************************/
    /*                     skip_list                          */
    template <class T>
    struct skip_list
    {
        struct iterator
            : public std::iterator<std::forward_iterator_tag, void, void, void, void>
        {
        };

        skip_list();
        ~skip_list();

        bool insert(T const& t);
        bool remove(T const& t);
        bool contains(T const& t);
        
    private:
        node_base*  mHead;
    };
} // namespace algic

#include "skip_list.hpp"

#endif
