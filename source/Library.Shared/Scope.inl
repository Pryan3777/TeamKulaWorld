#pragma once
#include "Scope.h"

namespace FieaGameEngine {
    template <typename... Args> inline Datum& Scope::Append(const key_type& key, Args&&... args) {
        if (key.empty()) {
            using namespace std::literals::string_literals;

            throw std::invalid_argument("Cannot append empty key!"s);
        }

        auto found = _map.Find(key);

        if (found == _map.end()) {
            found = _map.Insert(std::make_pair(key, Datum{std::forward<Args>(args)...}));
            found->second.SetAndPromulgateParent(this);
            _array.EmplaceBack(&(*found));
        }

        return found->second;
    }

    inline Scope::Scope(size_type capacity, GrowCapacityFunctorType functor) : _array{capacity, functor} {}

    inline Datum& Scope::operator[](const key_type& key) { return Append(key); }
    inline const Datum& Scope::operator[](const key_type& key) const { return _map[key]; }
    inline Datum& Scope::operator[](size_type index) { return _array[index]->second; }
    inline const Datum& Scope::operator[](size_type index) const { return const_cast<const Datum&>((const_cast<Scope*>(this))->operator[](index)); }

    inline bool Scope::operator!=(const Scope& other) const { return !operator==(other); }

    inline Scope& Scope::AppendScope(Datum& datum, ScopeUniquePointer&& child) { datum.PushBack(std::forward<ScopeUniquePointer>(child)); return datum.BackTable(); }

    inline Datum& Scope::At(const key_type& key) { return _map.At(key); }
    inline const Datum& Scope::CAt(const key_type& key) const { return const_cast<const Datum&>(const_cast<Scope*>(this)->At(key)); }
    inline const Datum& Scope::At(const key_type& key) const { return CAt(key); }
    inline Datum& Scope::At(size_type index) { return _array.At(index)->second; }
    inline const Datum& Scope::CAt(size_type index) const { return const_cast<const Datum&>(const_cast<Scope*>(this)->At(index)); }
    inline const Datum& Scope::At(size_type index) const { return CAt(index); }

    inline bool Scope::IsContainingKey(const key_type& key) const { return CFind(key) != cend(); }
    inline typename Scope::iterator Scope::Find(const key_type& key) { return _map.Find(key); }
    inline typename Scope::const_iterator Scope::CFind(const key_type& key) const { return _map.CFind(key); }
    inline typename Scope::const_iterator Scope::Find(const key_type& key) const { return CFind(key); }
    inline Datum* Scope::Search(const key_type& key) { Scope* _; return Search(key, _); }
    inline const Datum* Scope::CSearch(const key_type& key) const { return const_cast<const Datum*>((const_cast<Scope*>(this))->Search(key)); }
    inline const Datum* Scope::Search(const key_type& key) const { return CSearch(key); }
    inline void Scope::AttachAsChild(const key_type& key, Scope&& child) { bool _; AttachAsChild(key, std::forward<Scope>(child), _); }
    inline void Scope::AttachAsChild(const key_type& key, ScopeUniquePointer&& child) { bool _; AttachAsChild(key, std::forward<ScopeUniquePointer>(child), _); }

    inline bool Scope::IsThisOrDescendantOf(const Scope& scope) const { return (this == &scope) || IsDescendantOf(scope); }
    inline bool Scope::IsAncestorOf(const Scope& other) const { return other.IsDescendantOf(*this); }
    inline bool Scope::IsThisOrAncestorOf(const Scope& scope) const { return (this == &scope) || IsAncestorOf(scope); }

    inline typename Scope::ScopeUniquePointer Scope::Clone() const { return std::make_unique<Scope>(*this); }
    inline bool Scope::Equals(const RTTI* other) const { return RTTI::Equals(other) || (other->Is(TypeIdInstance()) && (operator==(*(other->As<Scope>())))); }

    inline Scope* Scope::Parent() const { return _parent; }
    inline typename Scope::size_type Scope::Size() const { return _array.Size(); }
    inline bool Scope::IsEmpty() const { assert(_array.IsEmpty() == _map.IsEmpty()); return _array.IsEmpty(); }

    inline typename Scope::iterator Scope::begin() { return _map.begin(); }
    inline typename Scope::const_iterator Scope::cbegin() const { return _map.cbegin(); }
    inline typename Scope::const_iterator Scope::begin() const { return cbegin(); }
    inline typename Scope::iterator Scope::end() { return _map.end(); }
    inline typename Scope::const_iterator Scope::cend() const { return _map.cend(); }
    inline typename Scope::const_iterator Scope::end() const { return cend(); }

    inline void Scope::Clear() { FullClear(); }

    inline void swap(Scope& lhs, Scope& rhs) { lhs.swap(rhs); }
}
