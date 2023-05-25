#include "SList.h"

namespace FieaGameEngine {
    /**
     * region Node
     */

    template <typename T> inline SList<T>::Node::Node(const_reference data, Node* next) : _data{data}, _next{next} {}

    template <typename T> template <typename... Args> inline SList<T>::Node::Node(Node* next, Args&&... args) : _data{std::forward<Args>(args)...}, _next{next} {}

    /**
     * endregion Node
     */


    /**
     * region iterator
     */

    template <typename T> template <typename TDeref>
    inline SList<T>::__iterator<TDeref>::__iterator(SList& list, Node* node) : _list{&list}, _node{node} {}

    template <typename T> template <typename TDeref>
    inline typename SList<T>::__iterator<TDeref>::reference SList<T>::__iterator<TDeref>::operator*() const {
        if (!IsDereferenceable()) {
            using namespace std::literals::string_literals;

            throw std::runtime_error("Iterator cannot be dereferenced, points to null."s);
        }

        return _node->_data;
    }

    template <typename T> template <typename TDeref>
    inline typename SList<T>::__iterator<TDeref>::pointer SList<T>::__iterator<TDeref>::operator->() const { return &(operator*()); }

    template <typename T> template <typename TDeref>
    inline SList<T>::__iterator<TDeref>& SList<T>::__iterator<TDeref>::operator++() {
        if (_list == nullptr) {
            using namespace std::literals::string_literals;

            throw std::runtime_error("Iterator is not associated with a list."s);
        }

        _node = (_node == nullptr) ? _node : _node->_next;
        return *this;
    }

    template <typename T> template <typename TDeref>
    inline SList<T>::__iterator<TDeref> SList<T>::__iterator<TDeref>::operator++(int) {
        __iterator it = *this;
        operator++();
        return it;
    }

    template <typename T> template <typename TDeref>
    inline bool SList<T>::__iterator<TDeref>::operator!=(const __iterator& other) const { return (_list != other._list) || (_node != other._node); }

    template <typename T> template <typename TDeref>
    inline bool SList<T>::__iterator<TDeref>::operator==(const __iterator& other) const { return !(*this != other); }

    template <typename T> template <typename TDeref>
    inline bool SList<T>::__iterator<TDeref>::IsIteratorFor(const SList& list) const { return _list == &list; }

    template <typename T> template <typename TDeref>
    inline bool SList<T>::__iterator<TDeref>::IsDereferenceable() const { return _node != nullptr; }

    /**
     * endregion iterator
     */


    template <typename T> inline SList<T>::SList(std::initializer_list<T> inits) {
        for (const auto& init : inits) {
            PushBack(init);
        }
    }

    template <typename T> inline SList<T>::SList(const SList& other) : _size{other._size} {
        Node* source = other._front;

        if (source != nullptr) {
            _front = new Node(source->_data);
            source = source->_next;
        }

        Node* destination = _front;

        while (source != nullptr) {
            destination->_next = new Node(source->_data);
            source = source->_next;
            destination = destination->_next;
        }

        _back = destination;
    }

    template <typename T> inline SList<T>& SList<T>::operator=(const SList& other) {
        SList copy(other);
        swap(copy);
        return *this;
    }

    template <typename T> inline SList<T>::SList(SList&& other) noexcept : _front{other._front}, _back{other._back}, _size{other._size} {
        other._front = nullptr;
        other._back = nullptr;
        other._size = size_type(0);
    }

    template <typename T> inline SList<T>& SList<T>::operator=(SList&& other) noexcept {
        if (this != &other) {
            Clear();
            _front = other._front;
            _back = other._back;
            _size = other._size;
            other._front = nullptr;
            other._back = nullptr;
            other._size = size_type(0);
        }

        return *this;
    }

    template <typename T> inline SList<T>::~SList() { Clear(); }

    template <typename T> inline void SList<T>::Clear() {
        Node* current = _front;

        while (current != nullptr) {
            Node* tmp = current->_next;
            delete current;
            current = tmp;
        }

        _front = nullptr;
        _back = nullptr;
        _size = 0;
    }

    template <typename T> inline void SList<T>::PopBack() {
        if (_size == 0) {
            return;
        }

        Node* back = _front;

        if (_size > 1) {
            while (back->_next->_next != nullptr) {
                back = back->_next;
            }
        }

        delete _back;
        _back = back;
        _back->_next = nullptr;
        --_size;

        if (_size == 0) {
            _front = nullptr;
        }
    }

    template <typename T> inline void SList<T>::PopFront() {
        if (_size == 0) {
            return;
        }

        Node* front = _front->_next;
        delete _front;
        _front = front;
        --_size;

        if (_size == 0) {
            _back = nullptr;
        }
    }

    template <typename T> inline typename SList<T>::iterator SList<T>::PushBack(const_reference data) {
        return EmplaceBack(data);
    }

    template <typename T> inline typename SList<T>::iterator SList<T>::PushBack(rvalue_reference data) {
        return EmplaceBack(std::forward<value_type>(data));
    }

    template <typename T> template <typename... Args> inline typename SList<T>::iterator SList<T>::EmplaceBack(Args&&... args) {
        auto* next = new Node(nullptr, std::forward<Args>(args)...);

        if (_size == 0) {
            assert(_front == nullptr);
            _front = next;
        } else {
            _back->_next = next;
        }

        _back = next;
        ++_size;

        return iterator{*this, _back};
    }

    template <typename T> inline typename SList<T>::iterator SList<T>::PushFront(const_reference data) {
        return EmplaceFront(data);
    }

    template <typename T> inline typename SList<T>::iterator SList<T>::PushFront(rvalue_reference data) {
        return EmplaceFront(std::forward<value_type>(data));
    }

    template <typename T> template <typename... Args> inline typename SList<T>::iterator SList<T>::EmplaceFront(Args&&... args) {
        _front = new Node(_front, std::forward<Args>(args)...);
        ++_size;

        if (_back == nullptr) {
            _back = _front;
        }

        return iterator{*this, _front};
    }

    template <typename T> inline bool SList<T>::IsEmpty() const { return _size == 0; }
    template <typename T> inline typename SList<T>::size_type SList<T>::Size() const { return _size; }

    template <typename T> inline typename SList<T>::reference SList<T>::Front() {
        if (_front == nullptr) {
            using namespace std::literals::string_literals;

            throw std::out_of_range("No front exists in the singly-linked list."s);
        }

        return _front->_data;
    }

    template <typename T> inline typename SList<T>::reference SList<T>::Back() {
        if (_back == nullptr) {
            using namespace std::literals::string_literals;

            throw std::out_of_range("No back exists in the singly-linked list."s);
        }

        return _back->_data;
    }

    template <typename T> inline typename SList<T>::const_reference SList<T>::Front() const {
        return const_cast<SList*>(this)->Front();
    }

    template <typename T> inline typename SList<T>::const_reference SList<T>::Back() const {
        return const_cast<SList*>(this)->Back();
    }

    template <typename T> template <typename ComparisonFunctor> inline typename SList<T>::iterator SList<T>::Find(const_reference element, ComparisonFunctor compare) {
        iterator found = begin();

        while ((found != end()) && !compare(found._node->_data, element)) {
            ++found;
        }

        return found;
    }
    template <typename T> template <typename ComparisonFunctor> inline typename SList<T>::const_iterator SList<T>::Find(const_reference element, ComparisonFunctor compare) const {
        return CFind(element, compare);
    }
    template <typename T> template <typename ComparisonFunctor> inline typename SList<T>::const_iterator SList<T>::CFind(const_reference element, ComparisonFunctor compare) const {
        iterator it = const_cast<SList*>(this)->Find(element, compare);
        return const_iterator{*it._list, it._node};
    }

    template <typename T> inline typename SList<T>::iterator SList<T>::begin() { return iterator{*this, _front}; }
    template <typename T> inline typename SList<T>::const_iterator SList<T>::begin() const { return cbegin(); }
    template <typename T> inline typename SList<T>::const_iterator SList<T>::cbegin() const { return const_iterator{*(const_cast<SList*>(this)), _front}; }

    template <typename T> inline typename SList<T>::iterator SList<T>::end() { return iterator{*this}; }
    template <typename T> inline typename SList<T>::const_iterator SList<T>::end() const { return cend(); }
    template <typename T> inline typename SList<T>::const_iterator SList<T>::cend() const { return const_iterator{*(const_cast<SList*>(this))}; }

    template <typename T> inline typename SList<T>::iterator SList<T>::InsertAfter(const iterator& position, const_reference data) {
        return EmplaceAfter(position, data);
    }

    template <typename T> inline typename SList<T>::iterator SList<T>::InsertAfter(const iterator& position, rvalue_reference data) {
        return EmplaceAfter(position, std::forward<value_type>(data));
    }

    template <typename T> template <typename... Args> inline typename SList<T>::iterator SList<T>::EmplaceAfter(const iterator& position, Args&&... args) {
        if (position._list != this) {
            using namespace std::literals::string_literals;

            throw std::runtime_error("Given position is not valid for this list!"s);
        }

        Node* node = nullptr;
        if ((position._node == nullptr) || (position._node->_next == nullptr)) {
            EmplaceBack(std::forward<Args>(args)...);
            node = _back;
        } else {
            node = new Node(position._node->_next, std::forward<Args>(args)...);
            position._node->_next = node;
            ++_size;
        }

        return iterator{*this, node};
    }

    template <typename T> inline bool SList<T>::Remove(const_reference element) { return Remove(Find(element)); }
    template <typename T> inline bool SList<T>::Remove(const iterator& position) {
        if ((position._list != this) || (position._node == nullptr) || (_size == 0)) {
            return false;
        }

        if (position._node == _back) {
            PopBack();
            return true;
        }

        if (position._node->_next == _back) {
            _back = position._node;
        }

        auto* node = position._node->_next;
        position._node->~Node();
        new (position._node) Node(std::move(*node));
        delete node;
        --_size;

        return true;
    }

    template <typename T> inline void SList<T>::swap(SList& other) {
        using std::swap;

        Node* node = _front;
        _front = other._front;
        other._front = node;

        node = _back;
        _back = other._back;
        other._back = node;

        swap(_size, other._size);
    }

    template <typename T> inline void swap(SList<T>& first, SList<T>& second) { first.swap(second); }
}
