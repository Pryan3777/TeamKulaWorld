#include "pch.h"
#include "JsonFloatTestParseHelper.h"

namespace LibraryDesktopTests {
    RTTI_DEFINITIONS(JsonFloatTestParseHelper);

    bool JsonFloatTestParseHelper::IsAbleToParse(
        Wrapper& wrapper,
        const Json::String&,
        const Json::Value& value,
        bool isArrayElement,
        Json::ArrayIndex index,
        NamedFloatArrayTestParseWrapper*& converted
    ) {
        converted = wrapper.As<NamedFloatArrayTestParseWrapper>();

        if (converted == nullptr) {
            return false;
        }

        return value.isNumeric() && ((!isArrayElement) || (index == Json::ArrayIndex(0))) && converted->Floats.IsEmpty();
    }

    bool JsonFloatTestParseHelper::TryParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) {
        bool baseSucceeded = JsonTestParseHelper::TryParse(wrapper, key, value, isArrayElement, index);

        NamedFloatArrayTestParseWrapper* floatWrapper;

        if (!IsAbleToParse(wrapper, key, value, isArrayElement, index, floatWrapper)) {
            return false;
        }

        float f = value.asFloat();
        floatWrapper->Floats.Clear();
        floatWrapper->Floats.PushBack(f);
        floatWrapper->Name = key;

        return baseSucceeded;
    }
}
