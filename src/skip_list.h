#ifndef ALGORITHMIC_SKIP_LIST_H
#define ALGORITHMIC_SKIP_LIST_H
#include <iterator>


namespace algic
{
    struct node_base;

    template <class Key, class RandomGen>//, class Compare = std::less<Key>, class Allocator = std::allocator<Key>>
    struct skip_list;
    
    template <class Key>
    struct slist_iterator;

    template <class Key>
    struct slist_const_iterator
    {
        typedef std::forward_iterator_tag  iterator_category;
        typedef Key  value_type;
        typedef std::ptrdiff_t  difference_type;
        typedef difference_type distance_type;	// retained
        typedef std::remove_cv_t<Key>*  pointer;
        typedef std::remove_cv_t<Key>&  reference;

        // NOTE: no destructor, cause there's no need in special destructing procedure
        // and the only descendant slist_iterator has no data

        template <class RandomGen>//, class Compare = std::less<Key>, class Allocator = std::allocator<Key>>
        slist_const_iterator(skip_list<Key, RandomGen/*, Compare, Allocator*/> const* slist, node_base* node);
        

        slist_const_iterator(slist_const_iterator const& rhs);
        slist_const_iterator const& operator=(slist_const_iterator const& rhs);

        Key const& operator*() const;

        slist_const_iterator& operator++();
        slist_const_iterator operator++(int);

        bool operator==(slist_const_iterator const& rhs) const;
        bool operator!=(slist_const_iterator const& rhs) const;
        bool operator==(slist_iterator<Key> const& rhs) const;
        bool operator!=(slist_iterator<Key> const& rhs) const;

    private:
        void const* mSlist;
        node_base* mNode;
    };

    template <class Key>
    struct slist_iterator : public slist_const_iterator<Key>
    {
        template <class RandomGen>//, class Compare = std::less<Key>, class Allocator = std::allocator<Key>>
        slist_iterator(skip_list<Key, RandomGen/*, Compare, Allocator*/> const* slist, node_base* node);
    };


    /**********************************************************/
    /*                     skip_list                          */
    template <class Key, class RandomGen>//, class Compare = std::less<Key>, class Allocator = std::allocator<Key>>
    struct skip_list
    {
        typedef Key key_type;
        typedef Key value_type;
        typedef std::size_t  size_type;
        //typedef Compare key_compare;
        //typedef Compare value_compare;
        //typedef Allocator allocator_type;
        typedef value_type& reference;
        typedef value_type const& const_reference;
        //typedef std::allocator_traits<Allocator>::pointer pointer;
        //typedef std::allocator_traits<Allocator>::pointer const_pointer;
        typedef slist_iterator<Key> iterator;
        typedef slist_const_iterator<Key> const_iterator;
        typedef std::pair<iterator, bool> pairib;
        typedef std::pair<iterator, iterator>  pairit;
        typedef std::pair<const_iterator, const_iterator>  paircit;

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

        pairib insert(Key const& key);

        pairib insert(Key&& key);

        template <class IterType>
        void insert(IterType first, IterType last);

        void insert(std::initializer_list<value_type> ilist);

        iterator erase(iterator pos);
        const_iterator erase(const_iterator pos);
        size_type erase(Key const& key);

        size_type count(Key const& key);

        iterator find(Key const& key);
        const_iterator find(Key const& key) const;

        pairit equal_range(const Key& key);
        paircit equal_range(const Key& key) const;

        iterator lower_bound(const Key& key);
        const_iterator lower_bound(const Key& key) const;

        iterator upper_bound(const Key& key);
        const_iterator upper_bound(const Key& key) const;

        bool contains(Key const& key);
        
    private:
        node_base* visit(Key const& key, std::vector<node_base*>* visited = nullptr, bool returnEqOnly = true) const;
        bool coin() const;
        size_t multiCoin() const;

        mutable RandomGen& mRand;
        float mProb{ 0.36787944117144f };
        node_base* mHead{ nullptr };
        size_type mSize{ 0 };
    };

    template <class Key, class RandomGen>
    void swap(skip_list<Key, RandomGen>& lhs, skip_list<Key, RandomGen>& rhs)
    {
        lhs.swap(rhs);
    }
} // namespace algic

namespace std
{
    template <class Key, class RandomGen>
    void swap(algic::skip_list<Key, RandomGen>& lhs, algic::skip_list<Key, RandomGen>& rhs)
    {
        algic::swap(lhs, rhs);
    }
} // namespace std
#include "skip_list.hpp"

#endif
