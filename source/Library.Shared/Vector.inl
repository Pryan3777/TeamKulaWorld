#include "Vector.h"

#ifndef UNREFERENCED_LOCAL
#define UNREFERENCED_LOCAL(L) (L)
#endif

namespace FieaGameEngine {
#pragma region __iterator
    /*
     * region __iterator
     */

    template <typename T> template <typename TDeref>
    inline Vector<T>::__iterator<TDeref>::__iterator(Vector& vector, Vector::size_type index) : _vector{&vector}, _index{index} {}

    template <typename T> template <typename TDeref>
    inline typename Vector<T>::__iterator<TDeref>::reference Vector<T>::__iterator<TDeref>::operator*() const {
        if (_vector == nullptr) {
            using namespace std::literals::string_literals;

            throw std::runtime_error("Iterator is not associated with a vector."s);
        }

        return const_cast<Vector*>(_vector)->At(_index);
    }

    template <typename T> template <typename TDeref>
    inline typename Vector<T>::__iterator<TDeref>::pointer Vector<T>::__iterator<TDeref>::operator->() const { return &(operator*()); }

    template <typename T> template <typename TDeref>
    inline Vector<T>::__iterator<TDeref>& Vector<T>::__iterator<TDeref>::operator++() {
        using namespace std;
        _index = (_vector == nullptr) ? (_index + 1) : min(_index + 1, _vector->_size);
        return *this;
    }

    template <typename T> template <typename TDeref>
    inline Vector<T>::__iterator<TDeref> Vector<T>::__iterator<TDeref>::operator++(int) {
        auto it = *this;
        operator++();
        return it;
    }

    template <typename T> template <typename TDeref>
    inline Vector<T>::__iterator<TDeref>& Vector<T>::__iterator<TDeref>::operator--() {
        _index = (_index > 0) ? (_index - 1) : 0;
        return *this;
    }

    template <typename T> template <typename TDeref>
    inline Vector<T>::__iterator<TDeref> Vector<T>::__iterator<TDeref>::operator--(int) {
        auto it = *this;
        operator--();
        return it;
    }

    template <typename T> template <typename TDeref>
    inline typename Vector<T>::__iterator<TDeref>::reference Vector<T>::__iterator<TDeref>::operator[](size_type index) {
        if (_vector == nullptr) {
            using namespace std::literals::string_literals;

            throw std::runtime_error("Iterator is not associated with a vector."s);
        }

        _index = index;
        return _vector->At(index);
    }

    template <typename T> template <typename TDeref>
    inline Vector<T>::__iterator<TDeref>& Vector<T>::__iterator<TDeref>::operator+=(size_type add) {
        size_type pre = _index;
        _index += add;
        assert(_index >= pre);
        UNREFERENCED_LOCAL(pre);
        return *this;
    }

    template <typename T> template <typename TDeref>
    inline Vector<T>::__iterator<TDeref>& Vector<T>::__iterator<TDeref>::operator-=(size_type sub) {
        size_type pre = _index;
        _index -= sub;
        assert(_index <= pre);
        UNREFERENCED_LOCAL(pre);
        return *this;
    }

    template <typename T> template <typename TDeref>
    inline bool Vector<T>::__iterator<TDeref>::operator<(const __iterator& other) const {
        if ((_vector == nullptr) || (_vector != other._vector)) {
            using namespace std::literals::string_literals;

            throw std::runtime_error("Iterators do not have comparable vectors."s);
        }

        return _index < other._index;
    }

    template <typename T> template <typename TDeref>
    inline bool Vector<T>::__iterator<TDeref>::operator>(const __iterator& other) const { return other < *this; }

    template <typename T> template <typename TDeref>
    inline bool Vector<T>::__iterator<TDeref>::operator<=(const __iterator& other) const { return !(other < *this); }

    template <typename T> template <typename TDeref>
    inline bool Vector<T>::__iterator<TDeref>::operator>=(const __iterator& other) const { return !(*this < other); }

    template <typename T> template <typename TDeref>
    inline bool Vector<T>::__iterator<TDeref>::operator!=(const __iterator& other) const { return (_vector != other._vector) || (_index != other._index); }

    template <typename T> template <typename TDeref>
    inline bool Vector<T>::__iterator<TDeref>::operator==(const __iterator& other) const { return !(*this != other); }

    template <typename T> template <typename TDeref>
    inline bool Vector<T>::__iterator<TDeref>::IsIteratorFor(const Vector& vector) const { return _vector == &vector; }

    template <typename T> template <typename TDeref>
    inline bool Vector<T>::__iterator<TDeref>::IsDereferenceable() const { return (_vector != nullptr) && (_index < _vector->_size); }

    /*
     * endregion __iterator
     */
#pragma endregion __iterator

    template <typename T> inline Vector<T>::~Vector() {
        Clear();
        ShrinkToFit();
    }

    template <typename T> inline Vector<T>::Vector(size_type defaultCapacity, GrowCapacityFunctorType growCapacityFunctor) : _growCapacityFunctor{growCapacityFunctor} {
        if (defaultCapacity) {
            Reserve(defaultCapacity);
        }
    }

    template <typename T> inline Vector<T>::Vector(std::initializer_list<value_type> list) : _growCapacityFunctor{DefaultGrowCapacity{}} {
        Reserve(list.size());
        for (const auto& item : list) {
            PushBack(item);
        }
    }

    template <typename T> inline Vector<T>::Vector(const Vector& other) : _growCapacityFunctor{other._growCapacityFunctor} {
        Reserve(other._size);
        for (const auto& item : other) {
            PushBack(item);
        }
    }

    template <typename T> inline Vector<T>::Vector(Vector&& other) noexcept
        : _data{other._data}
        , _size{other._size}
        , _capacity{other._capacity}
        , _growCapacityFunctor{other._growCapacityFunctor}
    {
        other._data = nullptr;
        other._size = size_type(0);
        other._capacity = size_type(0);
    }

    template <typename T> inline Vector<T>& Vector<T>::operator=(const Vector& other) {
        Vector copy{other};
        swap(copy);
        return *this;
    }

    template <typename T> inline Vector<T>& Vector<T>::operator=(Vector&& other) noexcept {
        if (this != &other) {
            this->~Vector();
            _data = other._data;
            _size = other._size;
            _capacity = other._capacity;
            _growCapacityFunctor = other._growCapacityFunctor;
            other._data = nullptr;
            other._size = size_type(0);
            other._capacity = size_type(0);
        }

        return *this;
    }

    template <typename T> inline typename Vector<T>::reference Vector<T>::operator[](size_type index) {
        if (_data == nullptr) {
            using namespace std::literals::string_literals;

            throw std::runtime_error("Vector has no allocated memory reserved, cannot dereference."s);
        }

        return *(_data + index);
    }
    template <typename T> inline typename Vector<T>::reference Vector<T>::At(size_type index) {
        if (index >= _size) {
            using namespace std::literals::string_literals;

            throw std::out_of_range("Given index exceeds the size of the vector."s);
        }

        return operator[](index);
    }

    template <typename T> inline typename Vector<T>::const_reference Vector<T>::operator[](size_type index) const {
        return const_cast<const_reference>(const_cast<Vector*>(this)->operator[](index));
    }

    template <typename T> inline typename Vector<T>::const_reference Vector<T>::At(size_type index) const { return CAt(index); }
    template <typename T> inline typename Vector<T>::const_reference Vector<T>::CAt(size_type index) const {
        return const_cast<const_reference>(const_cast<Vector*>(this)->At(index));
    }

    template <typename T> inline bool Vector<T>::Remove(const_reference element) { return Remove(Find(element)); }
    template <typename T> template <typename ComparisonFunctor> inline bool Vector<T>::Remove(const_reference element, ComparisonFunctor compare) { return Remove(Find(element, compare)); }
    template <typename T> inline bool Vector<T>::Remove(iterator position) { return Remove(position, position + 1); }
    template <typename T> inline bool Vector<T>::Remove(iterator startInclusive, iterator finishExclusive) {
        using namespace std;
        finishExclusive._index = min(finishExclusive._index, _size);

        if (startInclusive._vector != this) {
            using namespace std::literals::string_literals;

            throw std::invalid_argument("Cannot remove using given iterators because they are not associated with this vector."s);
        }

        if (startInclusive >= finishExclusive) {
            return false;
        }

        size_type index;
        for (index = startInclusive._index; index < finishExclusive._index; ++index) {
            (_data + index)->~T();
        }

        if (_size > finishExclusive._index) {
            std::memmove(_data + startInclusive._index, _data + finishExclusive._index, sizeof(value_type) * (_size - finishExclusive._index));
        }

        _size -= finishExclusive._index - startInclusive._index;
        return true;
    }

    template <typename T> inline bool Vector<T>::RemoveAt(size_type index) { return Remove(iterator{*this, index}); }

    template <typename T> inline void Vector<T>::Clear() {
        for (size_type index = size_type(0); index < _size; ++index) {
            (_data + index)->~T();
        }

        _size = size_type(0);
    }

    template <typename T> inline void Vector<T>::ShrinkToFit(size_type minCapacity) {
        if (_size == _capacity) {
            return;
        }
        using namespace std;
        _capacity = max(min(_capacity, minCapacity), _size);

        if (_capacity == size_type(0)) {
            std::free(_data);
            _data = nullptr;
            return;
        }

        value_type* data = reinterpret_cast<value_type*>(std::realloc(_data, sizeof(value_type) * _capacity));
        assert(data != nullptr);
        _data = data;
    }

    template <typename T> inline void Vector<T>::Reserve(size_type capacity) {
        if (capacity <= _capacity) {
            return;
        }

        value_type* data = reinterpret_cast<value_type*>(std::realloc(_data, sizeof(value_type) * capacity));
        assert(data != nullptr);
        _data = data;
        _capacity = capacity;
    }

    template <typename T> inline typename Vector<T>::iterator Vector<T>::PushBack(const_reference element) {
        return EmplaceBack(element);
    }

    template <typename T> inline typename Vector<T>::iterator Vector<T>::PushBack(rvalue_reference element) {
        return EmplaceBack(std::forward<value_type>(element));
    }

    template <typename T> template <typename... Args> inline typename Vector<T>::iterator Vector<T>::EmplaceBack(Args&&... args) {
        using namespace std;
        if (_size == _capacity) {
            Reserve(max(_growCapacityFunctor(_size, _capacity), _capacity + 1));
        }

        size_type index = _size++;
        new (_data + index) value_type{std::forward<Args>(args)...};
        return iterator{*this, index};
    }

    template <typename T> inline void Vector<T>::PopBack() {
        if (_size > size_type(0)) {
            (_data + --_size)->~T();
        }
    }

    template <typename T> inline bool Vector<T>::IsEmpty() const { return _size == size_type(0); }
    template <typename T> inline typename Vector<T>::size_type Vector<T>::Size() const { return _size; }
    template <typename T> inline typename Vector<T>::size_type Vector<T>::Capacity() const { return _capacity; }

    template <typename T> inline typename Vector<T>::reference Vector<T>::Front() { return At(size_type(0)); }
    template <typename T> inline typename Vector<T>::const_reference Vector<T>::Front() const { return CFront(); }
    template <typename T> inline typename Vector<T>::const_reference Vector<T>::CFront() const { return At(size_type(0)); }
    template <typename T> inline typename Vector<T>::reference Vector<T>::Back() { return At(_size - size_type(1)); }
    template <typename T> inline typename Vector<T>::const_reference Vector<T>::Back() const { return CBack(); }
    template <typename T> inline typename Vector<T>::const_reference Vector<T>::CBack() const { return At(_size - size_type(1)); }

    template <typename T> template <typename ComparisonFunctor> inline typename Vector<T>::iterator Vector<T>::Find(const_reference element, ComparisonFunctor compare) {
        size_type index;

        for (index = size_type(0); index < _size; ++index) {
            if (compare(element, *(_data + index))) {
                break;
            }
        }

        return iterator{*this, index};
    }
    template <typename T> template <typename ComparisonFunctor> inline typename Vector<T>::const_iterator Vector<T>::Find(const_reference element, ComparisonFunctor compare) const {
        iterator it = const_cast<Vector*>(this)->Find(element, compare);
        return const_iterator{*(it._vector), it._index};
    }
    template <typename T> template <typename ComparisonFunctor> inline typename Vector<T>::const_iterator Vector<T>::CFind(const_reference element, ComparisonFunctor compare) const {
        return Find(element, compare);
    }

    template <typename T> inline typename Vector<T>::iterator Vector<T>::begin() { return iterator{*this}; }
    template <typename T> inline typename Vector<T>::const_iterator Vector<T>::begin() const { return cbegin(); }
    template <typename T> inline typename Vector<T>::const_iterator Vector<T>::cbegin() const { return const_iterator{*(const_cast<Vector*>(this))}; }
    template <typename T> inline typename Vector<T>::iterator Vector<T>::end() { return iterator{*this, _size}; }
    template <typename T> inline typename Vector<T>::const_iterator Vector<T>::end() const { return cend(); }
    template <typename T> inline typename Vector<T>::const_iterator Vector<T>::cend() const { return const_iterator{*(const_cast<Vector*>(this)), _size}; }

    template <typename T> inline void Vector<T>::swap(Vector& other) {
        using std::swap;

        value_type* data = _data;
        _data = other._data;
        other._data = data;

        GrowCapacityFunctorType func = std::move(_growCapacityFunctor);
        _growCapacityFunctor = std::move(other._growCapacityFunctor);
        other._growCapacityFunctor = std::move(func);

        swap(_size, other._size);
        swap(_capacity, other._capacity);
    }

    template <typename T> inline void swap(Vector<T>& first, Vector<T>& second) { first.swap(second); }

    template <typename T> inline const typename Vector<T>::GrowCapacityFunctorType& Vector<T>::GetGrowCapacityFunctor() const { return _growCapacityFunctor; }
    template <typename T> inline void Vector<T>::SetGrowCapacityFunctor(GrowCapacityFunctorType func) {
        _growCapacityFunctor = std::move(func);
    }
}
