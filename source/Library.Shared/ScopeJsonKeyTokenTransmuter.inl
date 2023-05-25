#pragma once
#include "ScopeJsonKeyTokenTransmuter.h"

namespace FieaGameEngine::ScopeJsonParse {
    inline void ScopeJsonKeyTokenTransmuter::Initialize() { IJsonValueTransmuter::Initialize(); _classHelper.Initialize(); _typeHelper.Initialize(); }
    inline void ScopeJsonKeyTokenTransmuter::Cleanup() { IJsonValueTransmuter::Cleanup(); _classHelper.Cleanup(); _typeHelper.Cleanup(); }
    inline std::unique_ptr<IJsonValueTransmuter> ScopeJsonKeyTokenTransmuter::Create() const { return std::make_unique<ScopeJsonKeyTokenTransmuter>(); }

    inline bool ScopeJsonKeyTokenTransmuter::IsAbleToTransmute(Wrapper& wrapper, const Json::String& key, const Json::Value&) {
        const std::regex isContainingSpace{"\\s*[^\\s]+\\s+[^\\s]+.*"};
        return wrapper.Is(ScopeParseWrapper::TypeIdClass()) && std::regex_match(key, isContainingSpace);
    }
}
