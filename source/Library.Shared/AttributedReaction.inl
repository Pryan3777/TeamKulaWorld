#pragma once
#include "AttributedReaction.h"
#include <cassert>
#include "AttributedEventArgs.h"
#include "GameplayState.h"

namespace FieaGameEngine {
    inline SignatureVector AttributedReaction::Signatures() { return SignatureVector{
        Signature{REACTS_TO_SUBTYPE_REGEX_KEY, DatumType::String, false, size_type(0), std::size_t(0)}
    }; }

    inline AttributedReaction::AttributedReaction() : AttributedReaction{String{}} {}
    inline AttributedReaction::AttributedReaction(const AttributedReaction& other) : Reaction{other} { _reactsToSubtypeRegexes = &(At(REACTS_TO_SUBTYPE_REGEX_KEY)); }
    inline AttributedReaction::AttributedReaction(AttributedReaction&& other) noexcept : Reaction{std::move(other)} {
        _reactsToSubtypeRegexes = &(At(REACTS_TO_SUBTYPE_REGEX_KEY));
    }
    inline AttributedReaction::~AttributedReaction() { _reactsToSubtypeRegexes = nullptr; }

    inline Datum& AttributedReaction::ReactsToSubtypeRegexes() { assert(_reactsToSubtypeRegexes != nullptr); return *_reactsToSubtypeRegexes; }
    inline const Datum& AttributedReaction::GetReactsToSubtypeRegexes() const { assert(_reactsToSubtypeRegexes != nullptr); return *_reactsToSubtypeRegexes; }
    inline const typename AttributedReaction::String& AttributedReaction::GetReactsToSubtypeRegex(size_type index) const {
        assert(_reactsToSubtypeRegexes != nullptr);
        return _reactsToSubtypeRegexes->CGetStringElement(index);
    }

    inline typename AttributedReaction::ScopeUniquePointer AttributedReaction::Clone() const { return std::make_unique<AttributedReaction>(*this); }
}
