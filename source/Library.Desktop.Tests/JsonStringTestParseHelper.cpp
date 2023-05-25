#include "pch.h"
#include "JsonStringTestParseHelper.h"

namespace LibraryDesktopTests {
    RTTI_DEFINITIONS(JsonStringTestParseHelper);

    bool JsonStringTestParseHelper::IsAbleToParse(
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

        return value.isString() && ((!isArrayElement) || (index == Json::ArrayIndex(0))) && converted->Strings.IsEmpty();
    }

    bool JsonStringTestParseHelper::TryParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) {
        bool baseSucceeded = JsonTestParseHelper::TryParse(wrapper, key, value, isArrayElement, index);

        NamedStringArrayTestParseWrapper* stringWrapper;

        if (!IsAbleToParse(wrapper, key, value, isArrayElement, index, stringWrapper)) {
            return false;
        }

        Json::String s = value.asString();
        stringWrapper->Strings.Clear();
        stringWrapper->Strings.PushBack(s);
        stringWrapper->Name = key;

        return baseSucceeded;
    }
}
