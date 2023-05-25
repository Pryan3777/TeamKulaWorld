#pragma once
#include "Scope.h"

namespace FieaGameEngine {
    class Attributed : public Scope {
        RTTI_DECLARATIONS(Attributed, Scope);

    public:
        using DatumType = Datum::DatumType;

        class Signature final {

        public:
            Signature(const key_type& key, DatumType type, bool isStorageExternal, size_type count, std::size_t memoffset, bool isStorageConst = false);

            bool operator==(const Signature&) const;
            bool operator!=(const Signature&) const;

            const key_type& Key() const;
            DatumType Type() const;
            bool IsStorageExternal() const;
            size_type Count() const;
            std::size_t MemoryOffset() const;
            bool IsStorageConst() const;

        private:
            key_type _key;
            DatumType _type;
            bool _isStorageExternal;
            size_type _count;
            std::size_t _memoffset;
            bool _isStorageConst;

        };

        Attributed() = delete;
        Attributed(const Attributed&);
        Attributed(Attributed&&) noexcept;
        Attributed& operator=(const Attributed&);
        Attributed& operator=(Attributed&&) noexcept;
        virtual ~Attributed() = default;

        [[nodiscard]] size_type PrescribedAttributeCount() const;

        [[nodiscard]] bool IsAttribute(const key_type& attribute) const;
        [[nodiscard]] bool IsPrescribedAttribute(const key_type& attribute) const;
        [[nodiscard]] bool IsAuxiliaryAttribute(const key_type& attribute) const;

        template <typename... Args> Datum& AppendAuxiliaryAttribute(const key_type& attribute, Args&&... args);
        Scope& AppendAuxiliaryScopeAttribute(
            const key_type& attribute,
            size_type capacity = size_type(0),
            GrowCapacityFunctorType growCapacityFunctor = DefaultGrowCapacity{}
        );

        using IsAttributeForEachBreakingFunctor = std::function<bool(const key_type&, Datum&)>;
        void ForEachAttribute(IsAttributeForEachBreakingFunctor isForEachBreakingFunctor);
        void ForEachPrescribedAttribute(IsAttributeForEachBreakingFunctor isForEachBreakingFunctor);
        void ForEachAuxiliaryAttribute(IsAttributeForEachBreakingFunctor isForEachBreakingFunctor);

        using IsConstAttributeForEachBreakingFunctor = std::function<bool(const key_type&, const Datum&)>;
        void CForEachAttribute(IsConstAttributeForEachBreakingFunctor isForEachBreakingFunctor) const;
        void CForEachPrescribedAttribute(IsConstAttributeForEachBreakingFunctor isForEachBreakingFunctor) const;
        void CForEachAuxiliaryAttribute(IsConstAttributeForEachBreakingFunctor isForEachBreakingFunctor) const;

        virtual void Clear() override;

        [[nodiscard]] virtual ScopeUniquePointer Clone() const override = 0;

        void swap(Attributed&);

    protected:
        template <typename T>
        class ScopeAttribute final {

        public:
            template <typename... Args> ScopeAttribute(Args&&... args);
            ScopeAttribute(const ScopeAttribute&);
            ScopeAttribute(ScopeAttribute&&) noexcept;
            ScopeAttribute& operator=(const ScopeAttribute&);
            ScopeAttribute& operator=(ScopeAttribute&&) noexcept;
            ~ScopeAttribute() = default;

            ScopeAttribute& operator=(const T&);
            ScopeAttribute& operator=(T&&) noexcept;

            T& operator*();
            T* operator->();

            const T& operator*() const;
            const T* operator->() const;

            T& get();
            const T& get() const;
            const T& cget() const;

        private:
            T* _ptr;
            T _inst;

        };

        explicit Attributed(IdType idOfSignaturesToAppend);

        void Populate(IdType idOfSignaturesToAppend);
        void UpdatePrescribedAttributes(IdType idOfSignaturesToUpdate);

    private:
        using array_iterator = decltype(_array)::iterator;
        using array_const_iterator = decltype(_array)::const_iterator;
        using ConstAttributePredicate = std::function<bool(array_const_iterator)>;
        bool PerformPredicateOnAttribute(const key_type& attribute, ConstAttributePredicate predicate) const;

        array_iterator PrescribedAttributeEndPosition();
        array_const_iterator CPrescribedAttributeEndPosition() const;

        size_type _prescribedAttributeCount{size_type(1)};

    };

    void swap(Attributed& lhs, Attributed& rhs);
}

#include "Attributed.inl"
