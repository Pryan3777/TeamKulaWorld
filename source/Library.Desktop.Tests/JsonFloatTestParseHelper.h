#pragma once
#include "JsonTestParseHelper.h"
#include "NamedFloatArrayTestParseWrapper.h"

namespace LibraryDesktopTests {
    class JsonFloatTestParseHelper final : public JsonTestParseHelper {
        RTTI_DECLARATIONS(JsonFloatTestParseHelper, JsonTestParseHelper);

    private:
        bool IsAbleToParse(
            Wrapper& wrapper,
            const Json::String& key,
            const Json::Value& value,
            bool isArrayElement,
            Json::ArrayIndex index,
            NamedFloatArrayTestParseWrapper*& converted
        );

    public:
        bool TryParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) override;

        inline bool IsAbleToParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) override {
            NamedFloatArrayTestParseWrapper* _;
            return IsAbleToParse(wrapper, key, value, isArrayElement, index, _);
        }
        inline bool EndParse(Wrapper& wrapper, const Json::String& key) override { return JsonTestParseHelper::EndParse(wrapper, key); }
        inline std::unique_ptr<IJsonParseHelper> Create() const override { return std::make_unique<JsonFloatTestParseHelper>(); }

    };
}
