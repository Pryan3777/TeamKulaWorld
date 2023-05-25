#pragma once
#include "Attributed.h"
#include "AttributedSignatureRegistry.h"
#include "Factory.h"

namespace LibraryDesktopTests {
    class AttributedThing : public FieaGameEngine::Attributed {
        RTTI_DECLARATIONS(AttributedThing, FieaGameEngine::Attributed);

    public:
        using Integer = FieaGameEngine::Datum::Integer;
        using Float = FieaGameEngine::Datum::Float;
        using Matrix = FieaGameEngine::Datum::Matrix;

    protected:
        Integer _level{0};
        Integer _maxLevel{99};
        Float _maxHealth{100.f};
        Float _currentHealth{100.f};
        Matrix _transform{{0.f, 0.f, 0.f, 0.f}, {0.f, 0.f, 0.f, 0.f}, {0.f, 0.f, 0.f, 0.f}, {0.f, 0.f, 0.f, 0.f}};

        inline AttributedThing(IdType idOfSignaturesToAppend) : Attributed{idOfSignaturesToAppend} {}

    public:
        static FieaGameEngine::SignatureVector Signatures();

        AttributedThing(Integer level, Integer maxLevel, Float maxHealth, Float currentHealth, const Matrix& transform);

        inline AttributedThing() : AttributedThing{
            0,
            99,
            100.f,
            100.f,
            {{0.f, 0.f, 0.f, 0.f}, {0.f, 0.f, 0.f, 0.f}, {0.f, 0.f, 0.f, 0.f}, {0.f, 0.f, 0.f, 0.f}}
        } {}
        inline AttributedThing(Integer level, Integer maxLevel, Float maxHealth, const Matrix& transform)
            : AttributedThing{level, maxLevel, maxHealth, maxHealth, transform} {}

        ~AttributedThing() = default;

        [[nodiscard]] inline const Integer& Level() const { return _level; }
        [[nodiscard]] inline const Integer& MaxLevel() const { return _maxLevel; }
        [[nodiscard]] inline const Float& MaxHealth() const { return _maxHealth; }
        [[nodiscard]] inline const Float& CurrentHealth() const { return _currentHealth; }
        [[nodiscard]] inline const Matrix& Transform() const { return _transform; }

        inline void LevelUp() { _level = std::min(_maxLevel, _level + 1); }
        inline void TakeDamage(Float damage) { _currentHealth = std::max(0.f, _currentHealth - damage); }
        inline void Heal(Float health) { _currentHealth = std::min(_maxHealth, _currentHealth + health); }
        inline void FullHeal() { _currentHealth = _maxHealth; }

        [[nodiscard]] inline virtual ScopeUniquePointer Clone() const override { return std::make_unique<AttributedThing>(*this); }
        [[nodiscard]] inline virtual std::string ToString() const override { return Scope::ToString(); }

    };

    FACTORY(AttributedThing, FieaGameEngine::Scope);
}
