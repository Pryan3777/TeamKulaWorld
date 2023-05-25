#pragma once
#include "IJsonParseHelper.h"
#include "ClassScopeJsonParseHelper.h"
#include "TypeScopeJsonParseHelper.h"
#include "ElementScopeJsonParseHelper.h"
#include "ExpressionScopeJsonParseHelper.h"

namespace FieaGameEngine::ScopeJsonParse {
    class AllScopeJsonParseHelper final : public IJsonParseHelper {
        RTTI_DECLARATIONS(AllScopeJsonParseHelper, IJsonParseHelper);

    public:
        void Initialize() override;
        void Cleanup() override;
        bool IsAbleToParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) override;
        bool TryParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) override;
        bool EndParse(Wrapper& wrapper, const Json::String& key) override;
        std::unique_ptr<IJsonParseHelper> Create() const override;

    private:
        ClassScopeJsonParseHelper _classHelper{};
        TypeScopeJsonParseHelper _typeHelper{};
        ExpressionScopeJsonParseHelper _expressionHelper{};
        ElementScopeJsonParseHelper<Datum::Integer> _intHelper{};
        ElementScopeJsonParseHelper<Datum::Float> _floatHelper{};
        ElementScopeJsonParseHelper<Datum::Vector> _vectorHelper{};
        ElementScopeJsonParseHelper<Datum::Matrix> _matrixHelper{};
        ElementScopeJsonParseHelper<Datum::String> _stringHelper{};

    };
}

#include "AllScopeJsonParseHelper.inl"
