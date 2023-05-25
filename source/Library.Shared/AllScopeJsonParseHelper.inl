#pragma once
#include "AllScopeJsonParseHelper.h"

namespace FieaGameEngine::ScopeJsonParse {
    inline bool AllScopeJsonParseHelper::IsAbleToParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) {
        return _typeHelper.IsAbleToParse(wrapper, key, value, isArrayElement, index)
            || _classHelper.IsAbleToParse(wrapper, key, value, isArrayElement, index)
            || _expressionHelper.IsAbleToParse(wrapper, key, value, isArrayElement, index)
            || _intHelper.IsAbleToParse(wrapper, key, value, isArrayElement, index)
            || _floatHelper.IsAbleToParse(wrapper, key, value, isArrayElement, index)
            || _vectorHelper.IsAbleToParse(wrapper, key, value, isArrayElement, index)
            || _matrixHelper.IsAbleToParse(wrapper, key, value, isArrayElement, index)
            || _stringHelper.IsAbleToParse(wrapper, key, value, isArrayElement, index);
    }
    inline bool AllScopeJsonParseHelper::TryParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) {
        return _typeHelper.TryParse(wrapper, key, value, isArrayElement, index)
            || _classHelper.TryParse(wrapper, key, value, isArrayElement, index)
            || _expressionHelper.TryParse(wrapper, key, value, isArrayElement, index)
            || _intHelper.TryParse(wrapper, key, value, isArrayElement, index)
            || _floatHelper.TryParse(wrapper, key, value, isArrayElement, index)
            || _vectorHelper.TryParse(wrapper, key, value, isArrayElement, index)
            || _matrixHelper.TryParse(wrapper, key, value, isArrayElement, index)
            || _stringHelper.TryParse(wrapper, key, value, isArrayElement, index);
    }
    inline std::unique_ptr<IJsonParseHelper> AllScopeJsonParseHelper::Create() const { return std::make_unique<AllScopeJsonParseHelper>(); }
}
