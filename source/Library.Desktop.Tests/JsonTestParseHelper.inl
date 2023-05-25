#pragma once
#include "JsonTestParseHelper.h"

namespace LibraryDesktopTests {
    inline bool JsonTestParseHelper::TryParse(Wrapper&, const Json::String&, const Json::Value&, bool, Json::ArrayIndex) {
        ++_tryParseCallsCount;
        return true;
    }

    inline bool JsonTestParseHelper::EndParse(Wrapper& wrapper, const Json::String& key) {
        ++_endParseCallsCount;
        return FieaGameEngine::IJsonParseHelper::EndParse(wrapper, key);
    }
}
