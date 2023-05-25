#include "pch.h"
#include "AllScopeJsonParseHelper.h"

namespace FieaGameEngine::ScopeJsonParse {
    RTTI_DEFINITIONS(AllScopeJsonParseHelper);

    void AllScopeJsonParseHelper::Initialize() {
        _classHelper.Initialize();
        _typeHelper.Initialize();
        _expressionHelper.Initialize();
        _intHelper.Initialize();
        _floatHelper.Initialize();
        _vectorHelper.Initialize();
        _matrixHelper.Initialize();
        _stringHelper.Initialize();
    }

    void AllScopeJsonParseHelper::Cleanup() {
        _classHelper.Cleanup();
        _typeHelper.Cleanup();
        _expressionHelper.Cleanup();
        _intHelper.Cleanup();
        _floatHelper.Cleanup();
        _vectorHelper.Cleanup();
        _matrixHelper.Cleanup();
        _stringHelper.Cleanup();
    }

    bool AllScopeJsonParseHelper::EndParse(Wrapper& wrapper, const Json::String& key) {
        bool classEnd = _classHelper.EndParse(wrapper, key);
        bool typeEnd = _typeHelper.EndParse(wrapper, key);
        bool expressionEnd = _expressionHelper.EndParse(wrapper, key);
        bool intEnd = _intHelper.EndParse(wrapper, key);
        bool floatEnd = _floatHelper.EndParse(wrapper, key);
        bool vectorEnd = _vectorHelper.EndParse(wrapper, key);
        bool matrixEnd = _matrixHelper.EndParse(wrapper, key);
        bool stringEnd = _stringHelper.EndParse(wrapper, key);

        return classEnd || typeEnd || expressionEnd || intEnd || floatEnd || vectorEnd || matrixEnd || stringEnd;
    }
}
