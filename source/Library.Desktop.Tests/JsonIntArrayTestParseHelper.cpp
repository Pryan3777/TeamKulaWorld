#include "pch.h"
#include "JsonIntArrayTestParseHelper.h"

namespace LibraryDesktopTests {
    RTTI_DEFINITIONS(JsonIntArrayTestParseHelper);

    bool JsonIntArrayTestParseHelper::IsAbleToParse(
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

        return isArrayElement && value.isInt() && (converted->Ints.Size() >= index);
    }

    bool JsonIntArrayTestParseHelper::TryParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) {
        bool baseSucceeded = JsonTestParseHelper::TryParse(wrapper, key, value, isArrayElement, index);

        NamedIntArrayTestParseWrapper* intWrapper;

        if (!IsAbleToParse(wrapper, key, value, isArrayElement, index, intWrapper)) {
            return false;
        }

        Json::Value::Int i = value.asInt();

        if (intWrapper->Ints.Size() <= index) {
            intWrapper->Ints.PushBack(i);
        } else {
            intWrapper->Ints[index] = i;
        }

        intWrapper->Name = key;

        return baseSucceeded;
    }
}
