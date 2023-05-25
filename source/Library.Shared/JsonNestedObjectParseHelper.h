#pragma once
#include "IJsonParseHelper.h"

namespace FieaGameEngine {
    class JsonNestedObjectParseHelper final : public IJsonParseHelper {
        RTTI_DECLARATIONS(JsonNestedObjectParseHelper, IJsonParseHelper);

    public:
        [[nodiscard]] bool IsAbleToParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) override;
        bool TryParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) override;
        bool EndParse(Wrapper& wrapper, const Json::String& key) override;
        [[nodiscard]] std::unique_ptr<IJsonParseHelper> Create() const override;

    };
}

#include "JsonNestedObjectParseHelper.inl"
