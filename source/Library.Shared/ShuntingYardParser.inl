#pragma once
#include "ShuntingYardParser.h"

namespace FieaGameEngine {
    inline void ShuntingYardParser::Clear() { _operators.Clear(); }
    inline bool ShuntingYardParser::IsContainingOperator(const std::string& id) const { return _operators.IsContainingKey(id); }
    inline void ShuntingYardParser::AddOperator(const std::string& id, int precedence, bool isLeftAssociative) {
        AddOperator(id, OperatorDetails{precedence, isLeftAssociative});
    }
    inline void ShuntingYardParser::AddOperator(const std::string& id, OperatorDetails details) { _operators.Insert(std::make_pair(id, details)); }
    inline void ShuntingYardParser::RemoveOperator(const std::string& id) { _operators.Remove(id); }
    inline const std::string& ShuntingYardParser::GetLeftParenthesis() const { return _leftParenthesis; }
    inline const std::string& ShuntingYardParser::GetRightParenthesis() const { return _rightParenthesis; }
    inline const std::string& ShuntingYardParser::GetComma() const { return _comma; }
    inline void ShuntingYardParser::SetLeftParenthesis(const std::string& value) { _leftParenthesis = value; }
    inline void ShuntingYardParser::SetRightParenthesis(const std::string& value) { _rightParenthesis = value; }
    inline void ShuntingYardParser::SetComma(const std::string& value) { _comma = value; }
}
