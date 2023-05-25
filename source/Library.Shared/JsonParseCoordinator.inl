#pragma once
#include "JsonParseCoordinator.h"

namespace FieaGameEngine {
    inline JsonParseCoordinator::JsonParseCoordinator(JsonParseCoordinator&& other) noexcept
        : ParseCoordinator{std::forward<JsonParseCoordinator>(other)}
        , _helpers{std::move(other._helpers)}
        , _transmuters{std::move(other._transmuters)} {}

    inline bool JsonParseCoordinator::IsContainingHelper(RTTI::IdType helperTypeId) const { return GetHelper(helperTypeId) != nullptr; }
    inline bool JsonParseCoordinator::IsContainingTransmuter(RTTI::IdType transmuterTypeId) const { return GetTransmuter(transmuterTypeId) != nullptr; }

    inline bool JsonParseCoordinator::IsContainingTransmuter(const IJsonValueTransmuter* transmuter) const {
        return IsContainingTransmuter(transmuter->TypeIdInstance());
    }
}
