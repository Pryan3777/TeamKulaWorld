#pragma once
#include "GameObject.h"

namespace LibraryDesktopTests {
    class TestGameObject final : public FieaGameEngine::GameObject {
        RTTI_DECLARATIONS(TestGameObject, FieaGameEngine::GameObject);

    public:
        static FieaGameEngine::SignatureVector Signatures();

        TestGameObject();
        inline explicit TestGameObject(String name) : GameObject{name}, _updateCount{0} {}

        inline void UpdateSelf(const FieaGameEngine::GameTime&) override { ++_updateCount; }

        [[nodiscard]] inline FieaGameEngine::Datum::Integer UpdateCount() const { return _updateCount; }

        [[nodiscard]] inline FieaGameEngine::Datum& PublicChildren() { return Children(); }
        [[nodiscard]] inline const FieaGameEngine::Datum& PublicChildren() const { return GetChildren(); }

    private:
        static const key_type UPDATE_COUNT_KEY;

        FieaGameEngine::Datum::Integer _updateCount{0};

    };

    FACTORY(TestGameObject, FieaGameEngine::Scope);
}
