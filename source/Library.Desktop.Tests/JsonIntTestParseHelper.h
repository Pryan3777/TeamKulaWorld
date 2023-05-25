#pragma once
#include "JsonTestParseHelper.h"
#include "NamedIntArrayTestParseWrapper.h"

namespace LibraryDesktopTests {
    class JsonIntTestParseHelper final : public JsonTestParseHelper {
        RTTI_DECLARATIONS(JsonIntTestParseHelper, JsonTestParseHelper);

    private:
        bool IsAbleToParse(
            Wrapper& wrapper,
            const Json::String& key,
            const Json::Value& value,
            bool isArrayElement,
            Json::ArrayIndex index,
            NamedIntArrayTestParseWrapper*& converted
        );

    public:
        bool TryParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) override;

        inline bool IsAbleToParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) override {
            NamedIntArrayTestParseWrapper* _;
            return IsAbleToParse(wrapper, key, value, isArrayElement, index, _);
        }
        inline bool EndParse(Wrapper& wrapper, const Json::String& key) override { return JsonTestParseHelper::EndParse(wrapper, key); }
        inline std::unique_ptr<IJsonParseHelper> Create() const override { return std::make_unique<JsonIntTestParseHelper>(); }

    };
}
