#include "pch.h"
#include "AttributedTestMonster.h"

using namespace std::literals::string_literals;

namespace LibraryDesktopTests {
    RTTI_DEFINITIONS(AttributedTestMonster);

    FieaGameEngine::SignatureVector AttributedTestMonster::Signatures() {
        using DatumType = FieaGameEngine::Datum::DatumType;
        using size_type = FieaGameEngine::Datum::size_type;

        return FieaGameEngine::SignatureVector{
            Signature{"EntryMessage"s, DatumType::String, true, size_type(1), offsetof(AttributedTestMonster, _entryMessage)},
            Signature{"Reward"s, DatumType::Table, true, size_type(1), offsetof(AttributedTestMonster, _reward)}
        };
    }

    AttributedTestMonster::AttributedTestMonster(
        Integer level,
        Integer maxLevel,
        Float maxHealth,
        Float currentHealth,
        const Matrix& transform,
        const String& entryMessage,
        Bar::number_type reward
    ) : AttributedThing{AttributedTestMonster::TypeIdClass()}, _entryMessage{entryMessage}, _reward{reward} {
        _level = level;
        _maxLevel = maxLevel;
        _maxHealth = maxHealth;
        _currentHealth = currentHealth;
        _transform = transform;
    }
}
