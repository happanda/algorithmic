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

        typedef std::size_t  size_type;


        skip_list(RandomGen& randGen, float prob = 0.36787944117144f /* prob = 1 / e */);
        ~skip_list();

        void swap(skip_list& rhs);

        bool empty() const;
        size_type size() const;

        void clear();

        bool insert(T const& t);
        bool erase(T const& t);
        bool contains(T const& t);
        
        void print() const;

    private:
        node_base* visit(T const& t, std::vector<node_base*>* visited = nullptr);
        bool coin() const;
        size_t multiCoin() const;

        mutable RandomGen& mRand;
        float mProb{ 0.36787944117144f };
        node_base* mHead{ nullptr };
        size_type mSize{ 0 };
    };

    template <class T, class RandomGen>
    void swap(skip_list<T, RandomGen>& lhs, skip_list<T, RandomGen>& rhs)
    {
        lhs.swap(rhs);
    }
} // namespace algic

namespace std
{
    template <class T, class RandomGen>
    void swap(algic::skip_list<T, RandomGen>& lhs, algic::skip_list<T, RandomGen>& rhs)
    {
        algic::swap(lhs, rhs);
    }
} // namespace std
#include "skip_list.hpp"

#endif
