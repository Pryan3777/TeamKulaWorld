#pragma once
#include "ScopeJsonParseHelper.h"
#include "ScopeParseWrapper.h"

namespace FieaGameEngine::ScopeJsonParse {
    class ClassScopeJsonParseHelper final : public ScopeJsonParseHelper {
        RTTI_DECLARATIONS(ClassScopeJsonParseHelper, ScopeJsonParseHelper);

    public:
        [[nodiscard]] bool IsAbleToParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) override;
        bool TryParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) override;
        bool EndParse(Wrapper& wrapper, const Json::String& key) override;
        [[nodiscard]] std::unique_ptr<IJsonParseHelper> Create() const override;

    private:
        [[nodiscard]] bool IsAbleToParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, ScopeParseWrapper*& converted);

    };
}

#include "ClassScopeJsonParseHelper.inl"
