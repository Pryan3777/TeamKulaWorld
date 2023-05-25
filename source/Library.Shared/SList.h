#pragma once

#include <cassert>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include "DefaultEquality.h"

namespace FieaGameEngine {
    /// <summary>
    /// Singly-linked list.
    /// </summary>
    template <typename T>
    class SList final {

    public:
        using size_type = std::size_t;
        using value_type = T;
        using reference = value_type&;
        using const_reference = const value_type&;
        using rvalue_reference = value_type&&;

    private:
        /// <summary>
        /// Node in the singly-linked list.
        /// </summary>
        struct Node final {

            /// <summary>
            /// Data contained within the node.
            /// </summary>
            value_type _data;

            /// <summary>
            /// Pointer to the next node sequentially in the list. This can be `nullptr`; if it is, this *should be* the last node of the list.
            /// </summary>
            Node* _next{nullptr};

            /// <summary>
            /// Creates a new node.
            /// </summary>
            /// <param name="data"> - Data contained within the node. This constructor makes a copy of this data.</param>
            /// <param name="next"> - Optional pointer to the next node sequentially in the list.</param>
            explicit Node(const_reference data, Node* next = nullptr);

            /// <summary>
            /// Creates a new node, using the variadic template argument to construct the data in-place rather than copy an existing element.
            /// </summary>
            /// <typeparam name="...Args"> - Variadic template.</typeparam>
            /// <param name="next"> - Optional pointer to the next node sequentially in the list.</param>
            /// <param name="...args"> - Variadic template.</param>
            template <typename... Args> Node(Node* next, Args&&... args);

        };

        /// <summary>
        /// Forward iterator of the singly-linked list.
        /// </summary>
        template <typename TDeref>
        class __iterator final {

            friend SList;

        public:
            using difference_type = std::ptrdiff_t;
            using value_type = TDeref;
            using pointer = TDeref*;
            using reference = TDeref&;
            using iterator_category = std::forward_iterator_tag;

        private:
            /// <summary>
            /// Node of the singly-linked list this iterator points to. This can be `nullptr`; if it is, this iterator is equivalent to `end()`.
            /// </summary>
            Node* _node{nullptr};

            /// <summary>
            /// Pointer to the list this iterator is linked to. This can be `nullptr`; if it is, this iterator was default-constructed and is functionally invalid.
            /// (That is, this iterator will cause failure in most of the algorithms it would be used in.)
            /// </summary>
            SList* _list{nullptr};

            /// <summary>
            /// Creates a new forward iterator.
            /// </summary>
            /// <param name="list">List this iterator is linked to.</param>
            /// <param name="node">Optional pointer to the node this iterator should start at. By default this is `nullptr`, meaning this iterator would be equivalent to `end()`.</param>
            explicit __iterator(SList& list, Node* node = nullptr);

        public:
            /// <summary>
            /// Default constructor. Creates a functionally invalid iterator due to not being assigned a list. Default construction exists solely for ease of algorithmic writing.
            /// </summary>
            __iterator() = default;

            reference operator*() const;
            pointer operator->() const;
            __iterator& operator++();
            __iterator operator++(int);
            [[nodiscard]] bool operator!=(const __iterator&) const;
            [[nodiscard]] bool operator==(const __iterator&) const;

            [[nodiscard]] bool IsIteratorFor(const SList& list) const;

            /// <returns>Is this iterator able to be dereferenced?</returns>
            [[nodiscard]] bool IsDereferenceable() const;

        };

        /// <summary>
        /// First node of the list. This can be `nullptr`; if it is, this list *should be* empty.
        /// </summary>
        Node* _front{nullptr};

        /// <summary>
        /// Last node of the list. This can be `nullptr`; if it is, this list *should be* empty.
        /// </summary>
        Node* _back{nullptr};

        /// <summary>
        /// The number of elements contained within the list.
        /// </summary>
        size_type _size{0};

    public:
        /// <summary>
        /// Forward iterator of the vector. This iterator is non-const.
        /// </summary>
        using iterator = __iterator<value_type>;

        /// <summary>
        /// Forward iterator of the vector. This iterator is const.
        /// </summary>
        using const_iterator = __iterator<const value_type>;

        /// <summary>
        /// Default constructor. Creates an empty list (`Size` == 0) with `Front` and `Back` both pointing to `nullptr`.
        /// </summary>
        SList() = default;

        /// <summary>
        /// Destructor. This clears the list, which is an O(n) operation. (linear-time)
        /// </summary>
        ~SList();

        /// <summary>
        /// Creates a singly-linked list whose contents will match those of the initializer list. This operation is O(n). (linear-time)
        /// </summary>
        SList(std::initializer_list<value_type>);

        /// <summary>
        /// Copy constructor. This operation is O(n). (linear-time)
        /// </summary>
        SList(const SList&);

        /// <summary>
        /// Copy assignment operator. This operation is O(n). (linear-time)
        /// </summary>
        SList& operator=(const SList&);

        /// <summary>
        /// Move constructor. This operation is O(1). (constant-time)
        /// </summary>
        SList(SList&&) noexcept;

        /// <summary>
        /// Move assignment operator. This operation is O(1). (constant-time)
        /// </summary>
        SList& operator=(SList&&) noexcept;

        /// <summary>
        /// Empties the list and destroys all nodes. This operation is O(n). (linear-time)
        /// </summary>
        void Clear();

        /// <summary>
        /// Removes the last element of the list, then updates the `Back` pointer to the new last element. If the list is already empty, nothing happens.
        /// This operation is O(n). (linear-time)
        /// </summary>
        void PopBack();

        /// <summary>
        /// Removes the first element of the list, then updates the `Front` pointer to the new first element. If the list is already empty, nothing happens.
        /// This operation is O(1). (constant-time)
        /// </summary>
        void PopFront();

        /// <summary>
        /// Adds the given element to the back of the list, thereby making the new element the new `Back`. This operation is O(1). (constant-time)
        /// </summary>
        iterator PushBack(const_reference);

        /// <summary>
        /// Adds the given element to the back of the list, thereby making the new element the new `Back`. This operation is O(1). (constant-time)
        /// </summary>
        iterator PushBack(rvalue_reference);

        /// <summary>
        /// Adds the given element to the front of the list, thereby making the new element the new `Front`. This operation is O(1). (constant-time)
        /// </summary>
        iterator PushFront(const_reference);

        /// <summary>
        /// Adds the given element to the front of the list, thereby making the new element the new `Front`. This operation is O(1). (constant-time)
        /// </summary>
        iterator PushFront(rvalue_reference);

        /// <summary>
        /// Constructs an element in-place to the back of the list, thereby making the new element the new `Back`. This operation is O(1). (constant-time)
        /// </summary>
        template <typename... Args> iterator EmplaceBack(Args&&... args);

        /// <summary>
        /// Constructs an element in-place to the front of the list, thereby making the new element the new `Front`. This operation is O(1). (constant-time)
        /// </summary>
        template <typename... Args> iterator EmplaceFront(Args&&... args);

        /// <summary>
        /// Returns whether the list is empty.
        /// </summary>
        [[nodiscard]] bool IsEmpty() const;

        /// <summary>
        /// Returns the number of elements in the list. This operation is O(1). (constant-time)
        /// </summary>
        [[nodiscard]] size_type Size() const;

        /// <summary>
        /// Returns a mutable reference to the data stored in the first element of the list.
        /// If the list is empty, an `out_of_range` exception will be thrown.
        /// </summary>
        [[nodiscard]] reference Front();

        /// <summary>
        /// Returns a mutable reference to the data stored in the last element of the list.
        /// If the list is empty, an `out_of_range` exception will be thrown.
        /// </summary>
        [[nodiscard]] reference Back();

        /// <summary>
        /// Returns an immutable reference to the data stored in the first element of the list.
        /// If the list is empty, an `out_of_range` exception will be thrown.
        /// </summary>
        [[nodiscard]] const_reference Front() const;

        /// <summary>
        /// Returns an immutable reference to the data stored in the last element of the list.
        /// If the list is empty, an `out_of_range` exception will be thrown.
        /// </summary>
        [[nodiscard]] const_reference Back() const;

        /// <summary>
        /// Returns an iterator to the position in the list which contains the given element.
        /// </summary>
        /// <param name="compare"> - `bool(const_reference lhs, const_reference rhs)` - Comparison functor used to determine if the element has been found.</param>
        template <typename ComparisonFunctor = DefaultEquality<T>> [[nodiscard]] iterator Find(const_reference element, ComparisonFunctor compare = ComparisonFunctor{});

        /// <summary>
        /// Returns a const iterator to the position in the list which contains the given element.
        /// </summary>
        /// <param name="compare"> - `bool(const_reference lhs, const_reference rhs)` - Comparison functor used to determine if the element has been found.</param>
        template <typename ComparisonFunctor = DefaultEquality<T>> [[nodiscard]] const_iterator Find(const_reference element, ComparisonFunctor compare = ComparisonFunctor{}) const;

        /// <summary>
        /// Returns a const iterator to the position in the list which contains the given element.
        /// </summary>
        /// <param name="compare"> - `bool(const_reference lhs, const_reference rhs)` - Comparison functor used to determine if the element has been found.</param>
        template <typename ComparisonFunctor = DefaultEquality<T>> [[nodiscard]] const_iterator CFind(const_reference element, ComparisonFunctor compare = ComparisonFunctor{}) const;

        /// <summary>
        /// Returns an iterator to the beginning of the list.
        /// </summary>
        [[nodiscard]] iterator begin();

        /// <summary>
        /// Returns an iterator to the end of the list. Per convention, this iterator does not point to a valid element in the list, and so shouldn't be dereferenced.
        /// </summary>
        [[nodiscard]] iterator end();

        /// <summary>
        /// Returns a const iterator to the beginning of the list.
        /// </summary>
        [[nodiscard]] const_iterator begin() const;

        /// <summary>
        /// Returns a const iterator to the end of the list. Per convention, this iterator does not point to a valid element in the list, and so shouldn't be dereferenced.
        /// </summary>
        [[nodiscard]] const_iterator end() const;

        /// <summary>
        /// Returns a const iterator to the beginning of the list.
        /// </summary>
        [[nodiscard]] const_iterator cbegin() const;

        /// <summary>
        /// Returns a const iterator to the end of the list. Per convention, this iterator does not point to a valid element in the list, and so shouldn't be dereferenced.
        /// </summary>
        [[nodiscard]] const_iterator cend() const;

        /// <summary>
        /// Inserts the given data into the list, then returns an iterator set to the position of the new element. This operation is O(1). (constant-time)
        /// </summary>
        iterator InsertAfter(const iterator& position, const_reference data);

        /// <summary>
        /// Inserts the given data into the list, then returns an iterator set to the position of the new element. This operation is O(1). (constant-time)
        /// </summary>
        iterator InsertAfter(const iterator& position, rvalue_reference data);

        /// <summary>
        /// Constructs the given data in-place into the list, then returns an iterator set to the position of the new element. This operation is O(1). (constant-time)
        /// </summary>
        template <typename... Args> iterator EmplaceAfter(const iterator& position, Args&&... args);

        /// <summary>
        /// Removes the first occurence of the given element from the list, or does nothing if that element is not within the list. This operation is O(n). (linear-time)
        /// </summary>
        /// <returns>Whether an object was actually removed.</returns>
        bool Remove(const_reference);

        /// <summary>
        /// Removes the element at the given position. This operation is O(1) unless the iterator is pointing to `Back`, then the operation is O(n).
        /// </summary>
        /// <returns>Whether an object was actually removed.</returns>s
        bool Remove(const iterator&);

        /// <summary>
        /// Swaps the contents of this list with the other.
        /// </summary>
        void swap(SList& other);

    };
}

#include "SList.inl"
