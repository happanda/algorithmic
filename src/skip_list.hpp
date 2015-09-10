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

    template <class T>
    struct node;


    struct node_base
    {
        node_base(size_t height);
        virtual ~node_base();

        size_t height() const;
        node_base* next(size_t level) const;
        void set(size_t level, node_base* nb);

        void incHeight();

        template <class T>
        node<T>* as()
        {
            return dynamic_cast<node<T>*>(this);
        }

    private:
        size_t  mHeight;
        node_base**  mLinks;
    };


    template <class T>
    struct node : public node_base
    {
        node(size_t height, T const& t);
        node(size_t height, T&& t);
        ~node();

        T& value();
        T const& value() const;

    private:
        T* asT();
        T const* asT() const;

        char mData[sizeof(T)];
    };

    template <class T>
    bool less(node_base* node, T const& val)
    {
        if (auto tNode = node->as<T>())
            return std::less<T>()(tNode->value(), val);
        else
            return true;
    }

    template <class T>
    bool eq(node_base* node, T const& val)
    {
        if (auto tNode = node->as<T>())
            return std::equal_to<T>()(tNode->value(), val);
        else
            return false;
    }

    template <class T>
    bool gt(node_base* node, T const& val)
    {
        if (auto tNode = node->as<T>())
            return std::greater<T>()(tNode->value(), val);
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
    

    /**********************************************************/
    /*                         node                           */
    template <class T>
    node<T>::node(size_t height, T const& t)
        : node_base(height)
    {
        new (static_cast<void*>(mData))T(t);
    }

    template <class T>
    node<T>::node(size_t height, T&& t)
        : node_base(height)
    {
        new (static_cast<void*>(mData))T(std::move(t));
    }

    template <class T>
    node<T>::~node()
    {
        static_cast<T*>(static_cast<void*>(mData))->~T();
    }

    template <class T>
    T& node<T>::value()
    {
        return static_cast<T&>(*asT());
    }

    template <class T>
    T const& node<T>::value() const
    {
        return static_cast<T const&>(*asT());
    }

    template <class T>
    T* node<T>::asT()
    {
        return static_cast<T*>(static_cast<void*>(mData));
    }

    template <class T>
    T const* node<T>::asT() const
    {
        return static_cast<T const*>(static_cast<void*>(mData));
    }


    /**********************************************************/
    /*                      skip_list                         */
    template <class T, class RandomGen>
    skip_list<T, RandomGen>::skip_list(RandomGen& randGen, float prob)
        : mRand(randGen)
        , mHead(new node_base(1))
        , mProb(prob)
    {
        assert(prob >= 0.0f && "Probability can't be negative");
        assert(prob <= 1.0f && "Probability must be not greater than 1");
    }

    template <class T, class RandomGen>
    skip_list<T, RandomGen>::~skip_list()
    {
        node_base* curNode = mHead;
        while (mHead)
        {
            mHead = mHead->next(0);
            delete curNode;
            curNode = mHead;
        }
    }

    template <class T, class RandomGen>
    bool skip_list<T, RandomGen>::insert(T const& t)
    {
        size_t const H = mHead->height();
        if (H == 1 && !mHead->next(0))
        { // first element, treat specially
            node_base* newNode = new node<T>(1, t);
            mHead->set(0, newNode);
            return true;
        }

        std::vector<node_base*> visited(H + 1, nullptr);

        node_base* curNode = mHead;
        int curLvl = H - 1;
        bool out = false;
        while (!out)
        {
            auto nextNode = curNode->next(curLvl);
            if (nextNode && less(nextNode, t))
            {
                curNode = nextNode;
                continue;
            }
            else if (curLvl >= 0)
            {
                visited[curLvl] = curNode;
                --curLvl;
                if (curLvl < 0)
                {
                    if (nextNode && eq(nextNode, t))
                        return false;
                    curLvl = 0;
                    out = true;
                    continue;
                }
            }
        }

        size_t const newLvl = multiCoin();
        node_base* newNode = new node<T>(newLvl, t);
        if (newLvl > H)
        {
            mHead->incHeight();
            mHead->set(H, newNode);
        }

        size_t const minLvl = std::min(newLvl, H);
        for (size_t i = 0; i < minLvl; ++i)
        {
            newNode->set(i, visited[i]->next(i));
            visited[i]->set(i, newNode);
        }
        return true;
    }

    template <class T, class RandomGen>
    bool skip_list<T, RandomGen>::remove(T const& t)
    {
        size_t const H = mHead->height();
        if (H == 1 && !mHead->next(0))
            return false;

        std::vector<node_base*> visited(H, nullptr);

        node_base* foundNode = nullptr;
        node_base* curNode = mHead;
        int curLvl = H - 1;
        bool out = false;
        while (!out)
        {
            auto nextNode = curNode->next(curLvl);
            if (nextNode && less(nextNode, t))
            {
                curNode = nextNode;
                continue;
            }
            else if (curLvl >= 0)
            {
                visited[curLvl] = curNode;
                --curLvl;
                if (curLvl < 0)
                {
                    if (nextNode && eq(nextNode, t))
                    {
                        foundNode = nextNode;
                        out = true;
                        break;
                    }
                    else
                        return false;
                }
            }
        }

        if (!foundNode)
            return false;
        for (size_t i = 0; i < H; ++i)
        {
            if (visited[i] && visited[i]->next(i) == foundNode)
                visited[i]->set(i, foundNode->next(i));
        }
        delete foundNode;
        return true;
    }

    template <class T, class RandomGen>
    bool skip_list<T, RandomGen>::contains(T const& t)
    {
        size_t const H = mHead->height();

        node_base* curNode = mHead;
        int curLvl = H - 1;
        bool out = false;
        while (!out)
        {
            auto nextNode = curNode->next(curLvl);
            if (nextNode && less(nextNode, t))
            {
                curNode = nextNode;
                continue;
            }
            else if (curLvl >= 0)
            {
                --curLvl;
                if (curLvl < 0)
                {
                    if (nextNode && eq(nextNode, t))
                        return true;
                    return false;
                }
            }
        }
        return false;
    }

    template <class T, class RandomGen>
    void skip_list<T, RandomGen>::print() const
    {
        node_base* curNode = mHead;
        while (curNode->next(0))
        {
            curNode = curNode->next(0);
            auto tNode = curNode->as<T>();
            std::cout << tNode->value() << std::endl;
        }
    }

    template <class T, class RandomGen>
    node_base* skip_list<T, RandomGen>::findPlace(T const& t, size_t height, size_t level, node_base* start)
    {
        if (!start)
            return nullptr;
        
        if (less(start, t))
        {
            auto res = findPlace(t, height, level, start->next(level));
            if (!res)
            {
                if (level > 0)
                    return findPlace(t, height, level - 1, start);
                else
                    return start;
            }
        }
        if (gt(start, t))
            return nullptr;
        return start;
    }

    template <class T, class RandomGen>
    bool skip_list<T, RandomGen>::coin() const
    {
        return mRand() < mProb;
    }

    template <class T, class RandomGen>
    size_t skip_list<T, RandomGen>::multiCoin() const
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
