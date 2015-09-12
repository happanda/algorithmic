#ifndef ALGORITHMIC_SKIP_LIST_HPP
#define ALGORITHMIC_SKIP_LIST_HPP
#include <algorithm>
#include <array>
#include <cassert>
#include <functional>
#include <iostream>
#include <list>
#include <vector>


namespace algic
{
    using std::size_t;

    template <class Key>
    struct node;


    struct node_base
    {
        node_base(size_t height);
        virtual ~node_base();

        size_t height() const;
        node_base* next(size_t level) const;
        void set(size_t level, node_base* nb);

        void incHeight();
        void decHeight();

        template <class Key>
        node<Key>* as()
        {
            return dynamic_cast<node<Key>*>(this);
        }

    private:
        size_t  mHeight;
        node_base**  mLinks;
    };


    template <class Key>
    struct node : public node_base
    {
        node(size_t height, Key const& key);
        node(size_t height, Key&& key);
        ~node();

        Key& value();
        Key const& value() const;

    private:
        Key* asT();
        Key const* asT() const;

        char mData[sizeof(Key)];
    };

    template <class Key>
    bool less(node_base* node, Key const& val)
    {
        if (auto tNode = node->as<Key>())
            return std::less<Key>()(tNode->value(), val);
        else
            return true;
    }

    template <class Key>
    bool eq(node_base* node, Key const& val)
    {
        if (auto tNode = node->as<Key>())
            return std::equal_to<Key>()(tNode->value(), val);
        else
            return false;
    }

    template <class Key>
    bool gt(node_base* node, Key const& val)
    {
        if (auto tNode = node->as<Key>())
            return std::greater<Key>()(tNode->value(), val);
        else
            return false;
    }



    /**********************************************************/
    /*                  implementations                       */

    /**********************************************************/
    /*                      node_base                         */
    node_base::node_base(size_t height)
        : mHeight(height)
        , mLinks(new node_base*[mHeight])
    {
        std::fill(mLinks, mLinks + mHeight, nullptr);
    }

    node_base::~node_base()
    {
        delete [] mLinks;
    }

    size_t node_base::height() const
    {
        return mHeight;
    }

    node_base* node_base::next(size_t level) const
    {
        assert(level < mHeight && "Index out of range");
        return mLinks[level];
    }

    void node_base::set(size_t level, node_base* nb)
    {
        assert(level < mHeight && "Index out of range");
        mLinks[level] = nb;
    }

    void node_base::incHeight()
    {
        ++mHeight;
        node_base** newLinks = new node_base*[mHeight];
        std::copy(mLinks, mLinks + mHeight, newLinks);
        delete mLinks;
        mLinks = newLinks;
        mLinks[mHeight - 1] = nullptr;
    }

    void node_base::decHeight()
    {
        assert(mHeight > 0 && "Decreasing zero height is illegal");
        --mHeight;
        node_base** newLinks = new node_base*[mHeight];
        std::copy(mLinks, mLinks + mHeight, newLinks);
        delete mLinks;
        mLinks = newLinks;
        mLinks[mHeight - 1] = nullptr;
    }
    

    /**********************************************************/
    /*                         node                           */
    template <class Key>
    node<Key>::node(size_t height, Key const& key)
        : node_base(height)
    {
        new (static_cast<void*>(mData))Key(key);
    }

    template <class Key>
    node<Key>::node(size_t height, Key&& key)
        : node_base(height)
    {
        new (static_cast<void*>(mData))Key(std::move(key));
    }

    template <class Key>
    node<Key>::~node()
    {
        static_cast<Key*>(static_cast<void*>(mData))->~Key();
    }

    template <class Key>
    Key& node<Key>::value()
    {
        return static_cast<Key&>(*asT());
    }

    template <class Key>
    Key const& node<Key>::value() const
    {
        return static_cast<Key const&>(*asT());
    }

    template <class Key>
    Key* node<Key>::asT()
    {
        return static_cast<Key*>(static_cast<void*>(mData));
    }

    template <class Key>
    Key const* node<Key>::asT() const
    {
        return static_cast<Key const*>(static_cast<void*>(mData));
    }


    /**********************************************************/
    /*                 slist_const_iterator                   */

    template <class Key>
    template <class RandomGen>//, class Compare = std::less<Key>, class Allocator = std::allocator<Key>>
    slist_const_iterator<Key>::slist_const_iterator(skip_list<Key, RandomGen/*, Compare, Allocator*/> const* slist, node_base* node)
        : mSlist(static_cast<void const*>(slist))
        , mNode(node)
    {
    }

    template <class Key>
    slist_const_iterator<Key>::slist_const_iterator(slist_const_iterator const& rhs)
        : mSlist(rhs.mSlist)
        , mNode(rhs.mNode)
    {
    }

    template <class Key>
    slist_const_iterator<Key> const& slist_const_iterator<Key>::operator=(slist_const_iterator const& rhs)
    {
        mSlist = rhs.mSlist;
        mNode = rhs.mNode;
    }

    template <class Key>
    Key const& slist_const_iterator<Key>::operator*() const
    {
        return mNode->as<Key>()->value();
    }

    template <class Key>
    slist_const_iterator<Key>& slist_const_iterator<Key>::operator++()
    {
        mNode = mNode->next(0);
        return *this;
    }

    template <class Key>
    slist_const_iterator<Key> slist_const_iterator<Key>::operator++(int)
    {
        auto prevIter = *this;
        mNode = mNode->next(0);
        return prevIter;
    }

    template <class Key>
    bool slist_const_iterator<Key>::operator==(slist_const_iterator const& rhs)
    {
        return mSlist == rhs.mSlist && mNode == rhs.mNode;
    }

    template <class Key>
    bool slist_const_iterator<Key>::operator!=(slist_const_iterator const& rhs)
    {
        return mSlist != rhs.mSlist || mNode != rhs.mNode;
    }

    template <class Key>
    template <class RandomGen>//, class Compare = std::less<Key>, class Allocator = std::allocator<Key>>
    slist_iterator<Key>::slist_iterator(skip_list<Key, RandomGen/*, Compare, Allocator*/> const* slist, node_base* node)
        : slist_const_iterator(slist, node)
    {
    }
    
    /**********************************************************/
    /*                      skip_list                         */

    template <class Key, class RandomGen>
    skip_list<Key, RandomGen>::skip_list(RandomGen& randGen, float prob)
        : mRand(randGen)
        , mHead(new node_base(1))
        , mProb(prob)
    {
        assert(prob >= 0.0f && "Probability can'key be negative");
        assert(prob <= 1.0f && "Probability must be not greater than 1");
    }

    template <class Key, class RandomGen>
    skip_list<Key, RandomGen>::~skip_list()
    {
        node_base* curNode = mHead;
        while (mHead)
        {
            mHead = mHead->next(0);
            delete curNode;
            curNode = mHead;
        }
    }

    template <class Key, class RandomGen>
    typename skip_list<Key, RandomGen>::iterator skip_list<Key, RandomGen>::begin()
    {
        return slist_const_iterator<Key>(this, mHead->next(0));
    }

    template <class Key, class RandomGen>
    typename skip_list<Key, RandomGen>::const_iterator skip_list<Key, RandomGen>::begin() const
    {
        return slist_const_iterator<Key>(this, mHead->next(0));
    }

    template <class Key, class RandomGen>
    typename skip_list<Key, RandomGen>::const_iterator skip_list<Key, RandomGen>::cbegin() const
    {
        return slist_const_iterator<Key>(this, mHead->next(0));
    }

    template <class Key, class RandomGen>
    typename skip_list<Key, RandomGen>::iterator skip_list<Key, RandomGen>::end()
    {
        return slist_iterator<Key>(this, nullptr);
    }

    template <class Key, class RandomGen>
    typename skip_list<Key, RandomGen>::const_iterator skip_list<Key, RandomGen>::end() const
    {
        return slist_const_iterator<Key>(this, nullptr);
    }

    template <class Key, class RandomGen>
    typename skip_list<Key, RandomGen>::const_iterator skip_list<Key, RandomGen>::cend() const
    {
        return slist_const_iterator<Key>(this, nullptr);
    }


    template <class Key, class RandomGen>
    void skip_list<Key, RandomGen>::swap(skip_list& rhs)
    {
        std::swap(mRand, rhs.mRand);
        std::swap(mProb, rhs.mProb);
        std::swap(mHead, rhs.mHead);
        std::swap(mSize, rhs.mSize);
    }

    template <class Key, class RandomGen>
    bool skip_list<Key, RandomGen>::empty() const
    {
        return (mHead->next(0) == nullptr);
    }

    template <class Key, class RandomGen>
    typename skip_list<Key, RandomGen>::size_type skip_list<Key, RandomGen>::size() const
    {
        return mSize;
    }

    template <class Key, class RandomGen>
    void skip_list<Key, RandomGen>::clear()
    {
        node_base* curNode = mHead;
        while (mHead)
        {
            mHead = mHead->next(0);
            delete curNode;
            curNode = mHead;
        }
        mHead = new node_base(1);
    }

    template <class Key, class RandomGen>
    typename skip_list<Key, RandomGen>::pairib skip_list<Key, RandomGen>::insert(Key const& key)
    {
        return insert(std::move(Key(key)));
    }

    template <class Key, class RandomGen>
    typename skip_list<Key, RandomGen>::pairib skip_list<Key, RandomGen>::insert(Key&& key)
    {
        size_t const H = mHead->height();
        // treat first element in a special way
        if (H == 1 && !mHead->next(0))
        {
            node_base* newNode = new node<Key>(1, std::move(key));
            mHead->set(0, newNode);
            return std::make_pair(iterator(this, newNode), true);
        }

        std::vector<node_base*> visited(H + 1, nullptr);
        if (visit(key, &visited)) // if already exists
            return std::make_pair(end(), false);

        // randomly choose the height of the new element
        size_t const newLvl = multiCoin();
        node_base* newNode = new node<Key>(newLvl, std::move(key));
        if (newLvl > H)
        {
            mHead->incHeight();
            mHead->set(H, newNode);
        }

        // reassign links
        size_t const minLvl = std::min(newLvl, H);
        for (size_t i = 0; i < minLvl; ++i)
        {
            newNode->set(i, visited[i]->next(i));
            visited[i]->set(i, newNode);
        }
        ++mSize;
        return std::make_pair(iterator(this, newNode), true);
    }

    template <class Key, class RandomGen>
    template <class IterType>
    void skip_list<Key, RandomGen>::insert(IterType first, IterType last)
    {
        for (auto it = first; it != last; ++it)
        {
            insert(*it);
        }
    }

    template <class Key, class RandomGen>
    void skip_list<Key, RandomGen>::insert(std::initializer_list<value_type> ilist)
    {
        insert(std::cbegin(ilist), std::cend(ilist));
    }

    template <class Key, class RandomGen>
    typename skip_list<Key, RandomGen>::iterator skip_list<Key, RandomGen>::erase(iterator pos)
    {
        if (pos == end())
            return;
        iterator next = pos;
        ++next;
        erase(pos.mNode->as<Key>()->value());
        return next;
    }

    template <class Key, class RandomGen>
    typename skip_list<Key, RandomGen>::const_iterator skip_list<Key, RandomGen>::erase(const_iterator pos)
    {
        if (pos == end())
            return;
        const_iterator next = pos;
        ++next;
        erase(pos.mNode->as<Key>()->value());
        return next;
    }

    template <class Key, class RandomGen>
    typename skip_list<Key, RandomGen>::size_type skip_list<Key, RandomGen>::erase(Key const& key)
    {
        size_t const H = mHead->height();
        std::vector<node_base*> visited(H, nullptr);
        if (node_base* foundNode = visit(key, &visited))
        {
                // reassign links and delete the element
            size_t const H = mHead->height();
            for (size_t i = 0; i < H; ++i)
            {
                if (visited[i] && visited[i]->next(i) == foundNode)
                    visited[i]->set(i, foundNode->next(i));
            }
            delete foundNode;
            --mSize;
            if (!mHead->next(H - 1))
                mHead->decHeight();
            return 1;
        }
        else
            return 0;
    }

    template <class Key, class RandomGen>
    typename skip_list<Key, RandomGen>::size_type skip_list<Key, RandomGen>::count(Key const& key)
    {
        return visit(key) ? 1 : 0;
    }

    template <class Key, class RandomGen>
    typename skip_list<Key, RandomGen>::iterator skip_list<Key, RandomGen>::find(Key const& key)
    {
        auto node = visit(key);
        return iterator(this, node);
    }

    template <class Key, class RandomGen>
    typename skip_list<Key, RandomGen>::const_iterator skip_list<Key, RandomGen>::find(Key const& key) const
    {
        auto node = visit(key);
        return const_iterator(this, node);
    }

    template <class Key, class RandomGen>
    typename skip_list<Key, RandomGen>::pairit skip_list<Key, RandomGen>::equal_range(const Key& key)
    {
        auto found = visit(key, nullptr, false);
        if (!found)
            return std::make_pair(iterator(this, node), iterator(this, node));

        auto nextNode = found->next(0);
        if (less(found, key))
            return std::make_pair(iterator(this, nextNode), iterator(this, nextNode));
        else
            return std::make_pair(iterator(this, found), iterator(this, nextNode));
    }

    template <class Key, class RandomGen>
    typename skip_list<Key, RandomGen>::paircit skip_list<Key, RandomGen>::equal_range(const Key& key) const
    {
        auto found = visit(key, nullptr, false);
        if (!found)
            return std::make_pair(const_iterator(this, node), const_iterator(this, node));

        auto nextNode = found->next(0);
        if (less(found, key))
            return std::make_pair(const_iterator(this, nextNode), const_iterator(this, nextNode));
        else
            return std::make_pair(const_iterator(this, found), const_iterator(this, nextNode));
    }

    template <class Key, class RandomGen>
    typename skip_list<Key, RandomGen>::iterator skip_list<Key, RandomGen>::lower_bound(const Key& key)
    {
        auto found = visit(key, nullptr, false);
        if (!found || !less(found, key)) // either not found, or found is equal
            return iterator(this, found);
        return iterator(this, found->next(0));
    }

    template <class Key, class RandomGen>
    typename skip_list<Key, RandomGen>::const_iterator skip_list<Key, RandomGen>::lower_bound(const Key& key) const
    {
        auto found = visit(key, nullptr, false);
        if (!found || !less(found, key)) // either not found, or found is equal
            return const_iterator(this, found);
        return const_iterator(this, found->next(0));
    }

    template <class Key, class RandomGen>
    typename skip_list<Key, RandomGen>::iterator skip_list<Key, RandomGen>::upper_bound(const Key& key)
    {
        auto found = visit(key, nullptr, false);
        return found ? iterator(this, found->next(0)) : iterator(this, found);
    }

    template <class Key, class RandomGen>
    typename skip_list<Key, RandomGen>::const_iterator skip_list<Key, RandomGen>::upper_bound(const Key& key) const
    {
        auto found = visit(key, nullptr, false);
        return found ? const_iterator(this, found->next(0)) : const_iterator(this, found);
    }

    template <class Key, class RandomGen>
    bool skip_list<Key, RandomGen>::contains(Key const& key)
    {
        return count(key) == 1;
    }

    template <class Key, class RandomGen>
    node_base* skip_list<Key, RandomGen>::visit(Key const& key, std::vector<node_base*>* visited, bool returnEqOnly) const
    {
        size_t const H = mHead->height();
        if (H == 1 && !mHead->next(0))
            return nullptr;

        node_base* foundNode = nullptr;
        node_base* curNode = mHead;
        int curLvl = H - 1;
        bool out = false;
        while (!out)
        {
            auto nextNode = curNode->next(curLvl);
            if (nextNode && less(nextNode, key))
            {
                curNode = nextNode;
                continue;
            }
            else if (curLvl >= 0)
            {
                if (visited)
                    (*visited)[curLvl] = curNode;
                --curLvl;
                if (curLvl < 0)
                {
                    if (nextNode && eq(nextNode, key))
                        return nextNode;
                    else if (returnEqOnly)
                        return nullptr;
                    return curNode;
                }
            }
        }
        assert(false && "This statement should not be reached");
        return nullptr;
    }

    template <class Key, class RandomGen>
    bool skip_list<Key, RandomGen>::coin() const
    {
        return mRand() < mProb;
    }

    template <class Key, class RandomGen>
    size_t skip_list<Key, RandomGen>::multiCoin() const
    {
        size_t const maxH{ mHead->height() + 1 };
        size_t count{ 1 };
        while (mRand() < mProb)
        {
            ++count;
            if (count == maxH)
                break;
        }
        return count;
    }
} // namespace algic

#endif
