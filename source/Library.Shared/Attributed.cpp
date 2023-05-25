#include "pch.h"
#include "Attributed.h"
#include "AttributedSignatureRegistry.h"

namespace FieaGameEngine {
    RTTI_DEFINITIONS(Attributed);

    Attributed& Attributed::operator=(const Attributed& other) {
        if (this != &other) {
            Scope::operator=(other);
            UpdatePrescribedAttributes(other.TypeIdInstance());
        }

        return *this;
    }

    Attributed& Attributed::operator=(Attributed&& other) noexcept {
        if (this != &other) {
            Scope::operator=(std::forward<Attributed>(other));
            UpdatePrescribedAttributes(other.TypeIdInstance());
        }

        return *this;
    }

    void Attributed::Populate(IdType idOfSignaturesToAppend) {
        using namespace std::literals::string_literals;

        Append(THIS_KEY) = this;

        auto signatures = AttributedSignatureRegistry::FindSignatures(idOfSignaturesToAppend);

        if (!AttributedSignatureRegistry::IsFound(signatures)) {
            throw std::logic_error("No signatures found for Attributed ID "s + std::to_string(idOfSignaturesToAppend) + "!"s);
        }

        _prescribedAttributeCount = size_type(1) + signatures->second.Size();
        for (const auto& signature : signatures->second) {
            if (signature.IsStorageExternal()) {
                Append(signature.Key()).SetStorage(
                    signature.Type(),
                    reinterpret_cast<std::byte*>(this) + signature.MemoryOffset(),
                    signature.Count(),
                    signature.IsStorageConst()
                );
            } else {
                Append(signature.Key()).SetType((signature.Type() == DatumType::Table) ? DatumType::InternalTable : signature.Type());
            }
        }
    }

    void Attributed::UpdatePrescribedAttributes(IdType idOfSignaturesToUpdate) {
        using namespace std::literals::string_literals;

        assert(Size() >= _prescribedAttributeCount);
        assert(IsContainingKey(THIS_KEY));
        assert(&(operator[](THIS_KEY)) == &(operator[](size_type(0))));
        operator[](size_type(0)) = this;

        auto signatures = AttributedSignatureRegistry::FindSignatures(idOfSignaturesToUpdate);

        if (!AttributedSignatureRegistry::IsFound(signatures)) {
            throw std::logic_error("No signatures found for Attributed "s + std::to_string(idOfSignaturesToUpdate) + "!"s);
        }

        auto size = signatures->second.Size();
        for (auto i = size_type(1); i <= size; ++i) {
            const auto& signature = signatures->second[i - size_type(1)];
            assert(IsContainingKey(signature.Key()));
            assert(&(operator[](signature.Key())) == &(operator[](i)));
            assert((signature.Type() != DatumType::InternalTable) || (operator[](i).ActualType() == DatumType::InternalTable));
            if (signature.Type() != DatumType::InternalTable) {
                operator[](i).SetStorage(
                    signature.Type(),
                    reinterpret_cast<std::byte*>(this) + signature.MemoryOffset(),
                    signature.Count(),
                    signature.IsStorageConst()
                );
            }
        }
    }

    Scope& Attributed::AppendAuxiliaryScopeAttribute(const key_type& attr, size_type capacity, GrowCapacityFunctorType functor) {
        if (IsPrescribedAttribute(attr)) {
            using namespace std::literals::string_literals;

            throw std::invalid_argument(attr + " is a prescribed attribute, cannot append as auxiliary."s);
        }

        return AppendScope(attr, capacity, functor);
    }

    bool Attributed::PerformPredicateOnAttribute(const key_type& attribute, ConstAttributePredicate predicate) const {
        using array_value_type = decltype(_array)::value_type;

        value_type query = std::make_pair(attribute, Datum{});
        return predicate(_array.CFind(&query, [](const array_value_type& lhs, const array_value_type& rhs){
            return lhs->first == rhs->first;
        }));
    }

    typename Attributed::array_iterator Attributed::PrescribedAttributeEndPosition() {
        assert(_prescribedAttributeCount <= _array.Size());

        if (_prescribedAttributeCount == _array.Size()) {
            return _array.end();
        }

        auto position = _array.begin();
        position[_prescribedAttributeCount];
        return position;
    }

    typename Attributed::array_const_iterator Attributed::CPrescribedAttributeEndPosition() const {
        assert(_prescribedAttributeCount <= _array.Size());

        if (_prescribedAttributeCount == _array.Size()) {
            return _array.cend();
        }

        auto position = _array.cbegin();
        position[_prescribedAttributeCount];
        return position;
    }

    void Attributed::ForEachAttribute(IsAttributeForEachBreakingFunctor functor) {
        for (auto& pair : *this) {
            if (functor(pair.first, pair.second)) {
                break;
            }
        }
    }

    void Attributed::ForEachPrescribedAttribute(IsAttributeForEachBreakingFunctor functor) {
        assert(_prescribedAttributeCount <= _array.Size());
        for (auto i = size_type(0); i < _prescribedAttributeCount; ++i) {
            value_type* pair = _array[i];
            if (functor(pair->first, pair->second)) {
                break;
            }
        }
    }

    void Attributed::ForEachAuxiliaryAttribute(IsAttributeForEachBreakingFunctor functor) {
        for (auto i = _prescribedAttributeCount; i < _array.Size(); ++i) {
            value_type* pair = _array[i];
            if (functor(pair->first, pair->second)) {
                break;
            }
        }
    }

    void Attributed::CForEachAttribute(IsConstAttributeForEachBreakingFunctor functor) const {
        for (const auto& pair : *this) {
            if (functor(pair.first, pair.second)) {
                break;
            }
        }
    }

    void Attributed::CForEachPrescribedAttribute(IsConstAttributeForEachBreakingFunctor functor) const {
        assert(_prescribedAttributeCount <= _array.Size());
        for (auto i = size_type(0); i < _prescribedAttributeCount; ++i) {
            const value_type* pair = _array[i];
            if (functor(pair->first, pair->second)) {
                break;
            }
        }
    }

    void Attributed::CForEachAuxiliaryAttribute(IsConstAttributeForEachBreakingFunctor functor) const {
        for (auto i = _prescribedAttributeCount; i < _array.Size(); ++i) {
            const value_type* pair = _array[i];
            if (functor(pair->first, pair->second)) {
                break;
            }
        }
    }

    void Attributed::Clear() {
        auto start = PrescribedAttributeEndPosition();
        auto finish = _array.end();

        for (auto current = start; current < finish; ++current) {
            _map.Remove((*current)->first);
        }

        _array.Remove(start, finish);
    }

    void Attributed::swap(Attributed& other) {
        Scope::swap(other);
        UpdatePrescribedAttributes(TypeIdInstance());
        other.UpdatePrescribedAttributes(other.TypeIdInstance());
    }
}
