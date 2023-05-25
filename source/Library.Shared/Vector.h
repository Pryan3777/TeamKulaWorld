#pragma once
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include "DefaultEquality.h"
#include "DefaultGrowCapacity.h"

namespace FieaGameEngine {
    /// <summary>
    /// Resizeable collection of elements.
    /// </summary>
    /// <typeparam name="T"> - Type of elements to store.</typeparam>
    template<typename T>
    class Vector final {

    public:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = T;
        using reference = value_type&;
        using const_reference = const value_type&;
        using rvalue_reference = value_type&&;

        /// <summary>
        /// Takes as parameters the vector's current size and then the vector's current capacity (in that order), and then returns what the new capacity should be.
        /// </summary>
        using GrowCapacityFunctorType = std::function<size_type(size_type, size_type)>;

    private:

        /// <summary>
        /// Pointer to the contiguous block of memory which comprises this vector. If this is `nullptr`, the vector should be considered empty.
        /// </summary>
        value_type* _data{nullptr};

        /// <summary>
        /// Capacity this vector has for elements. If `_size == _capacity`, the vector is full and will need to resize itself upon next push.
        /// </summary>
        size_type _capacity{size_type(0)};

        /// <summary>
        /// Amount of elements actually initialized within `_data`. This should always be less than or equal to `_capacity`.
        /// </summary>
        size_type _size{size_type(0)};

        /// <summary>
        /// Strategy this vector employs to gain new capacity when full. Uses the `DefaultGrowCapacity` functor type by default.
        /// </summary>
        GrowCapacityFunctorType _growCapacityFunctor{DefaultGrowCapacity{}};

        /// <summary>
        /// Templated class for iterator to avoid having to make two separate classes for iterator and const_iterator.
        /// </summary>
        template <typename TDeref>
        class __iterator final {

            friend Vector;

        public:
            using difference_type = Vector::difference_type;
            using value_type = TDeref;
            using pointer = TDeref*;
            using reference = TDeref&;
            using iterator_category = std::random_access_iterator_tag;

        private:
            /// <summary>
            /// Index of the element this iterator points to.
            /// </summary>
            size_type _index{size_type(0)};

            /// <summary>
            /// Pointer to the vector this iterator is linked to. This can be `nullptr`; if it is, this iterator was default-constructed and is functionally invalid.
            /// (That is, this iterator will cause failure in most of the algorithms it would be used in.)
            /// </summary>
            Vector* _vector{nullptr};

            /// <summary>
            /// Creates a new random-access iterator.
            /// </summary>
            /// <param name="vector"> - Vector this iterator is linked to.</param>
            /// <param name="index"> - Index this iterator should start at. By default this is 0, meaning this iterator would be equivalent to `begin()`.</param>
            explicit __iterator(Vector& vector, size_type index = size_type(0));

        public:
            /// <summary>
            /// Default constructor. Creates a functionally invalid iterator due to not being assigned a vector. Default construction exists solely for ease of algorithmic writing.
            /// </summary>
            __iterator() = default;

            /// <summary>
            /// Dereference operator. Will throw an exception if this iterator is not linked to a vector or if the iterator is out of its vector's bounds.
            /// </summary>
            /// <returns>Reference to the element this iterator points to.</returns>
            [[nodiscard]] reference operator*() const;

            /// <summary>
            /// Arrow operator. Allows for dereferencing as though the iterator was a pointer.
            /// </summary>
            pointer operator->() const;

            /// <summary>
            /// Pre-increment operator. If this iterator would go out of bounds of its vector's size, it gets set to size instead.
            /// This operator does not guard against std::size_t overflow.
            /// </summary>
            __iterator& operator++();

            /// <summary>
            /// Post-increment operator. If this iterator would go out of bounds of its vector's size, it gets set to size instead.
            /// This operator does not guard against std::size_t overflow.
            /// </summary>
            __iterator operator++(int);

            /// <summary>
            /// Pre-decrement operator. This iterator guards against std::size_t underflow.
            /// </summary>
            __iterator& operator--();

            /// <summary>
            /// Pre-decrement operator. This iterator guards against std::size_t underflow.
            /// </summary>
            __iterator operator--(int);

            /// <summary>
            /// Random-access operator. Sets this iterator's index to the given value, then returns a reference to the value in the vector at that position.
            /// Will throw an exception if this iterator is not linked to a vector or if the iterator is out of its vector's bounds.
            /// </summary>
            reference operator[](size_type);

            /// <summary>
            /// Adds the given value to the index of this iterator. This operation does not guard against std::size_t overflow,
            /// nor does it guard against exceeding the size of this iterator's vector.
            /// </summary>
            __iterator& operator+=(size_type);

            /// <summary>
            /// Subtracts the given value from the index of this iterator. This operation does not guard against std::size_t underflow.
            /// </summary>
            __iterator& operator-=(size_type);

            /// <summary>
            /// Less-than operator. Throws an exception if the given iterator is not associated to the same vector as this iterator.
            /// </summary>
            [[nodiscard]] bool operator<(const __iterator&) const;

            /// <summary>
            /// Less-than-or-equal operator. Throws an exception if the given iterator is not associated to the same vector as this iterator.
            /// </summary>
            [[nodiscard]] bool operator<=(const __iterator&) const;

            /// <summary>
            /// Greater-than operator. Throws an exception if the given iterator is not associated to the same vector as this iterator.
            /// </summary>
            [[nodiscard]] bool operator>(const __iterator&) const;

            /// <summary>
            /// Greater-than-or-equal operator. Throws an exception if the given iterator is not associated to the same vector as this iterator.
            /// </summary>
            [[nodiscard]] bool operator>=(const __iterator&) const;

            /// <summary>
            /// Non-equivalence operator. Two iterators are considered unequivalent if either their linked vectors or their indicies are not the same.
            /// </summary>
            [[nodiscard]] bool operator!=(const __iterator&) const;

            /// <summary>
            /// Equivalence operator. Two iterators are considered equivalent if both their linked vectors and their indicies are the same.
            /// </summary>
            [[nodiscard]] bool operator==(const __iterator&) const;

            /// <summary>
            /// Returns true if this iterator is associated with the given vector, returns false otherwise.
            /// </summary>
            [[nodiscard]] bool IsIteratorFor(const Vector& vector) const;

            /// <returns>Is this iterator able to be dereferenced?</returns>
            [[nodiscard]] bool IsDereferenceable() const;

            /// <summary>
            /// Creates a new iterator offset from the given iterator by `add` indicies.
            /// </summary>
            /// <param name="it"> - Iterator used as the basis for the addition.</param>
            /// <param name="add"> - Offset to add.</param>
            friend inline __iterator operator+(const __iterator& it, size_type add) {
                __iterator result;
                result._vector = it._vector;
                result._index = it._index + add;
                assert(result._index >= it._index);
                return result;
            }

            /// <summary>
            /// Creates a new iterator offset from the given iterator by `sub` indicies.
            /// </summary>
            /// <param name="it"> - Iterator used as the basis for the subtraction.</param>
            /// <param name="add"> - Offset to subtract.</param>
            friend inline __iterator operator-(const __iterator& it, size_type sub) {
                __iterator result;
                result._vector = it._vector;
                result._index = it._index - sub;
                assert(result._index <= it._index);
                return result;
            }

            /// <returns>The difference in signed indicies between the given iterators.</returns>
            friend inline difference_type operator-(const __iterator& first, const __iterator& second) {
                return static_cast<difference_type>(first._index) - static_cast<difference_type>(second._index);
            }
        };

    public:
        /// <summary>
        /// Random-access iterator of the vector. This iterator is non-const.
        /// </summary>
        using iterator = __iterator<value_type>;

        /// <summary>
        /// Random-access iterator of the vector. This iterator is const.
        /// </summary>
        using const_iterator = __iterator<const value_type>;

        /// <summary>
        /// Default constructor.
        /// </summary>
        /// <param name="growCapacity">
        /// - Optional parameter. Strategy this vector will use to allocate new capacity when asked to push while full.
        /// By default this uses the `DefaultGrowCapacity` functor type.
        /// </param>
        Vector(size_type defaultCapacity = size_type(0), GrowCapacityFunctorType growCapacityFunctor = DefaultGrowCapacity{});

        /// <summary>
        /// Creates a vector whose contents will match those of the initializer list. This operation is O(n). (linear-time)
        /// </summary>
        Vector(std::initializer_list<value_type>);

        /// <summary>
        /// Copy constructor. This operation is O(n). (linear-time)
        /// </summary>
        Vector(const Vector&);

        /// <summary>
        /// Move constructor. This operation is O(1). (constant-time)
        /// </summary>
        Vector(Vector&&) noexcept;

        /// <summary>
        /// Copy assignment operator. This operation is O(m+n). (linear-time)
        /// </summary>
        Vector& operator=(const Vector&);

        /// <summary>
        /// Move assignment operator. Does nothing on self-assignment, but that situation should still be avoided. This operation is O(n). (linear-time)
        /// </summary>
        Vector& operator=(Vector&&) noexcept;

        /// <summary>
        /// Destructor. This operation is O(n). (linear-time)
        /// </summary>
        ~Vector();

        /// <summary>
        /// Random-access operator. This operation does not perform bounds checking and is O(1). (constant-time)
        /// </summary>
        [[nodiscard]] reference operator[](size_type);

        /// <summary>
        /// Returns a reference to the element at the given index. This operation performs bounds checking and is O(1). (constant-time)
        /// </summary>
        [[nodiscard]] reference At(size_type);

        /// <summary>
        /// Random-access const operator. This operation does not perform bounds checking and is O(1). (constant-time)
        /// </summary>
        [[nodiscard]] const_reference operator[](size_type) const;

        /// <summary>
        /// Returns a const reference to the element at the given index. This operation performs bounds checking and is O(1). (constant-time)
        /// </summary>
        [[nodiscard]] const_reference At(size_type) const;

        /// <summary>
        /// Returns a const reference to the element at the given index. This operation performs bounds checking and is O(1). (constant-time)
        /// </summary>
        [[nodiscard]] const_reference CAt(size_type) const;

        /// <summary>
        /// Removes the first occurence of the given element from the vector, or does nothing if the element is not in the vector.
        /// This operation is best-case O(1) (constant-time), and is worst-case O(n) (linear-time).
        /// </summary>
        /// <returns>Whether an element was actually removed.</returns>
        bool Remove(const_reference);

        /// <summary>
        /// Removes the first occurence of the given element from the vector, or does nothing if the element is not in the vector.
        /// This operation is best-case O(1) (constant-time), and is worst-case O(n) (linear-time).
        /// </summary>
        /// <param name="element"> - Element to remove.</param>
        /// <param name="compare"> - Functor used to compare like elements. By default, the == operator is used.</param>
        /// <returns>Whether an element was actually removed.</returns>
        template <typename ComparisonFunctor = DefaultEquality<T>> bool Remove(const_reference element, ComparisonFunctor compare = ComparisonFunctor{});

        /// <summary>
        /// Removes the element at the given position from the vector, or does nothing if the iterator is not within the bounds of the vector.
        /// This operation is best-case O(1) (constant-time), and is worst-case O(n) (linear-time).
        /// </summary>
        /// <returns>Whether an element was actually removed.</returns>
        bool Remove(iterator);

        /// <summary>
        /// Removes each element in the given range from the vector. If `finishExclusive` is outside the bounds of the vector, it is set to be within the bounds.
        /// This operator is O(n). (linear-time)
        /// </summary>
        /// <param name="startInclusive"> - Start of the range. Inclusive, so this element will be removed.</param>
        /// <param name="finishExclusive"> - End of the range. Exclusive, so this element will not be removed, and in fact could be outside the bounds of the vector.</param>
        /// <returns>Whether an element was actually removed.</returns>
        bool Remove(iterator startInclusive, iterator finishExclusive);

        /// <summary>
        /// Removes the element at the given position from the vector, or does nothing if the iterator is not within the bounds of the vector.
        /// This operation is best-case O(1) (constant-time), and is worst-case O(n) (linear-time).
        /// </summary>
        /// <returns>Whether an element was actually removed.</returns>
        bool RemoveAt(size_type index);

        /// <summary>
        /// Clears the vector, meaning it finalizes all elements within the vector and sets `size` to 0.
        /// Does not set `capacity` to 0, nor does it free the memory captured by the vector.
        /// This operation is O(n). (linear-time)
        /// </summary>
        void Clear();

        /// <summary>
        /// Reduces capacity and frees memory to minimally encapsulate the elements initialized within the vector. In short, makes `capacity` equal `size`.
        /// This operation is O(n). (linear-time)
        /// </summary>
        /// <param name="minCapacity">
        /// If the capacity were to be reduced, it will go no lower than this value.
        /// If the capacity is already smaller than this value, capacity will not be increased.
        /// </param>
        void ShrinkToFit(size_type minCapacity = size_type(0));

        /// <summary>
        /// Sets `capacity` equal to the given value and then allocates memory. If the given value is less than the current capacity, the method early-exits and nothing happens.
        /// This operation is O(n). (linear-time)
        /// </summary>
        void Reserve(size_type capacity);

        /// <summary>
        /// Adds the given element to the end of the list.
        /// </summary>
        /// <returns>Iterator set at the position of the newly inserted element.</returns>
        iterator PushBack(const_reference element);

        /// <summary>
        /// Adds the given element to the end of the list.
        /// </summary>
        /// <returns>Iterator set at the position of the newly inserted element.</returns>
        iterator PushBack(rvalue_reference element);

        /// <summary>
        /// Uses the variadic template to create a new element in the vector in-place.
        /// </summary>
        /// <typeparam name="...Args"> - Variadic template.</typeparam>
        /// <param name="...args"> - Variadic template.</param>
        /// <returns>Iterator set at the position of the newly inserted element.</returns>
        template <typename... Args> iterator EmplaceBack(Args&&... args);

        /// <summary>
        /// Removes the last element from the vector, or does nothing if the vector is empty.
        /// This operation is O(1). (constant-time)
        /// </summary>
        void PopBack();

        /// <summary>
        /// This operation is O(1). (constant-time)
        /// </summary>
        /// <returns>`true` if the vector is empty, false otherwise.</returns>
        [[nodiscard]] bool IsEmpty() const;

        /// <returns>Number of elements initialized within the vector. Should always be less than `capacity`.</returns>
        [[nodiscard]] size_type Size() const;

        /// <returns>Amount of space allocated by this vector to store elements, in terms of indicies.</returns>
        [[nodiscard]] size_type Capacity() const;

        /// <returns>Reference to the first element in the vector. This will throw an exception if the vector is empty.</returns>
        [[nodiscard]] reference Front();

        /// <returns>const reference to the first element in the vector. This will throw an exception if the vector is empty.</returns>
        [[nodiscard]] const_reference Front() const;

        /// <returns>const reference to the first element in the vector. This will throw an exception if the vector is empty.</returns>
        [[nodiscard]] const_reference CFront() const;

        /// <summary>
        /// This operation is O(1). (constant-time)
        /// </summary>
        /// <returns>Reference to the last element in the vector. This will throw an exception if the vector is empty.</returns>
        [[nodiscard]] reference Back();

        /// <summary>
        /// This operation is O(1). (constant-time)
        /// </summary>
        /// <returns>const reference to the last element in the vector. This will throw an exception if the vector is empty.</returns>
        [[nodiscard]] const_reference Back() const;

        /// <summary>
        /// This operation is O(1). (constant-time)
        /// </summary>
        /// <returns>const reference to the last element in the vector. This will throw an exception if the vector is empty.</returns>
        [[nodiscard]] const_reference CBack() const;

        /// <summary>
        /// Returns an iterator to the first occurence of the given element in the vector, or `end()` if no such element exists within the vector.
        /// This operation is O(n). (linear-time)
        /// </summary>
        /// <param name="element"> - Element to find.</param>
        /// <param name="compare"> - Functor used to compare like elements. By default, the == operator is used.</param>
        template <typename ComparisonFunctor = DefaultEquality<T>> [[nodiscard]] iterator Find(const_reference element, ComparisonFunctor compare = ComparisonFunctor{});

        /// <summary>
        /// Returns an iterator to the first occurence of the given element in the vector, or `cend()` if no such element exists within the vector.
        /// Dereference operations using this iterator will return const references.
        /// This operation is O(n). (linear-time)
        /// </summary>
        /// <param name="element"> - Element to find.</param>
        /// <param name="compare"> - Functor used to compare like elements. By default, the == operator is used.</param>
        template <typename ComparisonFunctor = DefaultEquality<T>> [[nodiscard]] const_iterator Find(const_reference element, ComparisonFunctor compare = ComparisonFunctor{}) const;

        /// <summary>
        /// Returns an iterator to the first occurence of the given element in the vector, or `cend()` if no such element exists within the vector.
        /// Dereference operations using this iterator will return const references.
        /// This operation is O(n). (linear-time)
        /// </summary>
        /// <param name="element"> - Element to find.</param>
        /// <param name="compare"> - Functor used to compare like elements. By default, the == operator is used.</param>
        template <typename ComparisonFunctor = DefaultEquality<T>> [[nodiscard]] const_iterator CFind(const_reference element, ComparisonFunctor compare = ComparisonFunctor{}) const;

        /// <returns>Iterator to the beginning of this vector.</returns>
        [[nodiscard]] iterator begin();

        /// <returns>Iterator to the beginning of this vector. Dereference operations using this iterator will return const references.</returns>
        [[nodiscard]] const_iterator begin() const;

        /// <returns>Iterator to the beginning of this vector. Dereference operations using this iterator will return const references.</returns>
        [[nodiscard]] const_iterator cbegin() const;

        /// <returns>Iterator to the end of this vector. This iterator is outside the bounds of the vector.</returns>
        [[nodiscard]] iterator end();

        /// <returns>Iterator to the end of this vector. This iterator is outside the bounds of the vector.</returns>
        [[nodiscard]] const_iterator end() const;

        /// <returns>Iterator to the end of this vector. This iterator is outside the bounds of the vector.</returns>
        [[nodiscard]] const_iterator cend() const;

        /// <summary>
        /// Swaps the contents of this vector with that of the given vector.
        /// </summary>
        void swap(Vector& other);

        /// <returns>Functor that is currently used whenever this vector needs to allocate new space when pushing while full.</returns>
        [[nodiscard]] const GrowCapacityFunctorType& GetGrowCapacityFunctor() const;

        /// <summary>
        /// The given functor will be used to allocate space whenever this vector needs to allocate new space when pushing while full.
        /// If the functor returns a value less than the current capacity, capacity will still increment by 1.
        /// </summary>
        void SetGrowCapacityFunctor(GrowCapacityFunctorType);

    };
}

#include "Vector.inl"
