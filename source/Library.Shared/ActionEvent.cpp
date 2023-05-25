#include "pch.h"
#include "ActionEvent.h"
#include "AttributedEventArgs.h"
#include "GameplayState.h"

using namespace std::literals::string_literals;

namespace FieaGameEngine::Actions {
    RTTI_DEFINITIONS(ActionEvent);

    const typename ActionEvent::key_type ActionEvent::SUBTYPE_KEY = "Subtype"s;
    const typename ActionEvent::key_type ActionEvent::MILLI_DELAY_KEY = "MillisecondDelay"s;

    SignatureVector ActionEvent::Signatures() { return SignatureVector{
        Signature{SUBTYPE_KEY, DatumType::String, true, size_type(1), offsetof(ActionEvent, _subtype)},
        Signature{MILLI_DELAY_KEY, DatumType::Integer, true, size_type(1), offsetof(ActionEvent, _millisecondDelay)}
    }; }

    void ActionEvent::Update(const GameTime& gameTime) {
        auto args = std::make_unique<AttributedEventArgs>(_subtype);

        CForEachAuxiliaryAttribute([&args](const key_type& key, const Datum& datum){
            args->Append(key) = datum;
            return false;
        });

        GameplayState::Singleton().Events().Enqueue(std::move(args), _millisecondDelay, gameTime.CurrentTime());
    }
}
