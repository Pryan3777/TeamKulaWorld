#include "pch.h"
#include "ScopeParseWrapper.h"

using namespace std::literals::string_literals;

namespace FieaGameEngine {
    RTTI_DEFINITIONS(ScopeParseWrapper);

    const std::string ScopeParseWrapper::KEYWORD_TYPE = "__type"s;
    const std::string ScopeParseWrapper::KEYWORD_CLASS = "__class"s;
    const std::string ScopeParseWrapper::KEYWORD_VALUE = "__value"s;
    const std::string ScopeParseWrapper::TYPE_OBJECT = "object"s;

    ScopeParseWrapper::ScopeParseWrapper(ScopeParseWrapper&& other) noexcept
        : ParseCoordinator::Wrapper{std::forward<ParseCoordinator::Wrapper>(other)}
        , _scope{std::move(other._scope)}
        , _stack{std::move(other._stack)}
    {
        other._depth = depth_type(0);
        other._scope.reset();
        other._stack.Clear();
    }

    ScopeParseWrapper& ScopeParseWrapper::operator=(ScopeParseWrapper&& other) noexcept {
        if (this != &other) {
            _stack.Clear();
            _scope.reset();
            ParseCoordinator::Wrapper::operator=(std::forward<ScopeParseWrapper>(other));
            other._depth = depth_type(0);
            other._scope.reset();
            other._stack.Clear();
        }

        return *this;
    }

    void ScopeParseWrapper::HandleSubobjectNameAndIndex(const std::string& subobjectName, bool isArray, std::size_t arrayIndex) {
        if (subobjectName.empty()) {
            using namespace std::literals::string_literals;

            throw std::invalid_argument("Subobject name cannot be empty!"s);
        }

        auto frame = _stack.Top();
        auto* newFrameScope = frame->_scope;

        bool isSubobjectValueKeyword = subobjectName == KEYWORD_VALUE;
        arrayIndex = isArray ? arrayIndex : (isSubobjectValueKeyword ? frame->_index : std::size_t(0));

        if (isSubobjectValueKeyword || ((frame->_keywordState == StackFrame::KeywordState::Undecided) && (frame->_name != KEYWORD_VALUE))) {
            assert(!IsKeyKeyword(frame->_name));
            assert(frame->_keywordState != StackFrame::KeywordState::NotKeyworded);

            // subobject names cannot be empty. therefore the only empty frame name should be the root, which does not need appending
            if (!(frame->_name.empty())) {
                auto found = frame->_scope->Find(frame->_name);
                newFrameScope = ((found == frame->_scope->end()) || (found->second.Size() == arrayIndex))
                    ? &(frame->_scope->AppendScope(frame->_name, frame->_class))
                    : &(found->second.GetTableElement(arrayIndex));
                assert((found == frame->_scope->end()) || (found->second.Size() >= arrayIndex));

                if (!isSubobjectValueKeyword) {
                    // This if statement was introduced to address a bug which is only visible when parsing a "__value" subobject array.
                    // "__value" subobjects, array elements or otherwise, never need to update THEIR keyworded stack frame with the newly appended scope.
                    // The stack frame which is about to be created has the appropriate newly appended scope, so no functionality is lost.
                    frame->_scope = newFrameScope;
                }
            }

            frame->_keywordState = isSubobjectValueKeyword ? StackFrame::KeywordState::Keyworded : StackFrame::KeywordState::NotKeyworded;
        }

        _stack.Push(StackFrame{newFrameScope, subobjectName, arrayIndex});
    }

    typename ScopeParseWrapper::WrapperSharedPointer ScopeParseWrapper::Create() const {
        WrapperSharedPointer clone = std::make_shared<ScopeParseWrapper>(_scope->Clone(), Coordinator());
        ScopeParseWrapper* pointer = clone->As<ScopeParseWrapper>();
        pointer->_scope->Clear();

        return clone;
    }

    bool ScopeParseWrapper::CreateShuntingYardParser(
        bool useDefaultConfiguration,
        std::string leftParenthesis,
        std::string rightParenthesis,
        std::string comma,
        std::initializer_list<std::pair<std::string, ShuntingYardParser::OperatorDetails>> operatorOverrides
    ) {
        if (_shuntingYardParser) {
            return false;
        }

        _shuntingYardParser = std::make_unique<ShuntingYardParser>(useDefaultConfiguration);
        _shuntingYardParser->SetLeftParenthesis(std::move(leftParenthesis));
        _shuntingYardParser->SetRightParenthesis(std::move(rightParenthesis));
        _shuntingYardParser->SetComma(std::move(comma));

        for (auto& pair : operatorOverrides) {
            _shuntingYardParser->RemoveOperator(pair.first);
            _shuntingYardParser->AddOperator(pair.first, pair.second);
        }

        return true;
    }
}
