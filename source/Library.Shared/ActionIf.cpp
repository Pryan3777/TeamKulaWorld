#include "pch.h"
#include "ActionIf.h"

using namespace std::literals::string_literals;

namespace FieaGameEngine::Actions {
    RTTI_DEFINITIONS(ActionIf);

    const typename ActionIf::key_type ActionIf::CONDITION_KEY = "Condition"s;
    const typename ActionIf::key_type ActionIf::ELSE_KEY = "ElseActions"s;

    SignatureVector ActionIf::Signatures() {
        return SignatureVector{
            Signature{CONDITION_KEY, DatumType::Integer, true, size_type(1), offsetof(ActionIf, _condition)},
            Signature{ELSE_KEY, DatumType::InternalTable, false, size_type(0), std::size_t(0)}
        };
    }

    ActionIf::ActionIf(const String& name) : ActionList{ActionIf::TypeIdClass(), name} {
        _else = &(At(ELSE_KEY));
    }

    ActionIf& ActionIf::operator=(ActionIf&& other) noexcept {
        if (this != &other) {
            ActionList::operator=(std::move(other));
            _else = &(At(ELSE_KEY));
        }

        return *this;
    }

    std::string ActionIf::ToString() const {
        assert(_else != nullptr);
        std::stringstream tostring{};
        tostring << TypeNameInstance() << ": { \""s
            << NAME_KEY << "\": "s << GetName() << ", number of then-actions: "s << GetActions().Size() << ", number of else-actions: "s << _else->Size()
        << " }"s;
        return tostring.str();
    }
}
