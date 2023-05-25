#pragma once
#include "IJsonValueTransmuter.h"
#include "ClassScopeJsonParseHelper.h"
#include "TypeScopeJsonParseHelper.h"
#include "ScopeParseWrapper.h"

namespace FieaGameEngine::ScopeJsonParse {
    class ScopeJsonKeyTokenTransmuter final : public IJsonValueTransmuter {
        RTTI_DECLARATIONS(ScopeJsonKeyTokenTransmuter, IJsonValueTransmuter);

    public:
        void Initialize() override;
        void Cleanup() override;

        pair_type Transmute(Wrapper& wrapper, const Json::String& key, const Json::Value& value) override;
        pair_type EndTransmutation(Wrapper& wrapper, const Json::String& key, const Json::Value& value) override;

        std::unique_ptr<IJsonValueTransmuter> Create() const override;

    protected:
        class ScopeJsonTransmuteHistory : public TransmuteHistory {

        public:
            inline ScopeJsonTransmuteHistory(const pair_type& input, bool isAbleToTransmute, Wrapper::depth_type depth) : TransmuteHistory{input, isAbleToTransmute}, _depth{depth} {}

            inline Wrapper::depth_type Depth() const { return _depth; }

        private:
            Wrapper::depth_type _depth;
        };

    private:
        ClassScopeJsonParseHelper _classHelper{};
        TypeScopeJsonParseHelper _typeHelper{};

        bool IsAbleToTransmute(Wrapper& wrapper, const Json::String& key, const Json::Value& value);
        void ParseKeyToken(Wrapper& wrapper, const std::string& token, std::size_t tokenIndex, std::size_t remainingTokenCount);
        bool IsTokenValidDatumType(const std::string& token) const;

    };
}

#include "ScopeJsonKeyTokenTransmuter.inl"
