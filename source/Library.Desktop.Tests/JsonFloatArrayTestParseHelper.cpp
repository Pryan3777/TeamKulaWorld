#include "pch.h"
#include "JsonFloatArrayTestParseHelper.h"

namespace LibraryDesktopTests {
    RTTI_DEFINITIONS(JsonFloatArrayTestParseHelper);

    bool JsonFloatArrayTestParseHelper::IsAbleToParse(
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

        return isArrayElement && value.isNumeric() && (converted->Floats.Size() >= index);
    }

    bool JsonFloatArrayTestParseHelper::TryParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) {
        bool baseSucceeded = JsonTestParseHelper::TryParse(wrapper, key, value, isArrayElement, index);

        NamedFloatArrayTestParseWrapper* floatWrapper;

        if (!IsAbleToParse(wrapper, key, value, isArrayElement, index, floatWrapper)) {
            return false;
        }

        float f = value.asFloat();

        if (floatWrapper->Floats.Size() <= index) {
            floatWrapper->Floats.PushBack(f);
        } else {
            floatWrapper->Floats[index] = f;
        }

        floatWrapper->Name = key;

        return baseSucceeded;
    }
}
