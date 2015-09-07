#ifndef ALGORITHMIC_SKIP_LIST_HPP
#define ALGORITHMIC_SKIP_LIST_HPP
#include <algorithm>
#include <array>
#include <cassert>


namespace algic
{
    using std::size_t;

    template <class T>
    struct node_base
    {
        node_base(T const& t);
        node_base(T&& t);
        virtual ~node_base();

        virtual size_t height() const = 0;
        virtual node_base* next(size_t level) const = 0;
        virtual void set(size_t level, node_base<T>* nb) = 0;

        T& value();
        T const& value() const;

    private:
        char mData[sizeof(T)];
    };


    template <class T, size_t _height>
    struct node : public node_base<T>
    {
        node(T const& t);
        node(T&& t);

        size_t height() const override;
        node_base<T>* next(size_t level) const override;
        void set(size_t level, node_base<T>* nb) override;

    private:
        std::array<node_base<T>*, _height>  mLinks;
    };


    /**********************************************************/
    /*                  implementations                       */

    template <class T>
    node_base<T>::node_base(T const& t)
    {
        new (static_cast<void*>(mData))T(t);
    }

    template <class T>
    node_base<T>::node_base(T&& t)
    {
        new (static_cast<void*>(mData))T(std::move(t));
    }

    template <class T>
    node_base<T>::~node_base()
    {
        static_cast<T*>(static_cast<void*>(mData))->~T();
    }

    template <class T>
    T& node_base<T>::value()
    {
        return static_cast<T&>(*mData);
    }

    template <class T>
    T const& node_base<T>::value() const
    {
        return static_cast<T const&>(*mData);
    }



    template <class T, size_t _height>
    node<T, _height>::node(T const& t)
        : node_base(t)
    {
        std::fill(std::begin(mLinks), std::end(mLinks), nullptr);
    }

    template <class T, size_t _height>
    node<T, _height>::node(T&& t)
        : node_base(std::move(t))
    {
        std::fill(std::begin(mLinks), std::end(mLinks), nullptr);
    }

    template <class T, size_t _height>
    size_t node<T, _height>::height() const
    {
        return _height;
    }

    template <class T, size_t _height>
    node_base<T>* node<T, _height>::next(size_t level) const
    {
        assert(level < mLinks.size() && "Index out of range");
        return mLinks[level];
    }

    template <class T, size_t _height>
    void node<T, _height>::set(size_t level, node_base<T>* nb)
    {
        assert(level < mLinks.size() && "Index out of range");
        mLinks[level] = nb;
    }
} // namespace algic

#endif
