#pragma once
#include "IJsonParseHelper.h"
#include "ScopeParseWrapper.h"

namespace FieaGameEngine::ScopeJsonParse {
    class ScopeJsonParseHelper : public IJsonParseHelper {
        RTTI_DECLARATIONS(ScopeJsonParseHelper, IJsonParseHelper);

    protected:
        [[nodiscard]] virtual bool IsAbleToParse(Wrapper& wrapper, ScopeParseWrapper*& converted);
        void UpdateAndValidateWrapperStackFrame(ScopeParseWrapper::StackFrame& frame, const Json::String& key) const;

    };
}
