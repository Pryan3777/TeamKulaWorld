#pragma once
#include "Transform.h"

namespace FieaGameEngine {
    inline Transform::Transform(const Transform& other)
        : Attributed{other}
        , _localPosition{other._localPosition}
        , _localRotation{other._localRotation}
        , _localScale{other._localScale} {}
    inline Transform::Transform(Transform&& other) noexcept
        : Attributed{std::forward<Transform>(other)}
        , _localPosition{std::move(other._localPosition)}
        , _localRotation{std::move(other._localRotation)}
        , _localScale{std::move(other._localScale)}
    {
        other.Reset();
    }
    inline Transform& Transform::operator=(const Transform& other) { Transform copy{other}; swap(copy); return *this; }

    inline const typename Transform::vec4 Transform::ZERO{0.f, 0.f, 0.f, 0.f};
    inline const typename Transform::vec4 Transform::ONE{1.f, 1.f, 1.f, 0.f};

    inline const typename Transform::vec4& Transform::GetLocalPosition() const { return _localPosition; }
    inline const typename Transform::vec4& Transform::GetLocalRotation() const { return _localRotation; }
    inline const typename Transform::vec4& Transform::GetLocalScale() const { return _localScale; }

    inline void Transform::LocalTranslate(const vec4& translation) { _localPosition += translation; }

    inline void Transform::Clear() { Attributed::Clear(); Reset(); }
    inline typename Transform::ScopeUniquePointer Transform::Clone() const { return std::make_unique<Transform>(*this); }
    inline std::string Transform::ToString() const { return Scope::ToString(); }
    inline void swap(Transform& lhs, Transform& rhs) { lhs.swap(rhs); }
}
