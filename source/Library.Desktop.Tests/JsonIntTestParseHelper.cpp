#include "pch.h"
#include "JsonIntTestParseHelper.h"

namespace LibraryDesktopTests {
    RTTI_DEFINITIONS(JsonIntTestParseHelper);

    bool JsonIntTestParseHelper::IsAbleToParse(
        Wrapper& wrapper,
        const Json::String&,
        const Json::Value& value,
        bool isArrayElement,
        Json::ArrayIndex index,
        NamedIntArrayTestParseWrapper*& converted
    ) {
        converted = wrapper.As<NamedIntArrayTestParseWrapper>();

        if (converted == nullptr) {
            return false;
        }

        return value.isInt() && ((!isArrayElement) || (index == Json::ArrayIndex(0))) && converted->Ints.IsEmpty();
    }

    bool JsonIntTestParseHelper::TryParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) {
        bool baseSucceeded = JsonTestParseHelper::TryParse(wrapper, key, value, isArrayElement, index);

        NamedIntArrayTestParseWrapper* intWrapper;

        if (!IsAbleToParse(wrapper, key, value, isArrayElement, index, intWrapper)) {
            return false;
        }

        Json::Value::Int i = value.asInt();
        intWrapper->Ints.Clear();
        intWrapper->Ints.PushBack(i);
        intWrapper->Name = key;

        return baseSucceeded;
    }
}
