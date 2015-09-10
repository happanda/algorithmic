#ifndef ALGORITHMIC_SKIP_LIST_H
#define ALGORITHMIC_SKIP_LIST_H
#include <iterator>


namespace algic
{
    struct node_base;
    
    
    /**********************************************************/
    /*                     skip_list                          */
    template <class T, class RandomGen>
    struct skip_list
    {
        struct iterator
            : public std::iterator<std::forward_iterator_tag, void, void, void, void>
        {
        };

        skip_list(RandomGen& randGen, float prob = 0.36787944117144f /* prob = 1 / e */);
        ~skip_list();

        bool insert(T const& t);
        bool remove(T const& t);
        bool contains(T const& t);
        
        void print() const;

    private:
        node_base* visit(T const& t, std::vector<node_base*>* visited = nullptr);
        bool coin() const;
        size_t multiCoin() const;

        mutable RandomGen& mRand;
        float mProb;
        node_base* mHead;
    };
} // namespace algic

#include "skip_list.hpp"

#endif
