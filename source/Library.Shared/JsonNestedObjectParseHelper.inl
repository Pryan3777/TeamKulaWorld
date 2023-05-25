#pragma once
#include "JsonNestedObjectParseHelper.h"

namespace FieaGameEngine {
    inline bool JsonNestedObjectParseHelper::IsAbleToParse(Wrapper&, const Json::String&, const Json::Value& value, bool, Json::ArrayIndex) { return value.isObject(); }
    inline bool JsonNestedObjectParseHelper::EndParse(Wrapper& wrapper, const Json::String& key) { return IJsonParseHelper::EndParse(wrapper, key); }
    inline std::unique_ptr<IJsonParseHelper> JsonNestedObjectParseHelper::Create() const { return std::make_unique<JsonNestedObjectParseHelper>(); }
}
