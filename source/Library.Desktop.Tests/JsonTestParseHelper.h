#pragma once
#include "IJsonParseHelper.h"

namespace LibraryDesktopTests {
    class JsonTestParseHelper : public FieaGameEngine::IJsonParseHelper {
        RTTI_DECLARATIONS(JsonTestParseHelper, FieaGameEngine::IJsonParseHelper);

    private:
        bool _isInitialized{false};
        int _tryParseCallsCount{0};
        int _endParseCallsCount{0};

    public:
        virtual ~JsonTestParseHelper() = default;

        virtual inline void Initialize() override { FieaGameEngine::IJsonParseHelper::Initialize(); _isInitialized = true; }
        virtual void Cleanup() override {
            FieaGameEngine::IJsonParseHelper::Cleanup();
            _tryParseCallsCount = 0;
            _endParseCallsCount = 0;
            _isInitialized = false;
        }
        virtual bool TryParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) override = 0;
        virtual bool EndParse(Wrapper& wrapper, const Json::String& key) override = 0;

        inline bool IsInitialized() const { return _isInitialized; }
        inline int TryParseCallsCount() const { return _tryParseCallsCount; }
        inline int EndParseCallsCount() const { return _endParseCallsCount; }

    };
}

#include "JsonTestParseHelper.inl"
