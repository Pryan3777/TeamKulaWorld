#pragma once
#include "JsonTestParseHelper.h"
#include "NamedStringArrayTestParseWrapper.h"

namespace LibraryDesktopTests {
    class JsonStringTestParseHelper final : public JsonTestParseHelper {
        RTTI_DECLARATIONS(JsonStringTestParseHelper, JsonTestParseHelper);

    private:
        bool IsAbleToParse(
            Wrapper& wrapper,
            const Json::String& key,
            const Json::Value& value,
            bool isArrayElement,
            Json::ArrayIndex index,
            NamedStringArrayTestParseWrapper*& converted
        );

    public:
        bool TryParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) override;

        inline bool IsAbleToParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) override {
            NamedStringArrayTestParseWrapper* _;
            return IsAbleToParse(wrapper, key, value, isArrayElement, index, _);
        }
        inline bool EndParse(Wrapper& wrapper, const Json::String& key) override { return JsonTestParseHelper::EndParse(wrapper, key); }
        inline std::unique_ptr<IJsonParseHelper> Create() const override { return std::make_unique<JsonStringTestParseHelper>(); }

    };
}
