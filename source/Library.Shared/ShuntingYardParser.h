#pragma once
#include "Algorithms.h"
#include "HashMap.h"

namespace FieaGameEngine {
    class ShuntingYardParser final {

    public:
        static const std::string LEFT_PARENTHESIS;
        static const std::string RIGHT_PARENTHESIS;
        static const std::string COMMA;
        static const std::string DEFAULT_ABS;
        static const std::string DEFAULT_ADDITION;
        static const std::string DEFAULT_AND;
        static const std::string DEFAULT_ASSIGN;
        static const std::string DEFAULT_COSINE;
        static const std::string DEFAULT_DECREMENT;
        static const std::string DEFAULT_DEGTORAD;
        static const std::string DEFAULT_DIVISION;
        static const std::string DEFAULT_EXPONENT;
        static const std::string DEFAULT_INCREMENT;
        static const std::string DEFAULT_IS_CONGRUENT;
        static const std::string DEFAULT_IS_EQUIVALENT;
        static const std::string DEFAULT_IS_GREATER;
        static const std::string DEFAULT_IS_LESS;
        static const std::string DEFAULT_MODULUS;
        static const std::string DEFAULT_MULTIPLICATION;
        static const std::string DEFAULT_NEGATE;
        static const std::string DEFAULT_OR;
        static const std::string DEFAULT_RADTODEG;
        static const std::string DEFAULT_SINE;
        static const std::string DEFAULT_SUBTRACTION;
        static const std::string DEFAULT_TANGENT;

        class OperatorDetails final {

        public:
            inline OperatorDetails(){}
            inline OperatorDetails(int precedence, bool isLeftAssociative)
                : _precedence{precedence}
                , _isLeftAssociative{isLeftAssociative} {}

            inline int Precedence() const { return _precedence; }
            inline bool IsLeftAssociative() const { return _isLeftAssociative; }

        private:
            int _precedence{-1};
            bool _isLeftAssociative{true};

        };

        explicit ShuntingYardParser(bool useDefaultOperators = true);

        std::string Parse(const std::string& expression) const;

        void Clear();
        bool IsContainingOperator(const std::string& id) const;
        void AddOperator(const std::string& id, int precedence, bool isLeftAssociative);
        void AddOperator(const std::string& id, OperatorDetails details);
        void RemoveOperator(const std::string& id);

        const std::string& GetLeftParenthesis() const;
        const std::string& GetRightParenthesis() const;
        const std::string& GetComma() const;
        void SetLeftParenthesis(const std::string&);
        void SetRightParenthesis(const std::string&);
        void SetComma(const std::string&);

    private:
        HashMap<std::string, OperatorDetails> _operators{};
        std::string _leftParenthesis{};
        std::string _rightParenthesis{};
        std::string _comma{};

        std::string AddSpacesToSpecialCharacters(const std::string& rawExpression) const;

    };
}

#include "ShuntingYardParser.inl"
