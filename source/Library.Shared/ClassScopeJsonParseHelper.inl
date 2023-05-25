#pragma once
#include "ClassScopeJsonParseHelper.h"

namespace FieaGameEngine::ScopeJsonParse {
    inline bool ClassScopeJsonParseHelper::IsAbleToParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex) {
        ScopeParseWrapper* _;
        return IsAbleToParse(wrapper, key, value, isArrayElement, _);
    }
    inline bool ClassScopeJsonParseHelper::EndParse(Wrapper& wrapper, const Json::String& key) { return IJsonParseHelper::EndParse(wrapper, key); }
    inline std::unique_ptr<IJsonParseHelper> ClassScopeJsonParseHelper::Create() const { return std::make_unique<ClassScopeJsonParseHelper>(); }

}
