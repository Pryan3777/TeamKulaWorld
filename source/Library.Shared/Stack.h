#pragma once
#include "SList.h"

namespace FieaGameEngine {
    template <typename T>
    class Stack final {

    private:
        SList<T> _slist{};

    public:
        using iterator = typename decltype(_slist)::iterator;
        using const_iterator = typename decltype(_slist)::const_iterator;
        using size_type = typename decltype(_slist)::size_type;
        using value_type = typename decltype(_slist)::value_type;
        using reference = value_type&;
        using const_reference = const value_type&;
        using rvalue_reference = value_type&&;

        Stack() = default;
        inline Stack(std::initializer_list<T> list) : _slist{list} {}

        inline bool IsEmpty() const { return _slist.IsEmpty(); }
        inline size_type Size() const { return _slist.Size(); }
        inline void Clear() { _slist.Clear(); }
        inline iterator Top() { return begin(); }
        inline const_iterator CTop() const { return cbegin(); }
        inline const_iterator Top() const { return CTop(); }
        inline iterator Push(const_reference value) { return _slist.PushFront(value); }
        inline iterator Push(rvalue_reference value) { return _slist.PushFront(std::forward<value_type>(value)); }
        template <typename... Args> inline iterator Emplace(Args&&... args) { return _slist.EmplaceFront(std::forward<Args>(args)...); }
        inline void Pop() { _slist.PopFront(); }

        inline iterator begin() { return _slist.begin(); }
        inline const_iterator cbegin() const { return _slist.cbegin(); }
        inline const_iterator begin() const { return cbegin(); }
        inline iterator end() { return _slist.end(); }
        inline const_iterator cend() const { return _slist.cend(); }
        inline const_iterator end() const { return cend(); }

        inline void swap(Stack& other) { _slist.swap(other._slist); }

    };

    template <typename T> inline void swap(Stack<T>& lhs, Stack<T>& rhs) { lhs.swap(rhs); }
}
