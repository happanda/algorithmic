#ifndef ALGORITHMIC_SKIP_LIST_H
#define ALGORITHMIC_SKIP_LIST_H
#include <iterator>


namespace algic
{
    struct node_base;

    template <class T, class RandomGen>//, class Compare = std::less<Key>, class Allocator = std::allocator<T>>
    struct skip_list;
    
    template <class T>
    struct slist_const_iterator
    {
        template <class RandomGen>//, class Compare = std::less<Key>, class Allocator = std::allocator<T>>
        slist_const_iterator(skip_list<T, RandomGen/*, Compare, Allocator*/> const* slist, node_base* node);

        slist_const_iterator(slist_const_iterator const& rhs);
        slist_const_iterator const& operator=(slist_const_iterator const& rhs);

        T const& operator*() const;

        slist_const_iterator& operator++();
        slist_const_iterator operator++(int);

        bool operator==(slist_const_iterator const& rhs);
        bool operator!=(slist_const_iterator const& rhs);

    private:
        void const* mSlist;
        node_base* mNode;
    };


    /**********************************************************/
    /*                     skip_list                          */
    template <class T, class RandomGen>//, class Compare = std::less<Key>, class Allocator = std::allocator<T>>
    struct skip_list
    {
        typedef T key_type;
        typedef T value_type;
        typedef std::size_t  size_type;
        //typedef Compare key_compare;
        //typedef Compare value_compare;
        //typedef Allocator allocator_type;
        typedef value_type& reference;
        typedef value_type const& const_reference;
        //typedef std::allocator_traits<Allocator>::pointer pointer;
        //typedef std::allocator_traits<Allocator>::pointer const_pointer;
        typedef slist_const_iterator<T> iterator;
        typedef slist_const_iterator<T> const_iterator;

        skip_list(RandomGen& randGen, float prob = 0.36787944117144f /* prob = 1 / e */);
        ~skip_list();

        iterator begin();
        const_iterator begin() const;
        const_iterator cbegin() const;
        iterator end();
        const_iterator end() const;
        const_iterator cend() const;

        void swap(skip_list& rhs);

        bool empty() const;
        size_type size() const;

        void clear();

        bool insert(T const& t);
        bool erase(T const& t);
        bool contains(T const& t);
        
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
