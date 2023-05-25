#include "pch.h"
#include "JsonTypeTestParseHelper.h"
#include "NamedIntArrayTestParseWrapper.h"
#include "NamedFloatArrayTestParseWrapper.h"
#include "NamedStringArrayTestParseWrapper.h"

namespace LibraryDesktopTests {
    RTTI_DEFINITIONS(JsonTypeTestParseHelper);

    bool JsonTypeTestParseHelper::IsType(const Json::String& key) {
        using namespace std::literals::string_literals;

        return (key == "type"s) || (key == "Type"s) || (key == "TYPE"s);
    }

    bool JsonTypeTestParseHelper::IsInteger(const Json::String& value) {
        using namespace std::literals::string_literals;

        return (value == "int"s) || (value == "Int"s) || (value == "INT"s)
            || (value == "integer"s) || (value == "Integer"s) || (value == "INTEGER"s);
    }

    bool JsonTypeTestParseHelper::IsFloat(const Json::String& value) {
        using namespace std::literals::string_literals;

        return (value == "flt"s) || (value == "Flt"s) || (value == "FLT"s)
            || (value == "float"s) || (value == "Float"s) || (value == "FLOAT"s);
    }

    bool JsonTypeTestParseHelper::IsString(const Json::String& value) {
        using namespace std::literals::string_literals;

        return (value == "str"s) || (value == "Str"s) || (value == "STR"s)
            || (value == "string"s) || (value == "String"s) || (value == "STRING"s);
    }

    bool JsonTypeTestParseHelper::TryParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) {
        bool success = JsonTestParseHelper::TryParse(wrapper, key, value, isArrayElement, index);

        if (!IsAbleToParse(wrapper, key, value, isArrayElement, index)) {
            return false;
        }

        Json::String str = value.asString();

        if (IsInteger(str)) {
            success &= wrapper.Is(NamedIntArrayTestParseWrapper::TypeIdClass());
        } else if (IsFloat(str)) {
            success &= wrapper.Is(NamedFloatArrayTestParseWrapper::TypeIdClass());
        } else if (IsString(str)) {
            success &= wrapper.Is(NamedStringArrayTestParseWrapper::TypeIdClass());
        } else {
            success = false;
        }

        return success;
    }
}
