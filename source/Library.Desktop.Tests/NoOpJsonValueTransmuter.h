#pragma once
#include "IJsonValueTransmuter.h"

namespace LibraryDesktopTests {
    class NoOpJsonValueTransmuter final : public FieaGameEngine::IJsonValueTransmuter {
        RTTI_DECLARATIONS(NoOpJsonValueTransmuter, FieaGameEngine::IJsonValueTransmuter);

        inline pair_type Transmute(Wrapper&, const Json::String& key, const Json::Value& value) override { return std::make_pair(key, std::cref(value)); }
        inline pair_type EndTransmutation(Wrapper&, const Json::String& key, const Json::Value& value) override { return std::make_pair(key, std::cref(value)); }
        inline std::unique_ptr<IJsonValueTransmuter> Create() const override { return std::make_unique<NoOpJsonValueTransmuter>(); }

    };
}
