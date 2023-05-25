#pragma once
#include "ExpressionScopeJsonParseHelper.h"

namespace FieaGameEngine::ScopeJsonParse {
    inline bool ExpressionScopeJsonParseHelper::IsAbleToParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool, Json::ArrayIndex index) {
        ScopeParseWrapper* _;
        Scope::key_type __;
        Datum::String ___;
        return IsAbleToParse(wrapper, key, value, index, _, __, ___);
    }
    inline bool ExpressionScopeJsonParseHelper::EndParse(Wrapper& wrapper, const Json::String& key) { return IJsonParseHelper::EndParse(wrapper, key); }
    inline std::unique_ptr<IJsonParseHelper> ExpressionScopeJsonParseHelper::Create() const { return std::make_unique<ExpressionScopeJsonParseHelper>(); }
}
