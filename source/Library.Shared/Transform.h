#pragma once
#include "Attributed.h"
#include "AttributedSignatureRegistry.h"
#include "Factory.h"

namespace FieaGameEngine {
    class Transform final : public Attributed {
        RTTI_DECLARATIONS(Transform, Attributed);

    public:
        using vec4 = Datum::Vector;

        static SignatureVector Signatures();
        static const vec4 ZERO;
        static const vec4 ONE;

        Transform();
        Transform(const vec4& localPosition, const vec4& localRotation, const vec4& localScale);
        Transform(const Transform&);
        Transform(Transform&&) noexcept;
        Transform& operator=(const Transform&);
        Transform& operator=(Transform&&) noexcept;
        ~Transform() = default;

        [[nodiscard]] const vec4& GetLocalPosition() const;
        [[nodiscard]] const vec4& GetLocalRotation() const;
        [[nodiscard]] const vec4& GetLocalScale() const;

        void LocalTranslate(const vec4& translation);

        void Reset();
        void Clear() override;

        [[nodiscard]] ScopeUniquePointer Clone() const override;
        [[nodiscard]] std::string ToString() const override;

        void swap(Transform&);

    private:
        vec4 _localPosition{ZERO};
        vec4 _localRotation{ZERO};
        vec4 _localScale{ONE};

    };

    void swap(Transform& lhs, Transform& rhs);

    FACTORY(Transform, Scope);
}

#include "Transform.inl"
