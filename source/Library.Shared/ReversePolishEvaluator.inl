#pragma once
#include "ReversePolishEvaluator.h"
#include <cmath>
#include <gsl/gsl>


namespace FieaGameEngine {
    inline ReversePolishEvaluator::ReversePolishEvaluator()
        : ReversePolishEvaluator{std::initializer_list<std::pair<OperationID, std::string>>{}} {}

    inline Datum ReversePolishEvaluator::Evaluate(std::string expression) const { Scope _; return Evaluate(std::move(expression), _); }

    inline typename ReversePolishEvaluator::DatumType ReversePolishEvaluator::ValidateBinaryInputSameType(OperationID operationID, const Datum& lhs, const Datum& rhs) const {
        bool _, __;
        return ValidateBinaryInputSameType(operationID, lhs, rhs, _, __);
    }
    inline typename ReversePolishEvaluator::DatumType ReversePolishEvaluator::ValidateBinaryInputSameType(
        OperationID operationID,
        const Datum& lhs,
        const Datum& rhs,
        std::initializer_list<DatumType> validTypes
    ) const {
        bool _, __;
        return ValidateBinaryInputSameType(operationID, lhs, rhs, _, __, std::move(validTypes));
    }
    inline typename ReversePolishEvaluator::DatumType ReversePolishEvaluator::ValidateBinaryInputSameType(
        OperationID operationID,
        const Datum& lhs,
        const Datum& rhs,
        bool& isLhsScalar,
        bool& isRhsScalar,
        std::initializer_list<DatumType> validTypes
    ) const {
        ValidateBinaryInputSameType(operationID, lhs, rhs, isLhsScalar, isRhsScalar);
        return ValidateUnaryInputType(operationID, lhs, std::move(validTypes));
    }

    inline Datum ReversePolishEvaluator::AbsoluteValue(Datum& input) const {
        return (ValidateUnaryInputType(OperationID::ABS, input, { DatumType::Integer, DatumType::Float }) == DatumType::Integer)
            ? ForEachUnaryConstIntegerDatumElement(input, [](Datum::size_type, const Datum::Integer& element){ return std::abs(element); })
            : ForEachUnaryConstFloatDatumElement(input, [](Datum::size_type, const Datum::Float& element){ return std::abs(element); });
    }
    inline Datum ReversePolishEvaluator::And(Datum& lhs, const Datum& rhs) const {
        return ForEachBinaryConstDatumPredicate(lhs, rhs, [](const Datum& d1, Datum::size_type i1, const Datum& d2, Datum::size_type i2){
            return d1.IsTruthy(i1) && d2.IsTruthy(i2);
        });
    }
    inline Datum ReversePolishEvaluator::Cosine(Datum& input) const {
        ValidateUnaryInputType(OperationID::COSINE, input, { DatumType::Float });
        return ForEachUnaryConstFloatDatumElement(input, [](Datum::size_type, const Datum::Float& element){ return std::cosf(element); });
    }
    inline Datum ReversePolishEvaluator::Decrement(Datum& input) const {
        ValidateUnaryInputType(OperationID::DECREMENT, input, { DatumType::Integer });
        return ForEachUnaryIntegerDatumElement(input, [](Datum::size_type, Datum::Integer& element) { return --element; });
    }
    inline Datum ReversePolishEvaluator::Increment(Datum& input) const {
        ValidateUnaryInputType(OperationID::INCREMENT, input, { DatumType::Integer });
        return ForEachUnaryIntegerDatumElement(input, [](Datum::size_type, Datum::Integer& element) { return ++element; });
    }
    inline Datum ReversePolishEvaluator::IsCongruent(Datum& lhs, const Datum& rhs) const {
        return Datum{(lhs == rhs) ? 1 : 0};
    }
    inline Datum ReversePolishEvaluator::IsGreater(Datum& lhs, const Datum& rhs) const {
        auto type = ValidateBinaryInputSameType(OperationID::IS_GREATER, lhs, rhs, { DatumType::Float, DatumType::Integer, DatumType::String });
        return (type == DatumType::String)
            ? ForEachBinaryConstDatumPredicate(lhs, rhs, [](const Datum& d1, Datum::size_type i1, const Datum& d2, Datum::size_type i2){ return d1.CGetStringElement(i1) > d2.CGetStringElement(i2); })
            : ((type == DatumType::Float)
                ? ForEachBinaryConstDatumPredicate(lhs, rhs, [](const Datum& d1, Datum::size_type i1, const Datum& d2, Datum::size_type i2){ return d1.CGetFloatElement(i1) > d2.CGetFloatElement(i2); })
                : ForEachBinaryConstDatumPredicate(lhs, rhs, [](const Datum& d1, Datum::size_type i1, const Datum& d2, Datum::size_type i2){ return d1.CGetIntegerElement(i1) > d2.CGetIntegerElement(i2); }));
    }
    inline Datum ReversePolishEvaluator::IsLess(Datum& lhs, const Datum& rhs) const {
        auto type = ValidateBinaryInputSameType(OperationID::IS_LESS, lhs, rhs, { DatumType::Float, DatumType::Integer, DatumType::String });
        return (type == DatumType::String)
            ? ForEachBinaryConstDatumPredicate(lhs, rhs, [](const Datum& d1, Datum::size_type i1, const Datum& d2, Datum::size_type i2){ return d1.CGetStringElement(i1) < d2.CGetStringElement(i2); })
            : ((type == DatumType::Float)
                ? ForEachBinaryConstDatumPredicate(lhs, rhs, [](const Datum& d1, Datum::size_type i1, const Datum& d2, Datum::size_type i2) { return d1.CGetFloatElement(i1) < d2.CGetFloatElement(i2); })
                : ForEachBinaryConstDatumPredicate(lhs, rhs, [](const Datum& d1, Datum::size_type i1, const Datum& d2, Datum::size_type i2){ return d1.CGetIntegerElement(i1) < d2.CGetIntegerElement(i2); }));
    }
    inline Datum ReversePolishEvaluator::Or(Datum& lhs, const Datum& rhs) const {
        return ForEachBinaryConstDatumPredicate(lhs, rhs, [](const Datum& d1, Datum::size_type i1, const Datum& d2, Datum::size_type i2){
            return d1.IsTruthy(i1) || d2.IsTruthy(i2);
        });
    }
    inline Datum ReversePolishEvaluator::RadToDeg(Datum& input) const {
        ValidateUnaryInputType(OperationID::RADTODEG, input, { DatumType::Float });
        return ForEachUnaryConstFloatDatumElement(input, [](Datum::size_type, const Datum::Float& element){ return element * 180.f / FLOAT_PI; });
    }
    inline Datum ReversePolishEvaluator::Sine(Datum& input) const {
        ValidateUnaryInputType(OperationID::SINE, input, { DatumType::Float });
        return ForEachUnaryConstFloatDatumElement(input, [](Datum::size_type, const Datum::Float& element){ return std::sinf(element); });
    }
    inline Datum ReversePolishEvaluator::Tangent(Datum& input) const {
        ValidateUnaryInputType(OperationID::TANGENT, input, { DatumType::Float });
        return ForEachUnaryConstFloatDatumElement(input, [](Datum::size_type, const Datum::Float& element){ return std::tanf(element); });
    }
}
