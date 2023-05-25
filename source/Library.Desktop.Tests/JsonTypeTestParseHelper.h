#pragma once
#include "JsonTestParseHelper.h"

namespace LibraryDesktopTests {
    class JsonTypeTestParseHelper final : public JsonTestParseHelper {
        RTTI_DECLARATIONS(JsonTypeTestParseHelper, JsonTestParseHelper);

    private:
        static bool IsType(const Json::String& key);
        static bool IsInteger(const Json::String& value);
        static bool IsFloat(const Json::String& value);
        static bool IsString(const Json::String& value);

    public:
        bool TryParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) override;

        inline bool IsAbleToParse(Wrapper&, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex) override {
            return IsType(key) && value.isString() && !isArrayElement;
        }
        inline bool EndParse(Wrapper& wrapper, const Json::String& key) override { return JsonTestParseHelper::EndParse(wrapper, key); }
        inline std::unique_ptr<IJsonParseHelper> Create() const override { return std::make_unique<JsonTypeTestParseHelper>(); }

    };
}
