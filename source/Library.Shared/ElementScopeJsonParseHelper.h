#pragma once
#include "ScopeJsonParseHelper.h"
#include "ScopeParseWrapper.h"

namespace FieaGameEngine::ScopeJsonParse {
    template <typename T>
    class ElementScopeJsonParseHelper final : public ScopeJsonParseHelper {
        RTTI_DECLARATIONS(ElementScopeJsonParseHelper<T>, ScopeJsonParseHelper);

    public:
        [[nodiscard]] bool IsAbleToParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) override;
        bool TryParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) override;
        bool EndParse(Wrapper& wrapper, const Json::String& key) override;
        [[nodiscard]] std::unique_ptr<IJsonParseHelper> Create() const override;

    private:
        static constexpr bool __notImplemented = false;

        [[nodiscard]] bool IsAbleToParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, Json::ArrayIndex index, ScopeParseWrapper*& converted, Scope::key_type& scopeKey);
        [[nodiscard]] Datum::DatumType ElementDatumActualType() const;
        [[nodiscard]] void SetToDatumFromJsonValue(Datum& datum, Datum::size_type index, const Json::Value& value);
        [[nodiscard]] void PushBackToDatumFromJsonValue(Datum& datum, const Json::Value& value);
        [[nodiscard]] bool IsJsonValueCorrectType(const Json::Value& value) const;
        [[nodiscard]] bool IsJsonValueAsStringCorrectlyFormatted(const Json::Value& value) const;

    };
}

#include "ElementScopeJsonParseHelper.inl"
