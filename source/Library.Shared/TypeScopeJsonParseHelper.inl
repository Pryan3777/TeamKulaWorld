#pragma once
#include "TypeScopeJsonParseHelper.h"

namespace FieaGameEngine::ScopeJsonParse {
    inline bool TypeScopeJsonParseHelper::IsAbleToParse(Wrapper& wrapper, const Json::String& key, const Json::Value&, bool isArrayElement, Json::ArrayIndex) {
        ScopeParseWrapper* _;
        return IsAbleToParse(wrapper, key, isArrayElement, _);
    }

    inline bool TypeScopeJsonParseHelper::IsAbleToParse(Wrapper& wrapper, const Json::String& key, bool isArrayElement, ScopeParseWrapper*& converted) {
        return ScopeJsonParseHelper::IsAbleToParse(wrapper, converted) && !isArrayElement && (key == ScopeParseWrapper::KEYWORD_TYPE);
    }

    inline bool TypeScopeJsonParseHelper::EndParse(Wrapper& wrapper, const Json::String& key) { return IJsonParseHelper::EndParse(wrapper, key); }
    inline std::unique_ptr<IJsonParseHelper> TypeScopeJsonParseHelper::Create() const { return std::make_unique<TypeScopeJsonParseHelper>(); }

}
