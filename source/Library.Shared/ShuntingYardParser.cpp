#include "pch.h"
#include "ShuntingYardParser.h"
#include <regex>
#include <sstream>
#include "Stack.h"

using namespace std::literals::string_literals;

namespace FieaGameEngine {
    const std::string ShuntingYardParser::LEFT_PARENTHESIS = "("s;
    const std::string ShuntingYardParser::RIGHT_PARENTHESIS = ")"s;
    const std::string ShuntingYardParser::COMMA = ","s;
    const std::string ShuntingYardParser::DEFAULT_ABS = "abs"s;
    const std::string ShuntingYardParser::DEFAULT_ADDITION = "+"s;
    const std::string ShuntingYardParser::DEFAULT_AND = "&&"s;
    const std::string ShuntingYardParser::DEFAULT_ASSIGN = "="s;
    const std::string ShuntingYardParser::DEFAULT_COSINE = "cos"s;
    const std::string ShuntingYardParser::DEFAULT_DECREMENT = "--";
    const std::string ShuntingYardParser::DEFAULT_DEGTORAD = "deg->rad"s;
    const std::string ShuntingYardParser::DEFAULT_DIVISION = "/"s;
    const std::string ShuntingYardParser::DEFAULT_EXPONENT = "^"s;
    const std::string ShuntingYardParser::DEFAULT_INCREMENT = "++"s;
    const std::string ShuntingYardParser::DEFAULT_IS_CONGRUENT = "==="s;
    const std::string ShuntingYardParser::DEFAULT_IS_EQUIVALENT = "=="s;
    const std::string ShuntingYardParser::DEFAULT_IS_GREATER = ">"s;
    const std::string ShuntingYardParser::DEFAULT_IS_LESS = "<"s;
    const std::string ShuntingYardParser::DEFAULT_MODULUS = "%"s;
    const std::string ShuntingYardParser::DEFAULT_MULTIPLICATION = "*"s;
    const std::string ShuntingYardParser::DEFAULT_NEGATE = "~"s;
    const std::string ShuntingYardParser::DEFAULT_OR = "||"s;
    const std::string ShuntingYardParser::DEFAULT_RADTODEG = "rad->deg"s;
    const std::string ShuntingYardParser::DEFAULT_SINE = "sin"s;
    const std::string ShuntingYardParser::DEFAULT_SUBTRACTION = "-"s;
    const std::string ShuntingYardParser::DEFAULT_TANGENT = "tan"s;

    ShuntingYardParser::ShuntingYardParser(bool useDefaultConfiguration)
        : _operators{}
        , _leftParenthesis{LEFT_PARENTHESIS}
        , _rightParenthesis{RIGHT_PARENTHESIS}
        , _comma{COMMA}
    {
        if (useDefaultConfiguration) {
            _operators.Insert(std::make_pair(ShuntingYardParser::DEFAULT_ABS, OperatorDetails{4, false}));
            _operators.Insert(std::make_pair(ShuntingYardParser::DEFAULT_ADDITION, OperatorDetails{1, true}));
            _operators.Insert(std::make_pair(ShuntingYardParser::DEFAULT_AND, OperatorDetails{2, true}));
            _operators.Insert(std::make_pair(ShuntingYardParser::DEFAULT_ASSIGN, OperatorDetails{0, false}));
            _operators.Insert(std::make_pair(ShuntingYardParser::DEFAULT_COSINE, OperatorDetails{4, false}));
            _operators.Insert(std::make_pair(ShuntingYardParser::DEFAULT_DECREMENT, OperatorDetails{6, true}));
            _operators.Insert(std::make_pair(ShuntingYardParser::DEFAULT_DEGTORAD, OperatorDetails{5, true}));
            _operators.Insert(std::make_pair(ShuntingYardParser::DEFAULT_DIVISION, OperatorDetails{2, true}));
            _operators.Insert(std::make_pair(ShuntingYardParser::DEFAULT_EXPONENT, OperatorDetails{3, false}));
            _operators.Insert(std::make_pair(ShuntingYardParser::DEFAULT_INCREMENT, OperatorDetails{6, true}));
            _operators.Insert(std::make_pair(ShuntingYardParser::DEFAULT_IS_CONGRUENT, OperatorDetails{0, false}));
            _operators.Insert(std::make_pair(ShuntingYardParser::DEFAULT_IS_EQUIVALENT, OperatorDetails{0, false}));
            _operators.Insert(std::make_pair(ShuntingYardParser::DEFAULT_IS_GREATER, OperatorDetails{0, false}));
            _operators.Insert(std::make_pair(ShuntingYardParser::DEFAULT_IS_LESS, OperatorDetails{0, false}));
            _operators.Insert(std::make_pair(ShuntingYardParser::DEFAULT_MODULUS, OperatorDetails{2, true}));
            _operators.Insert(std::make_pair(ShuntingYardParser::DEFAULT_MULTIPLICATION, OperatorDetails{2, true}));
            _operators.Insert(std::make_pair(ShuntingYardParser::DEFAULT_NEGATE, OperatorDetails{5, true}));
            _operators.Insert(std::make_pair(ShuntingYardParser::DEFAULT_OR, OperatorDetails{1, true}));
            _operators.Insert(std::make_pair(ShuntingYardParser::DEFAULT_RADTODEG, OperatorDetails{5, true}));
            _operators.Insert(std::make_pair(ShuntingYardParser::DEFAULT_SINE, OperatorDetails{4, false}));
            _operators.Insert(std::make_pair(ShuntingYardParser::DEFAULT_SUBTRACTION, OperatorDetails{1, true}));
            _operators.Insert(std::make_pair(ShuntingYardParser::DEFAULT_TANGENT, OperatorDetails{4, false}));
        }
    }

    std::string ShuntingYardParser::Parse(const std::string& expression) const {
        std::stringstream output{};
        bool isOutputNotEmpty = false;
        std::string input = AddSpacesToSpecialCharacters(expression);
        std::regex extractTokens{"\\s*([^\\s]+)(.*)"};
        std::smatch stringMatches{};
        Stack<std::string> operatorStack{};

        while (std::regex_match(input, stringMatches, extractTokens)) {
            std::string token = stringMatches[1].str();
            input = stringMatches[2].str();

            if (token == _comma) {
                if (*(operatorStack.CTop()) != _leftParenthesis) {
                    throw std::logic_error("Cannot parse \""s + _comma + "\" when not within a \""s + _leftParenthesis + _rightParenthesis + "\" block!"s);
                }
                continue;
            }

            if (token == _leftParenthesis) {
                operatorStack.Push(token);
                continue;
            }

            if (token == _rightParenthesis) {
                while (!(operatorStack.IsEmpty()) && (*(operatorStack.CTop()) != _leftParenthesis)) {
                    output << ' ' << *(operatorStack.CTop());
                    operatorStack.Pop();
                    isOutputNotEmpty = true;
                }
                operatorStack.Pop();
                continue;
            }

            auto found = _operators.CFind(token);
            if (found == _operators.cend()) {
                if (isOutputNotEmpty) {
                    output << ' ';
                }
                output << token;
                isOutputNotEmpty = true;
            } else {
                if (!operatorStack.IsEmpty()) {
                    auto top = operatorStack.CTop();
                    auto topop = _operators.CFind(*top);

                    while (
                        !(operatorStack.IsEmpty())
                        && ((*top) != _leftParenthesis)
                        && (((found->second.Precedence()) < (topop->second.Precedence()))
                            || (found->second.IsLeftAssociative()
                                && (found->second.Precedence() == (topop->second.Precedence()))))
                    ) {
                        output << ' ' << *top;
                        operatorStack.Pop();
                        top = operatorStack.CTop();
                        topop = operatorStack.IsEmpty() ? _operators.cend() : _operators.CFind(*top);
                        isOutputNotEmpty = true;
                    }
                }

                operatorStack.Push(token);
            }
        }

        while (!(operatorStack.IsEmpty())) {
            output << ' ' << *(operatorStack.CTop());
            operatorStack.Pop();
        }

        return output.str();
    }

    std::string ShuntingYardParser::AddSpacesToSpecialCharacters(const std::string& expression) const {
        using str_size_type = std::string::size_type;

        std::string result{expression};

        auto index = str_size_type(0);
        auto leftParenthesisReplace = " "s + _leftParenthesis + " "s;
        auto rightParenthesisReplace = " "s + _rightParenthesis + " "s;
        auto commaReplace = " "s + _comma + " "s;
        auto leftParenthesisSize = _leftParenthesis.size();
        auto rightParenthesisSize = _rightParenthesis.size();
        auto commaSize = _comma.size();
        auto leftParenthesisOffset = leftParenthesisReplace.size();
        auto rightParenthesisOffset = rightParenthesisReplace.size();
        auto commaOffset = commaReplace.size();

        while ((index = result.find_first_of(_leftParenthesis, index)) < result.size()) {
            result.replace(index, leftParenthesisSize, leftParenthesisReplace);
            index += leftParenthesisOffset;
        }

        index = std::string::size_type(0);

        while ((index = result.find_first_of(_rightParenthesis, index)) < result.size()) {
            result.replace(index, rightParenthesisSize, rightParenthesisReplace);
            index += rightParenthesisOffset;
        }

        index = std::string::size_type(0);

        while ((index = result.find_first_of(_comma, index)) < result.size()) {
            result.replace(index, commaSize, commaReplace);
            index += commaOffset;
        }

        return result;
    }
}
