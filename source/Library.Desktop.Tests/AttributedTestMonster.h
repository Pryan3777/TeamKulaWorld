#pragma once
#include "AttributedThing.h"
#include "Bar.h"

namespace LibraryDesktopTests {
    class AttributedTestMonster final : public AttributedThing {
        RTTI_DECLARATIONS(AttributedTestMonster, AttributedThing);

    public:
        using String = FieaGameEngine::Datum::String;

    private:
        String _entryMessage{};
        ScopeAttribute<Bar> _reward{0};

    public:
        static FieaGameEngine::SignatureVector Signatures();

        AttributedTestMonster(
            Integer level,
            Integer maxLevel,
            Float maxHealth,
            Float currentHealth,
            const Matrix& transform,
            const String& entryMessage,
            Bar::number_type reward
        );

        inline AttributedTestMonster() : AttributedTestMonster{
            0,
            99,
            100.f,
            100.f,
            {{0.f, 0.f, 0.f, 0.f}, {0.f, 0.f, 0.f, 0.f}, {0.f, 0.f, 0.f, 0.f}, {0.f, 0.f, 0.f, 0.f}},
            String{},
            0
        } {}

        AttributedTestMonster(const AttributedTestMonster&) = default;
        AttributedTestMonster(AttributedTestMonster&&) noexcept = default;
        AttributedTestMonster& operator=(const AttributedTestMonster&) = default;
        AttributedTestMonster& operator=(AttributedTestMonster&&) noexcept = default;
        ~AttributedTestMonster() = default;

        [[nodiscard]] inline const String& EntryMessage() const { return _entryMessage; }
        [[nodiscard]] inline const Bar& Reward() const { return _reward.get(); }

        inline void SetReward(Bar::number_type reward) { _reward.get().SetNumber(reward); }

        [[nodiscard]] inline ScopeUniquePointer Clone() const override { return std::make_unique<AttributedTestMonster>(*this); }
        [[nodiscard]] inline std::string ToString() const override { return AttributedThing::ToString(); }

    };

    FACTORY(AttributedTestMonster, FieaGameEngine::Scope);
}
