#pragma once
#include "HashMap.h"

#ifndef UNREFERENCED_LOCAL
#define UNREFERENCED_LOCAL(L) (L)
#endif

namespace FieaGameEngine {
#pragma region DefaultRehash
    /*
     * region DefaultRehash
     */

    template <typename TKey, typename TData>
    inline typename HashMap<TKey, TData>::size_type HashMap<TKey, TData>::DefaultRehash::operator()(size_type currentMaxHashValue) const {
        size_type grown;
        size_type min = currentMaxHashValue + size_type(2);

        // Find the next prime number.
        for (grown = size_type(1) + currentMaxHashValue + currentMaxHashValue; grown > min; grown -= size_type(2)) {
            bool found = true;

            for (size_type mod = size_type(3); mod < currentMaxHashValue; mod += size_type(2)) {
                if ((grown % mod) == size_type(0)) {
                    found = false;
                    break;
                }
            }

            if (found) {
                break;
            }
        }

        return grown;
    }

    /*
     * endregion DefaultRehash
     */
#pragma endregion DefaultRehash

#pragma region DefaultRehashIf
    /*
     * region DefaultRehashIf
     */

    template <typename TKey, typename TData>
    inline bool HashMap<TKey, TData>::DefaultRehashIf::operator()(size_type currentSize, size_type currentMaxHashValue) const {
        return currentSize > ((currentMaxHashValue >> 1) + (currentMaxHashValue >> 2));
    }

    /*
     * endregion DefaultRehashIf
     */
#pragma endregion DefaultRehashIf

#pragma region __iterator
     /*
      * region __iterator
      */

    template <typename TKey, typename TData> template <typename TDeref>
    inline HashMap<TKey, TData>::__iterator<TDeref>::__iterator(
        HashMap& map,
        typename array_type::iterator arrayItr,
        typename chain_type::iterator chainItr
    ) : _map{&map}, _arrayItr{std::move(arrayItr)}, _chainItr{std::move(chainItr)} {
        assert(_arrayItr.IsIteratorFor(_map->_chains));
        assert(_chainItr.IsIteratorFor(*_arrayItr));
    }

    template <typename TKey, typename TData> template <typename TDeref>
    inline typename HashMap<TKey, TData>::__iterator<TDeref>::reference HashMap<TKey, TData>::__iterator<TDeref>::operator*() const {
        return *_chainItr;
    }

    template <typename TKey, typename TData> template <typename TDeref>
    inline typename HashMap<TKey, TData>::__iterator<TDeref>::pointer HashMap<TKey, TData>::__iterator<TDeref>::operator->() const {
        return &(operator*());
    }

    template <typename TKey, typename TData> template <typename TDeref>
    inline HashMap<TKey, TData>::__iterator<TDeref>& HashMap<TKey, TData>::__iterator<TDeref>::operator++() {
        if (_chainItr == _arrayItr->end()) {
            auto arrayBack = (_map->_chains.end() - 1);
            while ((_arrayItr < arrayBack) && (_chainItr == _arrayItr->end())) {
                ++_arrayItr;
                _chainItr = _arrayItr->begin();
            }
        } else {
            ++_chainItr;

            if (_chainItr == _arrayItr->end()) {
                operator++();
            }
        }

        return *this;
    }

    template <typename TKey, typename TData> template <typename TDeref>
    inline HashMap<TKey, TData>::__iterator<TDeref> HashMap<TKey, TData>::__iterator<TDeref>::operator++(int) {
        auto pre = *this;
        operator++();
        return pre;
    }

    template <typename TKey, typename TData> template <typename TDeref>
    inline bool HashMap<TKey, TData>::__iterator<TDeref>::operator!=(const __iterator& other) const {
        return (_map != other._map) || (_arrayItr != other._arrayItr) || (_chainItr != other._chainItr);
    }

    template <typename TKey, typename TData> template <typename TDeref>
    inline bool HashMap<TKey, TData>::__iterator<TDeref>::operator==(const __iterator& other) const {
        return !(*this != other);
    }

    template <typename TKey, typename TData> template <typename TDeref>
    inline bool HashMap<TKey, TData>::__iterator<TDeref>::IsIteratorFor(const HashMap& map) const { return _map == &map; }

    template <typename TKey, typename TData> template <typename TDeref>
    inline bool HashMap<TKey, TData>::__iterator<TDeref>::IsDereferenceable() const { return _chainItr.IsDereferenceable(); }

     /*
      * endregion __iterator
      */
#pragma endregion __iterator

    template <typename TKey, typename TData>
    inline HashMap<TKey, TData>::HashMap(size_type maxHashValue, const emplace_default& defaultEmplaceFunctor)
        : HashMap{maxHashValue, DefaultHash<TKey>{}, DefaultEquality<TKey>{}, defaultEmplaceFunctor} {}

    template <typename TKey, typename TData>
    inline HashMap<TKey, TData>::HashMap(size_type maxHashValue, const key_equal& keyCompareFunctor, emplace_default defaultEmplaceFunctor)
        : HashMap{maxHashValue, DefaultHash<TKey>{}, keyCompareFunctor, defaultEmplaceFunctor} {}

    template <typename TKey, typename TData>
    inline HashMap<TKey, TData>::HashMap(size_type maxHashValue, hasher hashFunctor, key_equal keyCompareFunctor, emplace_default emplaceDefaultFunctor)
        : _chains{maxHashValue}
        , _size{size_type(0)}
        , _usedHashValueCount{size_type(0)}
        , _hashFunctor{std::move(hashFunctor)}
        , _keyCompareFunctor{std::move(keyCompareFunctor)}
        , _emplaceDefaultFunctor{std::move(emplaceDefaultFunctor)}
    {
        if (maxHashValue < size_type(2)) {
            using namespace std::literals::string_literals;

            throw std::invalid_argument("Cannot create a hash map with a max hash value less than 2!"s);
        }

        for (size_type i = size_type(0); i < maxHashValue; ++i) {
            _chains.EmplaceBack();
        }
    }

    template <typename TKey, typename TData>
    inline HashMap<TKey, TData>::HashMap(std::initializer_list<value_type> list) : HashMap{} {
        for (const auto& pair : list) {
            Insert(pair);
        }
    }

    template <typename TKey, typename TData>
    inline HashMap<TKey, TData>::HashMap(HashMap&& other) noexcept
        : _chains{ std::move(other._chains) }
        , _size{ other._size }
        , _usedHashValueCount{ other._usedHashValueCount }
        , _hashFunctor{ other._hashFunctor }
        , _keyCompareFunctor{ other._keyCompareFunctor }
        , _emplaceDefaultFunctor{ other._emplaceDefaultFunctor }
    {
        other._size = size_type(0);
        other._usedHashValueCount = size_type(0);
    }

    template <typename TKey, typename TData>
    inline HashMap<TKey, TData>& HashMap<TKey, TData>::operator=(HashMap&& other) noexcept {
        if (this != &other) {
            _chains = std::move(other._chains);
            _size = other._size;
            _usedHashValueCount = other._usedHashValueCount;
            _hashFunctor = other._hashFunctor;
            _keyCompareFunctor = other._keyCompareFunctor;
            _emplaceDefaultFunctor = other._emplaceDefaultFunctor;
            other._size = size_type(0);
            other._usedHashValueCount = size_type(0);
        }

        return *this;
    }

    template <typename TKey, typename TData>
    inline HashMap<TKey, TData>& HashMap<TKey, TData>::operator=(std::initializer_list<value_type> list) {
        Clear();
        for (const auto& pair : list) {
            Insert(pair);
        }

        return *this;
    }

    template <typename TKey, typename TData>
    inline typename HashMap<TKey, TData>::mapped_type& HashMap<TKey, TData>::operator[](const key_type& key) {
        size_type hash;
        auto found = Find(key, hash);
        return (found == end()) ? PushBackDefaulted(hash, key)->second : found->second;
    }

    template <typename TKey, typename TData>
    inline const typename HashMap<TKey, TData>::mapped_type& HashMap<TKey, TData>::operator[](const key_type& key) const { return Find(key)->second; }

    template <typename TKey, typename TData>
    inline typename HashMap<TKey, TData>::iterator HashMap<TKey, TData>::PushBackDefaulted(size_type chainIndex, const key_type& key) {
        ++_size;
        auto arrayItr = _chains.begin() + chainIndex;
        _usedHashValueCount += arrayItr->IsEmpty() ? 1 : 0;
        return iterator{*this, arrayItr, arrayItr->InsertAfter(arrayItr->end(), std::make_pair(key, _emplaceDefaultFunctor()))};
    }

    template <typename TKey, typename TData>
    inline typename HashMap<TKey, TData>::iterator HashMap<TKey, TData>::PushBack(size_type chainIndex, const_reference pair) {
        ++_size;
        auto arrayItr = _chains.begin() + chainIndex;
        _usedHashValueCount += arrayItr->IsEmpty() ? 1 : 0;
        return iterator{*this, arrayItr, arrayItr->InsertAfter(arrayItr->end(), pair)};
    }

    template <typename TKey, typename TData>
    inline typename HashMap<TKey, TData>::iterator HashMap<TKey, TData>::PushBack(size_type chainIndex, rvalue_reference pair) {
        ++_size;
        auto arrayItr = _chains.begin() + chainIndex;
        _usedHashValueCount += arrayItr->IsEmpty() ? 1 : 0;
        return iterator{*this, arrayItr, arrayItr->InsertAfter(arrayItr->end(), std::move(pair))};
    }

    template <typename TKey, typename TData>
    inline typename HashMap<TKey, TData>::iterator HashMap<TKey, TData>::Insert(const_reference pair) {
        size_type hash;
        auto found = Find(pair.first, hash);
        return (found == end()) ? PushBack(hash, pair) : found;
    }

    template <typename TKey, typename TData>
    inline typename HashMap<TKey, TData>::iterator HashMap<TKey, TData>::Insert(rvalue_reference pair) {
        size_type hash;
        auto found = Find(pair.first, hash);
        return (found == end()) ? PushBack(hash, std::move(pair)) : found;
    }

    template <typename TKey, typename TData>
    inline typename HashMap<TKey, TData>::iterator HashMap<TKey, TData>::InsertOrAssign(const_reference pair) {
        size_type hash;
        auto found = Find(pair.first, hash);

        if (found == end()) {
            return PushBack(hash, pair);
        }

        found->second = pair.second;
        return found;
    }

    template <typename TKey, typename TData>
    inline typename HashMap<TKey, TData>::iterator HashMap<TKey, TData>::InsertOrAssign(rvalue_reference pair) {
        size_type hash;
        auto found = Find(pair.first, hash);

        if (found == end()) {
            return PushBack(hash, std::move(pair));
        }

        found->second = std::move(pair.second);
        return found;
    }

    template <typename TKey, typename TData>
    inline bool HashMap<TKey, TData>::Remove(const key_type& key) { return Remove(Find(key)); }

    template <typename TKey, typename TData>
    inline bool HashMap<TKey, TData>::Remove(const iterator& position) { auto finishExclusive = position; return Remove(position, ++finishExclusive); }

    template <typename TKey, typename TData>
    inline bool HashMap<TKey, TData>::Remove(const iterator& startInclusive, const iterator& finishExclusive) {
        iterator current = startInclusive;

        while ((current != end()) && (current != finishExclusive)) {
            auto prev = current++;
            prev._arrayItr->Remove(prev._chainItr);
            --_size;
            _usedHashValueCount -= prev._arrayItr->IsEmpty() ? 1 : 0;
        }

        return startInclusive != current;
    }

    template <typename TKey, typename TData>
    inline void HashMap<TKey, TData>::Clear() {
        for (auto& chain : _chains) {
            chain.Clear();
        }

        _size = size_type(0);
        _usedHashValueCount = size_type(0);
    }

    template <typename TKey, typename TData>
    inline bool HashMap<TKey, TData>::RehashIfNeeded(rehash rehashFunctor, rehash_if rehashIfFunctor) {
        auto currentHaxHashValue = MaxHashValue();
        bool isRehashing = rehashIfFunctor(_usedHashValueCount, currentHaxHashValue);
        bool isStillRehashing = isRehashing;

        while (isStillRehashing && isRehashing) {
            HashMap rehashed{std::max(currentHaxHashValue + 2, rehashFunctor(currentHaxHashValue)), _hashFunctor, _keyCompareFunctor, _emplaceDefaultFunctor};

            for (const auto& pair : *this) {
                rehashed.Insert(pair);
            }

            swap(rehashed);
            currentHaxHashValue = MaxHashValue();
            isStillRehashing = rehashIfFunctor(_usedHashValueCount, currentHaxHashValue);
        }

        return isRehashing;
    }

    template <typename TKey, typename TData>
    inline void HashMap<TKey, TData>::ForceRehash(rehash rehashFunctor) {
        auto currentHaxHashValue = MaxHashValue();
        ForceRehash(std::max(currentHaxHashValue + 2, rehashFunctor(currentHaxHashValue)));
    }

    template <typename TKey, typename TData>
    inline void HashMap<TKey, TData>::ForceRehash(size_type newMaxHashValue) {
        if (newMaxHashValue < size_type(2)) {
            using namespace std::literals::string_literals;

            throw std::invalid_argument("Cannot create a hash map with a max hash value less than 2!"s);
        }

        HashMap rehashed{newMaxHashValue, _hashFunctor, _keyCompareFunctor, _emplaceDefaultFunctor};

        for (const auto& pair : *this) {
            rehashed.Insert(pair);
        }

        swap(rehashed);
    }

    template <typename TKey, typename TData>
    inline bool HashMap<TKey, TData>::IsEmpty() const { return _size == 0; }

    template <typename TKey, typename TData>
    inline typename HashMap<TKey, TData>::size_type HashMap<TKey, TData>::Size() const { return _size; }

    template <typename TKey, typename TData>
    inline typename HashMap<TKey, TData>::size_type HashMap<TKey, TData>::MaxHashValue() const { return _chains.Size(); }

    template <typename TKey, typename TData>
    inline typename HashMap<TKey, TData>::size_type HashMap<TKey, TData>::UsedHashValueCount() const { return _usedHashValueCount; }

    template <typename TKey, typename TData>
    inline double HashMap<TKey, TData>::LoadFactor() const {
        return static_cast<double>(_usedHashValueCount) / static_cast<double>(MaxHashValue());
    }

    template <typename TKey, typename TData>
    inline bool HashMap<TKey, TData>::IsContainingKey(const key_type& key) const {
        size_type _;
        return IsContainingKey(key, _);
    }

    template <typename TKey, typename TData>
    inline bool HashMap<TKey, TData>::IsContainingKey(const key_type& key, size_type& hash) const { return cend() != CFind(key, hash); }

    template <typename TKey, typename TData>
    inline typename HashMap<TKey, TData>::mapped_type& HashMap<TKey, TData>::At(const key_type& key) {
        auto found = Find(key);

        if (found == end()) {
            using namespace std::literals::string_literals;

            throw std::out_of_range("Map does not contain an element with the given key."s);
        }

        return found->second;
    }

    template <typename TKey, typename TData>
    inline const typename HashMap<TKey, TData>::mapped_type& HashMap<TKey, TData>::At(const key_type& key) const { return CAt(key); }

    template <typename TKey, typename TData>
    inline const typename HashMap<TKey, TData>::mapped_type& HashMap<TKey, TData>::CAt(const key_type& key) const {
        auto found = CFind(key);

        if (found == cend()) {
            using namespace std::literals::string_literals;

            throw std::out_of_range("Map does not contain an element with the given key."s);
        }

        return found->second;
    }

    template <typename TKey, typename TData>
    inline typename HashMap<TKey, TData>::iterator HashMap<TKey, TData>::Find(const key_type& key) {
        size_type _;
        return Find(key, _);
    }

    template <typename TKey, typename TData>
    inline typename HashMap<TKey, TData>::const_iterator HashMap<TKey, TData>::Find(const key_type& key) const { return CFind(key); }

    template <typename TKey, typename TData>
    inline typename HashMap<TKey, TData>::const_iterator HashMap<TKey, TData>::CFind(const key_type& key) const {
        size_type _;
        return CFind(key, _);
    }

    template <typename TKey, typename TData>
    inline typename HashMap<TKey, TData>::const_iterator HashMap<TKey, TData>::CFind(const key_type& key, size_type& hash) const {
        auto itr = (const_cast<HashMap*>(this))->Find(key, hash);
        return const_iterator{*(const_cast<HashMap*>(this)), itr._arrayItr, itr._chainItr};
    }

    template <typename TKey, typename TData>
    inline typename HashMap<TKey, TData>::iterator HashMap<TKey, TData>::Find(const key_type& key, size_type& hash) {
        hash = _hashFunctor(key) % MaxHashValue();
        auto arrayItr = _chains.begin();
        auto& chain = arrayItr[hash];

        for (auto chainItr = chain.begin(); chainItr != chain.end(); ++chainItr) {
            if (_keyCompareFunctor(key, chainItr->first)) {
                return iterator{*this, arrayItr, chainItr};
            }
        }

        return end();
    }

    template <typename TKey, typename TData>
    inline typename HashMap<TKey, TData>::iterator HashMap<TKey, TData>::begin() {
        auto arrayItr = _chains.begin();
        auto arrayBack = _chains.end() - 1;

        while ((arrayItr < arrayBack) && arrayItr->IsEmpty()) {
            ++arrayItr;
        }

        return iterator{*this, arrayItr, arrayItr->begin()};
    }

    template <typename TKey, typename TData>
    inline typename HashMap<TKey, TData>::const_iterator HashMap<TKey, TData>::begin() const { return cbegin(); }

    template <typename TKey, typename TData>
    inline typename HashMap<TKey, TData>::const_iterator HashMap<TKey, TData>::cbegin() const {
        auto itr = (const_cast<HashMap*>(this))->begin();
        return const_iterator{*(const_cast<HashMap*>(this)), itr._arrayItr, itr._chainItr};
    }

    template <typename TKey, typename TData>
    inline typename HashMap<TKey, TData>::iterator HashMap<TKey, TData>::end() {
        auto arrayItr = _chains.end() - 1;
        return iterator{*this, arrayItr, arrayItr->end()};
    }

    template <typename TKey, typename TData>
    inline typename HashMap<TKey, TData>::const_iterator HashMap<TKey, TData>::end() const { return cend(); }

    template <typename TKey, typename TData>
    inline typename HashMap<TKey, TData>::const_iterator HashMap<TKey, TData>::cend() const {
        auto itr = (const_cast<HashMap*>(this))->end();
        return const_iterator{*(const_cast<HashMap*>(this)), itr._arrayItr, itr._chainItr};
    }

    template <typename TKey, typename TData>
    inline void HashMap<TKey, TData>::swap(HashMap& other) {
        using std::swap;

        hasher hFunc = std::move(_hashFunctor);
        _hashFunctor = std::move(other._hashFunctor);
        other._hashFunctor = std::move(hFunc);

        key_equal kcFunc = std::move(_keyCompareFunctor);
        _keyCompareFunctor = std::move(other._keyCompareFunctor);
        other._keyCompareFunctor = std::move(kcFunc);

        emplace_default edFunc = std::move(_emplaceDefaultFunctor);
        _emplaceDefaultFunctor = std::move(other._emplaceDefaultFunctor);
        other._emplaceDefaultFunctor = std::move(edFunc);

        _chains.swap(other._chains);
        swap(_size, other._size);
        swap(_usedHashValueCount, other._usedHashValueCount);
    }

    template <typename TKey, typename TData>
    inline void swap(HashMap<TKey, TData>& first, HashMap<TKey, TData>& second) { first.swap(second); }

    template <typename TKey, typename TData>
    inline typename HashMap<TKey, TData>::hasher HashMap<TKey, TData>::GetHashFunctor() const { return _hashFunctor; }

    template <typename TKey, typename TData>
    inline const typename HashMap<TKey, TData>::key_equal& HashMap<TKey, TData>::GetKeyCompareFunctor() const { return _keyCompareFunctor; }

    template <typename TKey, typename TData>
    inline const typename HashMap<TKey, TData>::emplace_default& HashMap<TKey, TData>::GetEmplaceDefaultFunctor() const { return _emplaceDefaultFunctor; }

    template <typename TKey, typename TData>
    inline void HashMap<TKey, TData>::SetEmplaceDefaultFunctor(const emplace_default& emplaceDefaultFunctor) { _emplaceDefaultFunctor = emplaceDefaultFunctor; }
}
