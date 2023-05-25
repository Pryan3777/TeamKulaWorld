#include "pch.h"
#include "JsonNestedObjectParseHelper.h"

namespace FieaGameEngine {
    RTTI_DEFINITIONS(JsonNestedObjectParseHelper);

    bool JsonNestedObjectParseHelper::TryParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) {
        if (!IsAbleToParse(wrapper, key, value, isArrayElement, index)) {
            return false;
        }

        bool isIncremented = wrapper.IncrementDepth(key, isArrayElement, std::size_t(index));
        assert(isIncremented);

        bool success = static_cast<JsonParseCoordinator*>(wrapper.Coordinator())->ParseObject(value);

        bool isDecremented = wrapper.DecrementDepth();
        assert(isDecremented);

        return isIncremented && success && isDecremented;
    }
}
