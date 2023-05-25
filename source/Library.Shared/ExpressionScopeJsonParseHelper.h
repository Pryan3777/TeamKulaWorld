#pragma once
#include <regex>
#include "ScopeJsonParseHelper.h"
#include "ScopeParseWrapper.h"
#include "ShuntingYardParser.h"

namespace FieaGameEngine::ScopeJsonParse {
    class ExpressionScopeJsonParseHelper final : public ScopeJsonParseHelper {
        RTTI_DECLARATIONS(ExpressionScopeJsonParseHelper, ScopeJsonParseHelper);

    public:
        [[nodiscard]] bool IsAbleToParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) override;
        bool TryParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) override;
        bool EndParse(Wrapper& wrapper, const Json::String& key) override;
        [[nodiscard]] std::unique_ptr<IJsonParseHelper> Create() const override;

    private:
        static const Json::String EXPRESSION_BEGIN;
        static const Json::String EXPRESSION_END;
        static const std::regex EXTRACT_EXPRESSION;

        [[nodiscard]] bool IsAbleToParse(
            Wrapper& wrapper,
            const Json::String& key,
            const Json::Value& value,
            Json::ArrayIndex index,
            ScopeParseWrapper*& converted,
            Scope::key_type& scopeKey,
            Datum::String& extractedExpression
        );

    };
}

#include "ExpressionScopeJsonParseHelper.inl"
