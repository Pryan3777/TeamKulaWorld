#pragma once
#include <functional>
#include "Algorithms.h"
#include "Datum.h"
#include "HashMap.h"
#include "Scope.h"

#define __REVERSEPOLISHEVALUATOR_FOREACHUNARYDATUMELEMENT_DECLARATION(_DatumType)                                                                                                            \
        Datum ForEachUnary ## _DatumType ## DatumElement(Datum& input, std::function<Datum::_DatumType(Datum::size_type index, Datum::_DatumType& element)> function) const;                 \
        Datum ForEachUnaryConst ## _DatumType ## DatumElement(const Datum& input, std::function<Datum::_DatumType(Datum::size_type index, const Datum::_DatumType& element)> function) const

#define __REVERSEPOLISHEVALUATOR_FOREACHUNARYDATUMELEMENT_DEFINITION(_DatumType)                                                                                                                                   \
    Datum ReversePolishEvaluator::ForEachUnary ## _DatumType ## DatumElement(Datum& input, std::function<Datum::_DatumType(Datum::size_type index, Datum::_DatumType& element)> function) const {                  \
        Datum output{};                                                                                                                                                                                            \
        for (auto i = Datum::size_type(0); i < input.Size(); ++i) {                                                                                                                                                \
            output.PushBack(function(i, input.Get ## _DatumType ## Element(i)));                                                                                                                                   \
        }                                                                                                                                                                                                          \
        return output;                                                                                                                                                                                             \
    }                                                                                                                                                                                                              \
    Datum ReversePolishEvaluator::ForEachUnaryConst ## _DatumType ## DatumElement(const Datum& input, std::function<Datum::_DatumType(Datum::size_type index, const Datum::_DatumType& element)> function) const { \
        Datum output{};                                                                                                                                                                                            \
        for (auto i = Datum::size_type(0); i < input.Size(); ++i) {                                                                                                                                                \
            output.PushBack(function(i, input.CGet ## _DatumType ## Element(i)));                                                                                                                                  \
        }                                                                                                                                                                                                          \
        return output;                                                                                                                                                                                             \
    }

//#define __REVERSEPOLISHEVALUATOR_FOREACHBINARYDATUMELEMENT_DECLARATION(_DatumType)

namespace FieaGameEngine {
    class ReversePolishEvaluator final {

    public:
        enum class OperationID : uint8_t {
            ABS = 0U,
            ADDITION,
            AND,
            ASSIGN,
            COSINE,
            DECREMENT,
            DEGTORAD,
            DIVISION,
            EXPONENT,
            INCREMENT,
            IS_CONGRUENT,
            IS_EQUIVALENT,
            IS_GREATER,
            IS_LESS,
            MODULUS,
            MULTIPLICATION,
            NEGATE,
            OR,
            RADTODEG,
            SINE,
            SUBTRACTION,
            TANGENT,
            __OperationID_Count
        };

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

        ReversePolishEvaluator();
        explicit ReversePolishEvaluator(std::initializer_list<std::pair<OperationID, std::string>> operatorOverrides);

        Datum Evaluate(std::string expression) const;
        Datum Evaluate(std::string expression, Scope& scope) const;

    private:
        using DatumType = Datum::DatumType;

        static const std::string TO_STRING_OPERATION_ID[];

        HashMap<std::string, Datum(ReversePolishEvaluator::*)(Datum& input) const> _unaryOperators;
        HashMap<std::string, Datum(ReversePolishEvaluator::*)(Datum& lhs, const Datum& rhs) const> _binaryOperators;

        Datum* TryExtractDatum(std::string path, Scope& scope) const;

        Datum::DatumType ValidateUnaryInputType(OperationID operationID, const Datum& input, std::initializer_list<DatumType> validTypes) const;
        std::pair<bool, bool> ValidateBinaryInputSizes(OperationID operationID, const Datum& lhs, const Datum& rhs) const;
        Datum::DatumType ValidateBinaryInputSameType(OperationID operationID, const Datum& lhs, const Datum& rhs, std::initializer_list<DatumType> validTypes) const;
        Datum::DatumType ValidateBinaryInputSameType(OperationID operationID, const Datum& lhs, const Datum& rhs, bool& isLhsScalar, bool& isRhsScalar, std::initializer_list<DatumType> validTypes) const;
        Datum::DatumType ValidateBinaryInputSameType(OperationID operationID, const Datum& lhs, const Datum& rhs) const;
        Datum::DatumType ValidateBinaryInputSameType(OperationID operationID, const Datum& lhs, const Datum& rhs, bool& isLhsScalar, bool& isRhsScalar) const;
        __REVERSEPOLISHEVALUATOR_FOREACHUNARYDATUMELEMENT_DECLARATION(Integer);
        __REVERSEPOLISHEVALUATOR_FOREACHUNARYDATUMELEMENT_DECLARATION(Float);
        __REVERSEPOLISHEVALUATOR_FOREACHUNARYDATUMELEMENT_DECLARATION(Vector);
        __REVERSEPOLISHEVALUATOR_FOREACHUNARYDATUMELEMENT_DECLARATION(Matrix);
        Datum ForEachUnaryConstDatumPredicate(const Datum& input, std::function<bool(const Datum& datum, Datum::size_type index)> predicate) const;
        Datum ForEachBinaryConstDatumPredicate(const Datum& lhs, const Datum& rhs, std::function<bool(const Datum& d1, Datum::size_type i1, const Datum& d2, Datum::size_type i2)> predicate) const;

        Datum AbsoluteValue(Datum& input) const;
        Datum Addition(Datum& lhs, const Datum& rhs) const;
        Datum And(Datum& lhs, const Datum& rhs) const;
        Datum Assign(Datum& lhs, const Datum& rhs) const;
        Datum Cosine(Datum& input) const;
        Datum Decrement(Datum& input) const;
        Datum DegToRad(Datum& input) const;
        Datum Division(Datum& lhs, const Datum& rhs) const;
        Datum Exponent(Datum& lhs, const Datum& rhs) const;
        Datum Increment(Datum& input) const;
        Datum IsCongruent(Datum& lhs, const Datum& rhs) const;
        Datum IsEquivalent(Datum& lhs, const Datum& rhs) const;
        Datum IsGreater(Datum& lhs, const Datum& rhs) const;
        Datum IsLess(Datum& lhs, const Datum& rhs) const;
        Datum Modulus(Datum& lhs, const Datum& rhs) const;
        Datum Multiplication(Datum& lhs, const Datum& rhs) const;
        Datum Negate(Datum& input) const;
        Datum Or(Datum& lhs, const Datum& rhs) const;
        Datum RadToDeg(Datum& input) const;
        Datum Sine(Datum& input) const;
        Datum Subtraction(Datum& lhs, const Datum& rhs) const;
        Datum Tangent(Datum& input) const;

    };
}

#include "ReversePolishEvaluator.inl"
