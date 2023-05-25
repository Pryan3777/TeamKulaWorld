#include "pch.h"
#include "ScopeJsonParseHelper.h"

namespace FieaGameEngine::ScopeJsonParse {
    RTTI_DEFINITIONS(ScopeJsonParseHelper);

    bool ScopeJsonParseHelper::IsAbleToParse(Wrapper& wrapper, ScopeParseWrapper*& converted) {
        converted = wrapper.As<ScopeParseWrapper>();
        assert((converted != nullptr) && !(converted->IsEmpty()));
        return converted != nullptr;
    }

    void ScopeJsonParseHelper::UpdateAndValidateWrapperStackFrame(ScopeParseWrapper::StackFrame& frame, const Json::String& key) const {
        using namespace std::literals::string_literals;
        using KeywordState = ScopeParseWrapper::StackFrame::KeywordState;

        auto newKeywordState = ScopeParseWrapper::IsKeyKeyword(key) ? KeywordState::Keyworded : KeywordState::NotKeyworded;

        if (frame._keywordState == KeywordState::Undecided) {
            frame._keywordState = newKeywordState;

            if (newKeywordState == KeywordState::Keyworded) {
                frame._scope->Append(frame._name);
            } else if (frame._name != ScopeParseWrapper::KEYWORD_VALUE) {
                auto found = frame._scope->Find(frame._name);
                frame._scope = ((found == frame._scope->end()) || (found->second.Size() >= frame._index))
                    ? &(frame._scope->AppendScope(frame._name, frame._class))
                    : &(found->second.GetTableElement(frame._index));
            }
        } else if (frame._keywordState != newKeywordState) {
            throw std::logic_error("Cannot mix keyworded members with non-keyworded members!"s);
        }

        if ((newKeywordState == KeywordState::Keyworded) && (frame._name == ScopeParseWrapper::KEYWORD_VALUE)) {
            throw std::logic_error("A keyworded stack frame cannot have a direct child which is also a keyworded stack frame!"s);
        }
    }
}
