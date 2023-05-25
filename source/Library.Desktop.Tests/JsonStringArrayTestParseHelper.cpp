#include "pch.h"
#include "JsonStringArrayTestParseHelper.h"

namespace LibraryDesktopTests {
    RTTI_DEFINITIONS(JsonStringArrayTestParseHelper);

    bool JsonStringArrayTestParseHelper::IsAbleToParse(
        Wrapper& wrapper,
        const Json::String&,
        const Json::Value& value,
        bool isArrayElement,
        Json::ArrayIndex index,
        NamedStringArrayTestParseWrapper*& converted
    ) {
        converted = wrapper.As<NamedStringArrayTestParseWrapper>();

        if (converted == nullptr) {
            return false;
        }

        return isArrayElement && value.isString() && (converted->Strings.Size() >= index);
    }

    bool JsonStringArrayTestParseHelper::TryParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) {
        bool baseSucceeded = JsonTestParseHelper::TryParse(wrapper, key, value, isArrayElement, index);

        NamedStringArrayTestParseWrapper* stringWrapper;

        if (!IsAbleToParse(wrapper, key, value, isArrayElement, index, stringWrapper)) {
            return false;
        }

        Json::String s = value.asString();

        if (stringWrapper->Strings.Size() <= index) {
            stringWrapper->Strings.PushBack(s);
        } else {
            stringWrapper->Strings[index] = s;
        }

        stringWrapper->Name = key;

        return baseSucceeded;
    }
}
