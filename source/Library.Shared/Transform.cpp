#include "pch.h"
#include "Transform.h"

namespace FieaGameEngine {
    RTTI_DEFINITIONS(Transform);

    SignatureVector Transform::Signatures() {
        using namespace std::literals::string_literals;

        using DatumType = FieaGameEngine::Datum::DatumType;
        using size_type = FieaGameEngine::Datum::size_type;

        return FieaGameEngine::SignatureVector{
            Signature{"LocalPosition"s, DatumType::Vector, true, size_type(1), offsetof(Transform, _localPosition)},
            Signature{"LocalRotation"s, DatumType::Vector, true, size_type(1), offsetof(Transform, _localRotation)},
            Signature{"LocalScale"s, DatumType::Vector, true, size_type(1), offsetof(Transform, _localScale)}
        };
    }

    Transform::Transform() : Attributed{Transform::TypeIdClass()} {}
    Transform::Transform(const vec4& localPosition, const vec4& localRotation, const vec4& localScale)
        : Attributed{Transform::TypeIdClass()}
        , _localPosition{localPosition}
        , _localRotation{localRotation}
        , _localScale{localScale} {}

    Transform& Transform::operator=(Transform&& other) noexcept {
        if (this != &other) {
            Attributed::operator=(std::forward<Transform>(other));
            _localPosition = std::move(other._localPosition);
            _localRotation = std::move(other._localRotation);
            _localScale = std::move(other._localScale);
        }

        return *this;
    }

    void Transform::Reset() {
        _localPosition = ZERO;
        _localRotation = ZERO;
        _localScale = ONE;
    }

    void Transform::swap(Transform& other) {
        Attributed::swap(other);

        vec4 tmp = std::move(_localPosition);
        _localPosition = std::move(other._localPosition);
        other._localPosition = std::move(tmp);

        tmp = std::move(_localRotation);
        _localRotation = std::move(other._localRotation);
        other._localRotation = std::move(tmp);

        tmp = std::move(_localScale);
        _localScale = std::move(other._localScale);
        other._localScale = std::move(tmp);
    }
}
