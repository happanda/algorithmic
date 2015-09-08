#ifndef ALGORITHMIC_SKIP_LIST_HPP
#define ALGORITHMIC_SKIP_LIST_HPP
#include <algorithm>
#include <array>
#include <cassert>
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
        delete mHead;
    }

    template <class T, class RandomGen>
    bool skip_list<T, RandomGen>::insert(T const& t)
    {
    }

    template <class T, class RandomGen>
    bool skip_list<T, RandomGen>::remove(T const& t)
    {
    }

    template <class T, class RandomGen>
    bool skip_list<T, RandomGen>::contains(T const& t)
    {
        size_t const H = mHead->height();
        for (size_t lvl = H - 1; lvl >= 0; --lvl)
        {
            if (node_base* place = findPlace(t, H, lvl, mHead->next(lvl)))
            {
                if (place->as<T>()->value() == t)
                    return true;
                else
                    return false;
            }
            if (lvl == 0)
                break;
        }
        return false;
    }

    template <class T, class RandomGen>
    node_base* skip_list<T, RandomGen>::findPlace(T const& t, size_t height, size_t level, node_base* start)
    {
        if (!start)
            return nullptr;
        
        auto const& val = start->as<T>()->value();
        if (val < t)
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
        if (val > t)
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
        size_t count{ 0 };
        while (mRand() < mProb)
            ++count;
        return count;
    }
} // namespace algic

#endif
