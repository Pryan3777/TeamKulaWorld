#pragma once
#include "ScopeParseWrapper.h"
#include <cassert>

namespace FieaGameEngine {
    inline ScopeParseWrapper::StackFrame::StackFrame(Scope* scope, const std::string& name, std::size_t index)
        : _scope{scope}, _name{name}, _class{Scope::TypeNameClass()}, _index{index} {}

    inline ScopeParseWrapper::ScopeParseWrapper(std::shared_ptr<Scope> scope) : ScopeParseWrapper{scope, nullptr} {}
    inline ScopeParseWrapper::ScopeParseWrapper(std::shared_ptr<Scope> scope, ParseCoordinator* coordinator)
        : ParseCoordinator::Wrapper{coordinator}
        , _scope{std::move(scope)}
    {
        using namespace std::literals::string_literals;

        _stack.Push(StackFrame{_scope.get(), ""s});
    }

    inline bool ScopeParseWrapper::IsKeyKeyword(const std::string& key) {
        return (key == ScopeParseWrapper::KEYWORD_TYPE) || (key == ScopeParseWrapper::KEYWORD_CLASS) || (key == ScopeParseWrapper::KEYWORD_VALUE);
    }

    inline bool ScopeParseWrapper::DecrementDepth() { _stack.Pop(); return ParseCoordinator::Wrapper::DecrementDepth(); }

    inline bool ScopeParseWrapper::IsEmpty() const { return _stack.IsEmpty(); }

    inline const Stack<typename ScopeParseWrapper::StackFrame>::iterator ScopeParseWrapper::TopStackFrame() { return _stack.Top(); }
    inline const Stack<typename ScopeParseWrapper::StackFrame>::const_iterator ScopeParseWrapper::TopStackFrame() const { return CTopStackFrame(); }
    inline const Stack<typename ScopeParseWrapper::StackFrame>::const_iterator ScopeParseWrapper::CTopStackFrame() const { return _stack.CTop(); }

    inline bool ScopeParseWrapper::CreateShuntingYardParser() {
        return CreateShuntingYardParser(true, ShuntingYardParser::LEFT_PARENTHESIS, ShuntingYardParser::RIGHT_PARENTHESIS, ShuntingYardParser::COMMA, {});
    }
    inline bool ScopeParseWrapper::CreateShuntingYardParser(std::string leftParenthesis, std::string rightParenthesis, std::string comma) {
        return CreateShuntingYardParser(true, std::move(leftParenthesis), std::move(rightParenthesis), std::move(comma), {});
    }
    inline bool ScopeParseWrapper::CreateShuntingYardParser(std::initializer_list<std::pair<std::string, ShuntingYardParser::OperatorDetails>> list) {
        return CreateShuntingYardParser(
            true,
            ShuntingYardParser::LEFT_PARENTHESIS,
            ShuntingYardParser::RIGHT_PARENTHESIS,
            ShuntingYardParser::COMMA,
            std::move(list)
        );
    }
    inline bool ScopeParseWrapper::CreateShuntingYardParser(
        std::string leftParenthesis,
        std::string rightParenthesis,
        std::string comma,
        std::initializer_list<std::pair<std::string, ShuntingYardParser::OperatorDetails>> list
    ) {
        return CreateShuntingYardParser(true, std::move(leftParenthesis), std::move(rightParenthesis), std::move(comma), std::move(list));
    }
    inline void ScopeParseWrapper::DestroyShuntingYardParser() { _shuntingYardParser.reset(); }
    inline const ShuntingYardParser& ScopeParseWrapper::GetShuntingYardParser() const { assert(_shuntingYardParser); return *_shuntingYardParser; }
}
