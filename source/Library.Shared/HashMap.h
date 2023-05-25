#pragma once
#include <cstdint>
#include <functional>
#include "DefaultEmplaceDefault.h"
#include "DefaultEquality.h"
#include "DefaultHash.h"
#include "SList.h"
#include "Vector.h"

namespace FieaGameEngine {
    /// <summary>
    /// Unordered hash map.
    /// </summary>
    /// <typeparam name="TKey">Key type.</typeparam>
    /// <typeparam name="TData">Mapped type.</typeparam>
    template <typename TKey, typename TData>
    class HashMap final {

    public:
        /// <summary>
        /// Key type.
        /// </summary>
        using key_type = TKey;

        /// <summary>
        /// Mapped type.
        /// </summary>
        using mapped_type = TData;

        /// <summary>
        /// Key-value-pair type stored in this map.
        /// </summary>
        using value_type = std::pair<const key_type, mapped_type>;
        using reference = value_type&;
        using const_reference = const value_type&;
        using rvalue_reference = value_type&&;

        /// <summary>
        /// Functor type used for evaluating key equivalence.
        /// </summary>
        using key_equal = std::function<bool(const key_type&, const key_type&)>;

        /// <summary>
        /// Functor type used for emplacing elements without constructor parameters.
        /// </summary>
        using emplace_default = std::function<TData(void)>;

    private:
        using chain_type = SList<value_type>;
        using array_type = Vector<chain_type>;

    public:
        using difference_type = typename array_type::difference_type;
        using size_type = typename array_type::size_type;

        /// <summary>
        /// Functor type used for converting keys into hash values.
        /// </summary>
        using hasher = std::function<size_type(const key_type&)>;

        /// <summary>
        /// Functor type used for rehashing.
        /// </summary>
        using rehash = std::function<size_type(size_type)>;

        /// <summary>
        /// Functor type used for determining if rehashing should occur.
        /// </summary>
        using rehash_if = std::function<bool(size_type, size_type)>;

        /// <summary>
        /// Default max hash value.
        /// </summary>
        static inline constexpr size_type DEFAULT_MAX_HASH_VALUE = size_type(251);

        /// <summary>
        /// Functor struct type which implements default behavior for rehashing.
        /// </summary>
        struct DefaultRehash final {
            [[nodiscard]] size_type operator()(size_type currentMaxHashValue) const;
        };

        /// <summary>
        /// Functor struct type which implements default behavior for determining if rehashing should occur.
        /// </summary>
        struct DefaultRehashIf final {
            [[nodiscard]] bool operator()(size_type currentSize, size_type currentMaxHashValue) const;
        };

    private:
        /// <summary>
        /// Private templated iterator class to implement behavior for const and nonconst iteration.
        /// </summary>
        template <typename TDeref>
        class __iterator final {

            friend HashMap;

        public:
            using difference_type = HashMap::difference_type;
            using value_type = TDeref;
            using pointer = value_type*;
            using reference = value_type&;
            using iterator_category = std::forward_iterator_tag;

        private:
            typename array_type::iterator _arrayItr{typename array_type::iterator{}};
            typename chain_type::iterator _chainItr{typename chain_type::iterator{}};
            HashMap* _map{nullptr};

            explicit __iterator(HashMap& map, typename array_type::iterator arrayItr, typename chain_type::iterator chainItr);

        public:
            __iterator() = default;

            [[nodiscard]] reference operator*() const;
            pointer operator->() const;
            [[nodiscard]] bool operator!=(const __iterator&) const;
            [[nodiscard]] bool operator==(const __iterator&) const;

            /// <summary>
            /// Increments this operator. This will find the next key-value pair, which may not necessarily be adjacent to the current position.
            /// In other words, this operation is worst-case O(n). (linear-time)
            /// </summary>
            __iterator& operator++();

            /// <summary>
            /// Increments this operator. This will find the next key-value pair, which may not necessarily be adjacent to the current position.
            /// In other words, this operation is worst-case O(n). (linear-time)
            /// </summary>
            __iterator operator++(int);

            /// <summary>
            /// Is this iterator associated with the given map?
            /// </summary>
            [[nodiscard]] bool IsIteratorFor(const HashMap& map) const;

            /// <returns>Is this iterator able to be dereferenced?</returns>
            [[nodiscard]] bool IsDereferenceable() const;

        };

        friend __iterator;

        array_type _chains;
        size_type _size;
        size_type _usedHashValueCount;
        emplace_default _emplaceDefaultFunctor;
        hasher _hashFunctor;
        key_equal _keyCompareFunctor;

        [[nodiscard]] bool IsContainingKey(const key_type& key, size_type& hash) const;

    public:
        /// <summary>
        /// Forward iterator for hash map. This iterator is non-const.
        /// </summary>
        using iterator = __iterator<value_type>;

        /// <summary>
        /// Forward iterator for hash map. This iterator is const.
        /// </summary>
        using const_iterator = __iterator<const value_type>;

    private:
        /// <summary>
        /// Inserts the given element into the indicated chain and default-emplaces it using the functor.
        /// </summary>
        iterator PushBackDefaulted(size_type chainIndex, const key_type& key);

        /// <summary>
        /// Inserts the given element into the indicated chain.
        /// </summary>
        iterator PushBack(size_type chainIndex, const_reference pair);

        /// <summary>
        /// Inserts the given element into the indicated chain.
        /// </summary>
        iterator PushBack(size_type chainIndex, rvalue_reference pair);

        /// <summary>
        /// Finds an element with the given key, or returns `end()` if no such element exists in this map.
        /// This operation is worst-case O(n). (linear-time)
        /// </summary>
        /// <param name="hash"> - reference parameter used to pass along the hash calculated in this method.</param>
        [[nodiscard]] iterator Find(const key_type& key, size_type& hash);

        /// <summary>
        /// Finds an element with the given key, or returns `cend()` if no such element exists in this map.
        /// This operation is worst-case O(n). (linear-time)
        /// </summary>
        /// <param name="hash"> - reference parameter used to pass along the hash calculated in this method.</param>
        [[nodiscard]] const_iterator CFind(const key_type& key, size_type& hash) const;

    public:
        /// <summary>
        /// Constructor allowing easy setting of the emplace default functor.
        /// </summary>
        HashMap(size_type maxHashValue, const emplace_default& emplaceDefaultFunctor);

        /// <summary>
        /// Constructor allowing easy setting of the key comparison functor.
        /// </summary>
        HashMap(size_type maxHashValue, const key_equal& keyCompareFunctor, emplace_default emplaceDefaultFunctor = DefaultEmplaceDefault<TData>{});

        /// <summary>
        /// Default constructor.
        /// </summary>
        /// <param name="maxHashValue"> - Max value a hash code will produce within the map.</param>
        /// <param name="hashFunctor"> - Functor implementation of the hash function.</param>
        /// <param name="keyCompareFunctor"> - Functor implementation of the key comparison function.</param>
        /// <param name="emplaceDefaultFunctor"> - Functor implementation of the emplace default function.</param>
        explicit HashMap(
            size_type maxHashValue = DEFAULT_MAX_HASH_VALUE,
            hasher hashFunctor = DefaultHash<TKey>{},
            key_equal keyCompareFunctor = DefaultEquality<TKey>{},
            emplace_default emplaceDefaultFunctor = DefaultEmplaceDefault<TData>{}
        );

        /// <summary>
        /// Constructor using an initializer list to create the hash map. Note, if entries with duplicate keys are given,
        /// only the first entry will end up in the list and all other entries will be ignored.
        /// </summary>
        explicit HashMap(std::initializer_list<value_type>);

        /// <summary>
        /// Move constructor.
        /// </summary>
        HashMap(HashMap&&) noexcept;

        /// <summary>
        /// Move assignment operator.
        /// </summary>
        HashMap& operator=(HashMap&&) noexcept;

        HashMap(const HashMap&) = default;
        HashMap& operator=(const HashMap&) = default;
        ~HashMap() = default;

        /// <summary>
        /// Assigns the hash map using an initializer list. Note, if entries with duplicate keys are given,
        /// only the first entry will end up in the list and all other entries will be ignored.
        /// </summary>
        HashMap& operator=(std::initializer_list<value_type>);

        /// <summary>
        /// Returns a reference to the element paired with the given key, or creates the element using the emplace default functor
        /// if no such element exists.
        /// </summary>
        mapped_type& operator[](const key_type&);

        /// <summary>
        /// Returns a reference to the element paired with the given key, or creates the element using the emplace default functor
        /// if no such element exists.
        /// </summary>
        const mapped_type& operator[](const key_type&) const;

        /// <summary>
        /// Inserts the pair into the map. If an element with the given key already exists, does nothing.
        /// </summary>
        iterator Insert(const_reference);

        /// <summary>
        /// Inserts the pair into the map. If an element with the given key already exists, does nothing.
        /// </summary>
        iterator Insert(rvalue_reference);

        /// <summary>
        /// Inserts the pair into the map. If an element with the given key already exists, the previous value will be overriden.
        /// </summary>
        iterator InsertOrAssign(const_reference);

        /// <summary>
        /// Inserts the pair into the map. If an element with the given key already exists, the previous value will be overriden.
        /// </summary>
        iterator InsertOrAssign(rvalue_reference);

        /// <summary>
        /// Removes the pair with the given key from the map. If no such element exists, does nothing.
        /// </summary>
        /// <returns>If a remove occured.</returns>
        bool Remove(const key_type&);

        /// <summary>
        /// Removes the element at the given position.
        /// </summary>
        /// <returns>If a remove occured.</returns>
        bool Remove(const iterator& position);

        /// <summary>
        /// Removes each element within the range defined by the two iterators. `startInclusive` is inclusive, meaning that element will be removed.
        /// `finishExclusive` is exclusive, meaning that element will not be removed, nor will it even be checked for a value or dereferenced.
        /// </summary>
        /// <returns>If a remove occured.</returns>
        bool Remove(const iterator& startInclusive, const iterator& finishExclusive);

        /// <summary>
        /// Clears the map. Does not rehash the map.
        /// </summary>
        void Clear();

        /// <summary>
        /// Rehashes the map based on the "if" functor. Returns true if a rehash occurs, false otherwise.
        /// </summary>
        /// <param name="rehashFunctor"> - functor used to determine the new max hash value.</param>
        /// <param name="rehashIfFunctor"> - if this functor returns true, a rehash will occur. Otherwise, nothing happens.</param>
        bool RehashIfNeeded(rehash rehashFunctor = DefaultRehash{}, rehash_if rehashIfFunctor = DefaultRehashIf{});

        /// <summary>
        /// Forces a rehash to occur, using the functor to determine the new max hash value.
        /// </summary>
        void ForceRehash(rehash rehashFunctor = DefaultRehash{});

        /// <summary>
        /// Forces a rehash to occur, setting the new max hash value to the given value.
        /// </summary>
        void ForceRehash(size_type newMaxHashValue);

        /// <summary>
        /// Returns true if the map is empty, false otherwise.
        /// </summary>
        [[nodiscard]] bool IsEmpty() const;

        /// <summary>
        /// Returns the number of key-value pairs currently stored within the map.
        /// </summary>
        [[nodiscard]] size_type Size() const;

        /// <summary>
        /// Returns the maximum hash code which can be returned within the map. This value increases after a rehash
        /// and by default will always be a prime number.
        /// </summary>
        [[nodiscard]] size_type MaxHashValue() const;

        /// <summary>
        /// Returns the number of hash codes which are currently in use within the map. Used to calculate load factor.
        /// </summary>
        [[nodiscard]] size_type UsedHashValueCount() const;

        /// <summary>
        /// Returns as a percentage between 0.0 and 1.0 how many of the possible hash codes are in use within the map.
        /// </summary>
        [[nodiscard]] double LoadFactor() const;

        /// <summary>
        /// Returns true if an element in the map is paired with the given key. Returns false otherwise.
        /// This operation is worst-case O(n). (linear-time)
        /// </summary>
        [[nodiscard]] bool IsContainingKey(const key_type&) const;

        /// <summary>
        /// Returns a reference to the element mapped to the given key, or throws an exception if no such element exists.
        /// This operation is worst-case O(n). (linear-time)
        /// </summary>
        [[nodiscard]] mapped_type& At(const key_type&);

        /// <summary>
        /// Returns a reference to the element mapped to the given key, or throws an exception if no such element exists.
        /// This operation is worst-case O(n). (linear-time)
        /// </summary>
        [[nodiscard]] const mapped_type& At(const key_type&) const;

        /// <summary>
        /// Returns a reference to the element mapped to the given key, or throws an exception if no such element exists.
        /// This operation is worst-case O(n). (linear-time)
        /// </summary>
        [[nodiscard]] const mapped_type& CAt(const key_type&) const;

        /// <summary>
        /// Finds the given element in the map, or returns `end()` if no such element exists.
        /// This operation is worst-case O(n). (linear-time)
        /// </summary>
        [[nodiscard]] iterator Find(const key_type&);

        /// <summary>
        /// Finds the given element in the map, or returns `cend()` if no such element exists.
        /// This operation is worst-case O(n). (linear-time)
        /// </summary>
        [[nodiscard]] const_iterator Find(const key_type&) const;

        /// <summary>
        /// Finds the given element in the map, or returns `cend()` if no such element exists.
        /// This operation is worst-case O(n). (linear-time)
        /// </summary>
        [[nodiscard]] const_iterator CFind(const key_type&) const;

        /// <summary>
        /// Creates a nonconst forward iterator at the beginning of this hash map. Order of elements is not guaranteed
        /// and should effectively be considered random. This operation is worst-case O(n). (linear-time)
        /// </summary>
        [[nodiscard]] iterator begin();

        /// <summary>
        /// Creates a const forward iterator at the beginning of this hash map. Order of elements is not guaranteed
        /// and should effectively be considered random. This operation is worst-case O(n). (linear-time)
        /// </summary>
        [[nodiscard]] const_iterator begin() const;

        /// <summary>
        /// Creates a const forward iterator at the beginning of this hash map. Order of elements is not guaranteed
        /// and should effectively be considered random. This operation is worst-case O(n). (linear-time)
        /// </summary>
        [[nodiscard]] const_iterator cbegin() const;

        /// <summary>
        /// Creates a nonconst forward iterator to the end of this hash map. This should only be used for
        /// comparison to other iterators, as dereferencing will result in an exception. This operation is O(1). (constant-time)
        /// </summary>
        [[nodiscard]] iterator end();

        /// <summary>
        /// Creates a const forward iterator to the end of this hash map. This should only be used for
        /// comparison to other iterators, as dereferencing will result in an exception. This operation is O(1). (constant-time)
        /// </summary>
        [[nodiscard]] const_iterator end() const;

        /// <summary>
        /// Creates a const forward iterator to the end of this hash map. This should only be used for
        /// comparison to other iterators, as dereferencing will result in an exception. This operation is O(1). (constant-time)
        /// </summary>
        [[nodiscard]] const_iterator cend() const;

        /// <summary>
        /// Swaps the contents of this hash map with the other hash map.
        /// </summary>
        void swap(HashMap& other);

        [[nodiscard]] hasher GetHashFunctor() const;
        [[nodiscard]] const key_equal& GetKeyCompareFunctor() const;
        [[nodiscard]] const emplace_default& GetEmplaceDefaultFunctor() const;
        void SetEmplaceDefaultFunctor(const emplace_default&);

    };
}

#include "HashMap.inl"
