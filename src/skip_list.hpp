#ifndef ALGORITHMIC_SKIP_LIST_HPP
#define ALGORITHMIC_SKIP_LIST_HPP
#include <algorithm>
#include <array>
#include <cassert>


namespace algic
{
    using std::size_t;

    template <class T, size_t _height>
    struct node;


    struct node_base
    {
        virtual ~node_base() {}

        virtual size_t height() const = 0;
        virtual node_base* next(size_t level) const = 0;
        virtual void set(size_t level, node_base* nb) = 0;
    };


    template <size_t _height>
    struct node_head : public node_base
    {
        node_head();
        ~node_head();

        size_t height() const override;
        node_base* next(size_t level) const override;
        void set(size_t level, node_base* nb) override;

    private:
        std::array<node_base*, _height>  mLinks;
    };


    template <class T, size_t _height>
    struct node : public node_head<_height>
    {
        node(T const& t);
        node(T&& t);
        ~node();

        T& value();
        T const& value() const;

    private:
        char mData[sizeof(T)];
    };


    /**********************************************************/
    /*                  implementations                       */

    /**********************************************************/
    /*                      node_head                         */
    template <size_t _height>
    node_head<_height>::node_head()
    {
        std::fill(std::begin(mLinks), std::end(mLinks), nullptr);
    }

    template <size_t _height>
    node_head<_height>::~node_head()
    {
    }

    template <size_t _height>
    size_t node_head<_height>::height() const
    {
        return _height;
    }

    template <size_t _height>
    node_base* node_head<_height>::next(size_t level) const
    {
        assert(level < mLinks.size() && "Index out of range");
        return mLinks[level];
    }

    template <size_t _height>
    void node_head<_height>::set(size_t level, node_base* nb)
    {
        assert(level < mLinks.size() && "Index out of range");
        mLinks[level] = nb;
    }
    

    /**********************************************************/
    /*                         node                           */
    template <class T, size_t _height>
    node<T, _height>::node(T const& t)
    {
        new (static_cast<void*>(mData))T(t);
    }

    template <class T, size_t _height>
    node<T, _height>::node(T&& t)
    {
        new (static_cast<void*>(mData))T(std::move(t));
    }

    template <class T, size_t _height>
    node<T, _height>::~node()
    {
        static_cast<T*>(static_cast<void*>(mData))->~T();
    }

    template <class T, size_t _height>
    T& node<T, _height>::value()
    {
        return static_cast<T&>(*mData);
    }

    template <class T, size_t _height>
    T const& node<T, _height>::value() const
    {
        return static_cast<T const&>(*mData);
    }


    
    /**********************************************************/
    /*                      skip_list                         */
    template <class T>
    skip_list<T>::skip_list()
        : mHead(new node_head<1>())
    {
    }

    template <class T>
    bool skip_list<T>::insert(T const& t)
    {
    }

    template <class T>
    bool skip_list<T>::remove(T const& t)
    {
    }

    template <class T>
    bool skip_list<T>::contains(T const& t)
    {
    }
} // namespace algic

#endif
