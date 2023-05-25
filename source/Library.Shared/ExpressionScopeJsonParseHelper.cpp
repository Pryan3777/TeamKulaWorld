#include "pch.h"
#include "ExpressionScopeJsonParseHelper.h"
#include <regex>

using namespace std::literals::string_literals;

namespace FieaGameEngine::ScopeJsonParse {
    RTTI_DEFINITIONS(ExpressionScopeJsonParseHelper);

    const Json::String ExpressionScopeJsonParseHelper::EXPRESSION_BEGIN = "<<<"s;
    const Json::String ExpressionScopeJsonParseHelper::EXPRESSION_END = ">>>"s;
    const std::regex ExpressionScopeJsonParseHelper::EXTRACT_EXPRESSION{EXPRESSION_BEGIN + "(.+)"s + EXPRESSION_END};

    bool ExpressionScopeJsonParseHelper::IsAbleToParse(
        Wrapper& wrapper,
        const Json::String& key,
        const Json::Value& value,
        Json::ArrayIndex index,
        ScopeParseWrapper*& converted,
        Scope::key_type& scopeKey,
        Datum::String& extractedExpression
    ) {
        if (!ScopeJsonParseHelper::IsAbleToParse(wrapper, converted) || !(value.isString())) {
            scopeKey = key;
            return false;
        }

        auto str = value.asString();
        std::smatch stringMatch{};

        if (!std::regex_match(str, stringMatch, EXTRACT_EXPRESSION)) {
            return false;
        }

        extractedExpression = stringMatch[1].str();

        auto cframe = converted->CTopStackFrame();
        scopeKey = (key == ScopeParseWrapper::KEYWORD_VALUE) ? cframe->_name : key;
        auto cfound = cframe->_scope->CFind(scopeKey);

        if (cfound == cframe->_scope->cend()) {
            return index == Json::ArrayIndex(0);
        }

        return (cfound->second.Size() >= Datum::size_type(index)) && (cfound->second.ActualType() == Datum::DatumType::String);
    }

    bool ExpressionScopeJsonParseHelper::TryParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool, Json::ArrayIndex index) {
        ScopeParseWrapper* scopeWrapper;
        Scope::key_type scopeKey;
        Datum::String extractedExpression;

        if (IsAbleToParse(wrapper, key, value, index, scopeWrapper, scopeKey, extractedExpression)) {
            auto frame = scopeWrapper->TopStackFrame();
            UpdateAndValidateWrapperStackFrame(*frame, key);
            auto& appended = frame->_scope->Append(scopeKey);
            appended.SetType(Datum::DatumType::String);

            auto s = Datum::size_type(index);
            if (appended.Size() > s) {
                appended.SetElement(scopeWrapper->GetShuntingYardParser().Parse(extractedExpression), s);
            } else {
                assert(appended.Size() == s);
                appended.PushBack(scopeWrapper->GetShuntingYardParser().Parse(extractedExpression));
            }

            return true;
        }

        return false;
    }
}
