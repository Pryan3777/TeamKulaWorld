#pragma once
#include "Scope.h"
#include "Factory.h"

namespace LibraryDesktopTests {
    class Bar final : public FieaGameEngine::Scope {
        RTTI_DECLARATIONS(Bar, FieaGameEngine::Scope);

    public:
        using number_type = FieaGameEngine::Datum::Integer;

    private:
        number_type* _number;

    public:
        Bar(number_type number);
        Bar(const Bar&);
        Bar(Bar&&) noexcept;
        Bar& operator=(const Bar&);
        Bar& operator=(Bar&&) noexcept;
        ~Bar();

        bool operator==(const Bar& other) const;
        inline bool operator!=(const Bar& other) const { return !operator==(other); }

        inline number_type Number() const { return *_number; }
        inline void SetNumber(number_type number) { *_number = number; }

        [[nodiscard]] virtual ScopeUniquePointer Clone() const override { return std::make_unique<Bar>(*this); }
        inline virtual void Clear() override { *_number = 0; FieaGameEngine::Scope::Clear(); }
        [[nodiscard]] virtual bool Equals(const RTTI* other) const override { return other->Is(TypeIdInstance()) && (operator==(*(other->As<Bar>()))); }
        [[nodiscard]] virtual std::string ToString() const override;
    };

    FACTORY(Bar, FieaGameEngine::Scope, 0);
}
