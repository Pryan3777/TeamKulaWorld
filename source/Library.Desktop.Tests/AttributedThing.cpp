#include "pch.h"
#include "AttributedThing.h"

using namespace std::literals::string_literals;

namespace LibraryDesktopTests {
    RTTI_DEFINITIONS(AttributedThing);

    FieaGameEngine::SignatureVector AttributedThing::Signatures() {
        using DatumType = FieaGameEngine::Datum::DatumType;
        using size_type = FieaGameEngine::Datum::size_type;

        return FieaGameEngine::SignatureVector{
            Signature{"Level"s, DatumType::Integer, true, size_type(1), offsetof(AttributedThing, _level)},
            Signature{"MaxLevel"s, DatumType::Integer, true, size_type(1), offsetof(AttributedThing, _maxLevel)},
            Signature{"MaxHealth"s, DatumType::Float, true, size_type(1), offsetof(AttributedThing, _maxHealth)},
            Signature{"CurrentHealth"s, DatumType::Float, true, size_type(1), offsetof(AttributedThing, _currentHealth)},
            Signature{"Transform"s, DatumType::Matrix, true, size_type(1), offsetof(AttributedThing, _transform)}
        };
    }

    AttributedThing::AttributedThing(
        Integer level,
        Integer maxLevel,
        Float maxHealth,
        Float currentHealth,
        const Matrix& transform
    ) : Attributed{AttributedThing::TypeIdClass()}
        , _level{level}
        , _maxLevel{maxLevel}
        , _maxHealth{maxHealth}
        , _currentHealth{currentHealth}
        , _transform{transform} {}
}
