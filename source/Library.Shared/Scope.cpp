#include "pch.h"
#include "Scope.h"

namespace FieaGameEngine {
    RTTI_DEFINITIONS(Scope);

    using namespace std::literals::string_literals;

    const typename Scope::key_type Scope::THIS_KEY = "this"s;

    Scope::Scope(const Scope& other)
        : RTTI(other)
        , _array{other._array.Size(), other._array.GetGrowCapacityFunctor()}
        , _map{other._map.MaxHashValue(), other._map.GetHashFunctor(), other._map.GetKeyCompareFunctor(), other._map.GetEmplaceDefaultFunctor()}
    {
        for (const auto pair : other._array) {
            Append(pair->first) = pair->second;
        }
        ParentDatumsToThis();
    }

    Scope::Scope(Scope&& other) noexcept : RTTI(std::move(other)), _array{std::move(other._array)}, _map{std::move(other._map)} {
        FinalizeMove(std::forward<Scope>(other));
    }

    Scope& Scope::operator=(const Scope& other) {
        RTTI::operator=(other);
        Scope copy{other};
        swap(copy);
        return *this;
    }

    Scope& Scope::operator=(Scope&& other) noexcept {
        if (this != &other) {
            RTTI::operator=(std::move(other));
            FullClear();
            _array = std::move(other._array);
            _map = std::move(other._map);
            FinalizeMove(std::forward<Scope>(other));
        }

        return *this;
    }

    Scope::~Scope() {
        DetachFromTree();
        FullClear();
    }

    void Scope::ParentDatumsToThis() {
        for (auto& pair : _array) {
            pair->second.SetAndPromulgateParent(this);
        }
    }

    void Scope::FinalizeMove(Scope&& other) noexcept {
        other.DetachFromTree();
        ParentDatumsToThis();
    }

    void Scope::ForEachNestedScope(IsNestedScopeForEachBreakingFunctor isForEachBreakingFunctor) {
        for (auto* pair : _array) {
            if (pair->second.ActualType() == Datum::DatumType::InternalTable) {
                for (size_type i = size_type(0); i < pair->second.Size(); ++i) {
                    if (pair->second.IsElementRetrievable(i) && isForEachBreakingFunctor(pair->second, pair->second.GetTableElement(i), i)) {
                        return;
                    }
                }
            }
        }
    }

    std::pair<Datum*, typename Scope::size_type> Scope::FindContainingDatum(const Scope& scope) {
        auto position = std::make_pair<Datum*, size_type>(nullptr, Size());

        ForEachNestedScope([&position, &scope](Datum& datum, Scope& nested, size_type index) {
            if (&scope == &nested) {
                position.first = &datum;
                position.second = index;
                return true;
            }

            return false;
        });

        return position;
    }

    Datum& Scope::ValidatedAppendScopeDatum(const key_type& key) {
        Datum& appended = Append(key);

        if ((appended.ActualType() != Datum::DatumType::Unknown) && (appended.ActualType() != Datum::DatumType::InternalTable)) {
            using namespace std::literals::string_literals;

            throw std::runtime_error("Cannot append scope to datum with type "s + ToStringDatumType(appended.ActualType()) + "!"s);
        }

        return appended;
    }

    Scope& Scope::AppendScope(const key_type& key, size_type capacity, GrowCapacityFunctorType growCapacityFunctor) {
        Datum& validated = ValidatedAppendScopeDatum(key);
        return AppendScope(validated, std::make_unique<Scope>(capacity, growCapacityFunctor));
    }

    Scope& Scope::AppendScope(const key_type& key, const std::string& classname) {
        Datum& validated = ValidatedAppendScopeDatum(key);
        return AppendScope(validated, Factory<Scope>::StaticCreate(classname));
    }

    bool Scope::operator==(const Scope& other) const {
        if (this == &other) {
            return true;
        }

        if (Size() != other.Size()) {
            return false;
        }

        for (const auto& key : other._map) {
            auto found = _map.CFind(key.first);

            if (found == _map.cend()) {
                return false;
            } else {
                if (found->first == THIS_KEY) {
                    continue;
                }

                if (found->second != key.second) {
                    return false;
                }
            }
        }

        return true;
    }

    typename Datum* Scope::Search(const key_type& key, Scope*& outputContainingScope) {
        auto found = Find(key);

        if (found == end()) {
            outputContainingScope = _parent;
            return (_parent == nullptr) ? nullptr : _parent->Search(key, outputContainingScope);
        }

        outputContainingScope = this;
        return &(found->second);
    }

    void Scope::AttachAsChild(const key_type& key, Scope&& child, bool& outputDidDetachFromOtherParent) {
        if (child.IsThisOrAncestorOf(*this)) {
            throw std::invalid_argument("Fatal, attaching child would result in cyclic dependency!"s);
        }

        outputDidDetachFromOtherParent = child._parent != nullptr;
        child.DetachFromTree();
        AppendScope(ValidatedAppendScopeDatum(key), std::make_unique<Scope>(std::forward<Scope>(child)));
    }

    void Scope::AttachAsChild(const key_type& key, ScopeUniquePointer&& child, bool& outputDidDetachFromOtherParent) {
        if (child->IsThisOrAncestorOf(*this)) {
            throw std::invalid_argument("Fatal, attaching child pointer would result in cyclic dependency!"s);
        }

        outputDidDetachFromOtherParent = child->_parent != nullptr;
        child->DetachFromTree();
        AppendScope(ValidatedAppendScopeDatum(key), std::forward<ScopeUniquePointer>(child));
    }

    void Scope::DetachFromTree() {
        if (_parent != nullptr) {
            auto position = _parent->FindContainingDatum(*this);

            if (position.first != nullptr) {
                position.first->RemoveAt(position.second);
            }

            _parent = nullptr;
        }
    }

    bool Scope::TrySetParent(Scope* parent) {
        if ((_parent == nullptr) && (parent != nullptr) && !IsThisOrAncestorOf(*parent)) {
            _parent = parent;
            return true;
        }

        return false;
    }

    bool Scope::IsDescendantOf(const Scope& other) const {
        return (_parent != nullptr) && _parent->IsThisOrDescendantOf(other);
    }

    void Scope::FullClear() {
        ForEachNestedScope([](Datum&, Scope& scope, size_type) {
            scope._parent = nullptr;
            scope.FullClear();
            return false;
        });

        _array.Clear();
        _map.Clear();
    }

    std::string Scope::ElementToString(size_type index) const {
        using namespace std::literals::string_literals;

        const auto* pair = _array.CAt(index);
        return "{ \""s + pair->first + "\": "s + pair->second.ToString() + " }"s;
    }

    std::string Scope::ToString() const {
        using namespace std::literals::string_literals;

        String s = "{ "s;

        if (Size() >= size_type(1)) {
            s += "pair"s + ElementToString(size_type(0));
        }

        for (size_type index = size_type(1); index < Size(); ++index) {
            s += ", pair"s + ElementToString(index);
        }

        s += " }"s;

        return s;
    }

    void Scope::swap(Scope& other) {
        _map.swap(other._map);
        _array.swap(other._array);
        ParentDatumsToThis();
        other.ParentDatumsToThis();
    }
}
