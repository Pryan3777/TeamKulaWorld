#pragma once
#include "Attributed.h"

namespace FieaGameEngine {
    template <typename... Args> inline Datum& Attributed::AppendAuxiliaryAttribute(const key_type& attr, Args&& ...args) {
        if (IsPrescribedAttribute(attr)) {
            using namespace std::literals::string_literals;

            throw std::invalid_argument(attr + " is a prescribed attribute, cannot append as auxiliary."s);
        }

        return Append(attr, std::forward<Args>(args)...);
    }

    inline Attributed::Attributed(IdType idOfSignaturesToAppend) { Populate(idOfSignaturesToAppend); }
    inline Attributed::Attributed(const Attributed& other) : Scope{other} {
        UpdatePrescribedAttributes(other.TypeIdInstance());
    }
    inline Attributed::Attributed(Attributed&& other) noexcept : Scope{std::forward<Attributed>(other)} {
        UpdatePrescribedAttributes(other.TypeIdInstance());
    }

    inline typename Attributed::size_type Attributed::PrescribedAttributeCount() const { return _prescribedAttributeCount; }

    inline bool Attributed::IsAttribute(const key_type& attr) const { return IsContainingKey(attr); }
    inline bool Attributed::IsPrescribedAttribute(const key_type& attr) const {
        auto pos = CPrescribedAttributeEndPosition();
        return PerformPredicateOnAttribute(attr, [&pos](array_const_iterator itr){ return itr < pos; });
    }
    inline bool Attributed::IsAuxiliaryAttribute(const key_type& attr) const {
        auto pos = CPrescribedAttributeEndPosition();
        return PerformPredicateOnAttribute(attr, [&pos](array_const_iterator itr){ return itr >= pos; });
    }

    inline void swap(Attributed& lhs, Attributed& rhs) { lhs.swap(rhs); }

    template <typename T> template <typename... Args> inline Attributed::ScopeAttribute<T>::ScopeAttribute(Args&&... args)
        : _inst{std::forward<Args>(args)...} { _ptr = &_inst; }

    template <typename T> inline Attributed::ScopeAttribute<T>::ScopeAttribute(const ScopeAttribute& other) : _inst{other._inst} { _ptr = &_inst; }
    template <typename T> inline Attributed::ScopeAttribute<T>::ScopeAttribute(ScopeAttribute&& other) noexcept : _inst{std::move(other._inst)} {
        _ptr = &_inst;
    }
    template <typename T> inline typename Attributed::ScopeAttribute<T>& Attributed::ScopeAttribute<T>::operator=(const ScopeAttribute& other) {
        if (this != &other) {
            _inst = other._inst;
        }

        return *this;
    }
    template <typename T> inline typename Attributed::ScopeAttribute<T>& Attributed::ScopeAttribute<T>::operator=(ScopeAttribute&& other) noexcept {
        if (this != &other) {
            _inst = std::move(other._inst);
        }

        return *this;
    }

    template <typename T> inline typename Attributed::ScopeAttribute<T>& Attributed::ScopeAttribute<T>::operator=(const T& other) { _inst = other; return *this; }
    template <typename T> inline typename Attributed::ScopeAttribute<T>& Attributed::ScopeAttribute<T>::operator=(T&& other) noexcept { _inst = std::move(other); return *this; }

    template <typename T> inline T& Attributed::ScopeAttribute<T>::get() { return _inst; }
    template <typename T> inline const T& Attributed::ScopeAttribute<T>::get() const { return _inst; }
    template <typename T> inline const T& Attributed::ScopeAttribute<T>::cget() const { return _inst; }
    template <typename T> inline T& Attributed::ScopeAttribute<T>::operator*() { return get(); }
    template <typename T> inline const T& Attributed::ScopeAttribute<T>::operator*() const { return get(); }
    template <typename T> inline T* Attributed::ScopeAttribute<T>::operator->() { return _ptr; }
    template <typename T> inline const T* Attributed::ScopeAttribute<T>::operator->() const { return _ptr; }

    inline Attributed::Signature::Signature(const key_type& key, DatumType type, bool isStorageExternal, size_type count, std::size_t memoffset, bool isStorageConst)
        : _key{key}
        , _type{type}
        , _isStorageExternal{isStorageExternal}
        , _count{count}
        , _memoffset{memoffset}
        , _isStorageConst{isStorageConst} {}

    inline bool Attributed::Signature::operator==(const Signature& other) const {
        // TODO - use mem compare
        return (_key == other._key) && (_type == other._type) && (_count == other._count) && (_memoffset == other._memoffset);
    }
    inline bool Attributed::Signature::operator!=(const Signature& other) const { return !operator==(other); }

    inline const typename Attributed::key_type& Attributed::Signature::Key() const { return _key; }
    inline typename Attributed::DatumType Attributed::Signature::Type() const { return _type; }
    inline bool Attributed::Signature::IsStorageExternal() const { return _isStorageExternal; }
    inline typename Attributed::size_type Attributed::Signature::Count() const { return _count; }
    inline std::size_t Attributed::Signature::MemoryOffset() const { return _memoffset; }
    inline bool Attributed::Signature::IsStorageConst() const { return _isStorageConst; }
}
