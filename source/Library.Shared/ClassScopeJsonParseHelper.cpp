#include "pch.h"
#include "ClassScopeJsonParseHelper.h"

namespace FieaGameEngine::ScopeJsonParse {
    RTTI_DEFINITIONS(ClassScopeJsonParseHelper);

    bool ClassScopeJsonParseHelper::IsAbleToParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, ScopeParseWrapper*& converted) {
        if (!ScopeJsonParseHelper::IsAbleToParse(wrapper, converted)) {
            return false;
        }

        return (!isArrayElement) && wrapper.Is(ScopeParseWrapper::TypeIdClass()) && (key == ScopeParseWrapper::KEYWORD_CLASS) && value.isString();
    }

    bool ClassScopeJsonParseHelper::TryParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex) {
        ScopeParseWrapper* converted = nullptr;

        if (!IsAbleToParse(wrapper, key, value, isArrayElement, converted)) {
            return false;
        }

        auto asstring = value.asString();
        auto frame = converted->TopStackFrame();
        auto found = frame->_scope->CFind(frame->_name);
        if ((frame->_class != asstring)
            && (found != frame->_scope->cend())
            && ((frame->_index) < (found->second.Size()))
            && (found->second.CGetTableElement(frame->_index).TypeNameInstance() != asstring)
        ) {
            using namespace std::literals::string_literals;

            throw std::runtime_error("Scope object was parsed before class tag, cannot complete the operation!"s);
        }

        converted->TopStackFrame()->_class = asstring;
        UpdateAndValidateWrapperStackFrame(*frame, key);

        return true;
    }
}
