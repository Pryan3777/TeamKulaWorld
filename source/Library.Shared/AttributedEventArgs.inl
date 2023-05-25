#pragma once
#include "AttributedEventArgs.h"

namespace FieaGameEngine {
    inline SignatureVector AttributedEventArgs::Signatures() {
        return SignatureVector{
            Signature{SUBTYPE_KEY, DatumType::String, true, size_type(1), offsetof(AttributedEventArgs, _subtype)}
        };
    }

    inline AttributedEventArgs::AttributedEventArgs(String subtype) : BaseConstructor{AttributedEventArgs::TypeIdClass()}, _subtype{std::move(subtype)} {}

    inline typename AttributedEventArgs::String& AttributedEventArgs::Subtype() { return _subtype; }
    inline const typename AttributedEventArgs::String& AttributedEventArgs::GetSubtype() const { return _subtype; }
    inline void AttributedEventArgs::SetSubtype(String value) noexcept { _subtype = std::move(value); }

    inline typename AttributedEventArgs::ScopeUniquePointer AttributedEventArgs::CloneAsScope() const { return std::make_unique<AttributedEventArgs>(*this); }
    inline std::unique_ptr<IEventArgs> AttributedEventArgs::CloneAsEventArgs() const { return std::make_unique<AttributedEventArgs>(*this); }
}
