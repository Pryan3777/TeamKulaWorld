#include "pch.h"
#include "ActionIncrement.h"

using namespace std::literals::string_literals;

namespace FieaGameEngine::Actions {
    RTTI_DEFINITIONS(ActionIncrement);

    const typename ActionIncrement::key_type ActionIncrement::CURRENT_KEY = "Current"s;
    const typename ActionIncrement::key_type ActionIncrement::STEP_KEY = "Step"s;

    SignatureVector ActionIncrement::Signatures() {
        return SignatureVector{
            Signature{CURRENT_KEY, DatumType::Integer, true, size_type(1), offsetof(ActionIncrement, _current)},
            Signature{STEP_KEY, DatumType::Integer, true, size_type(1), offsetof(ActionIncrement, _step)}
        };
    }

    ActionIncrement::ActionIncrement() : Action{ActionIncrement::TypeIdClass(), String{}} {}
}
