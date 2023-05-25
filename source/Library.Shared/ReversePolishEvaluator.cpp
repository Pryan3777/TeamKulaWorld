#include "pch.h"
#include "ReversePolishEvaluator.h"
#include <functional>
#include <string>
#include "Scope.h"
#include "Stack.h"

using namespace std::literals::string_literals;

namespace FieaGameEngine {
    const std::string ReversePolishEvaluator::DEFAULT_ABS = "abs"s;
    const std::string ReversePolishEvaluator::DEFAULT_ADDITION = "+"s;
    const std::string ReversePolishEvaluator::DEFAULT_AND = "&&"s;
    const std::string ReversePolishEvaluator::DEFAULT_ASSIGN = "="s;
    const std::string ReversePolishEvaluator::DEFAULT_COSINE = "cos"s;
    const std::string ReversePolishEvaluator::DEFAULT_DECREMENT = "--"s;
    const std::string ReversePolishEvaluator::DEFAULT_DEGTORAD = "deg->rad"s;
    const std::string ReversePolishEvaluator::DEFAULT_DIVISION = "/"s;
    const std::string ReversePolishEvaluator::DEFAULT_EXPONENT = "^"s;
    const std::string ReversePolishEvaluator::DEFAULT_INCREMENT = "++"s;
    const std::string ReversePolishEvaluator::DEFAULT_IS_CONGRUENT = "==="s;
    const std::string ReversePolishEvaluator::DEFAULT_IS_EQUIVALENT = "=="s;
    const std::string ReversePolishEvaluator::DEFAULT_IS_GREATER = ">"s;
    const std::string ReversePolishEvaluator::DEFAULT_IS_LESS = "<"s;
    const std::string ReversePolishEvaluator::DEFAULT_MODULUS = "%"s;
    const std::string ReversePolishEvaluator::DEFAULT_MULTIPLICATION = "*"s;
    const std::string ReversePolishEvaluator::DEFAULT_NEGATE = "~"s;
    const std::string ReversePolishEvaluator::DEFAULT_OR = "||"s;
    const std::string ReversePolishEvaluator::DEFAULT_RADTODEG = "rad->deg"s;
    const std::string ReversePolishEvaluator::DEFAULT_SINE = "sin"s;
    const std::string ReversePolishEvaluator::DEFAULT_SUBTRACTION = "-"s;
    const std::string ReversePolishEvaluator::DEFAULT_TANGENT = "tan"s;

    const std::string ReversePolishEvaluator::TO_STRING_OPERATION_ID[] = { // Must match the definition of OperationID, defined in ReversePolishEvaluator.h
        "absolute value"s,
        "addition"s,
        "and"s,
        "assignment"s,
        "cosine"s,
        "decrement"s,
        "degree to radian"s,
        "division"s,
        "exponent"s,
        "increment"s,
        "is congruent"s,
        "is equivalent"s,
        "is greater"s,
        "is less"s,
        "modulus"s,
        "multiplication"s,
        "negate"s,
        "or"s,
        "radian to degree"s,
        "sine"s,
        "subtraction"s,
        "tangent"s
    };

    ReversePolishEvaluator::ReversePolishEvaluator(std::initializer_list<std::pair<OperationID, std::string>> operatorOverrides)
        : _unaryOperators{}
        , _binaryOperators{}
    {
        std::string operators[static_cast<uint8_t>(OperationID::__OperationID_Count)];
        operators[static_cast<uint8_t>(OperationID::ABS)] = DEFAULT_ABS;
        operators[static_cast<uint8_t>(OperationID::ADDITION)] = DEFAULT_ADDITION;
        operators[static_cast<uint8_t>(OperationID::AND)] = DEFAULT_AND;
        operators[static_cast<uint8_t>(OperationID::ASSIGN)] = DEFAULT_ASSIGN;
        operators[static_cast<uint8_t>(OperationID::COSINE)] = DEFAULT_COSINE;
        operators[static_cast<uint8_t>(OperationID::DECREMENT)] = DEFAULT_DECREMENT;
        operators[static_cast<uint8_t>(OperationID::DEGTORAD)] = DEFAULT_DEGTORAD;
        operators[static_cast<uint8_t>(OperationID::DIVISION)] = DEFAULT_DIVISION;
        operators[static_cast<uint8_t>(OperationID::EXPONENT)] = DEFAULT_EXPONENT;
        operators[static_cast<uint8_t>(OperationID::INCREMENT)] = DEFAULT_INCREMENT;
        operators[static_cast<uint8_t>(OperationID::IS_CONGRUENT)] = DEFAULT_IS_CONGRUENT;
        operators[static_cast<uint8_t>(OperationID::IS_EQUIVALENT)] = DEFAULT_IS_EQUIVALENT;
        operators[static_cast<uint8_t>(OperationID::IS_GREATER)] = DEFAULT_IS_GREATER;
        operators[static_cast<uint8_t>(OperationID::IS_LESS)] = DEFAULT_IS_LESS;
        operators[static_cast<uint8_t>(OperationID::MODULUS)] = DEFAULT_MODULUS;
        operators[static_cast<uint8_t>(OperationID::MULTIPLICATION)] = DEFAULT_MULTIPLICATION;
        operators[static_cast<uint8_t>(OperationID::NEGATE)] = DEFAULT_NEGATE;
        operators[static_cast<uint8_t>(OperationID::OR)] = DEFAULT_OR;
        operators[static_cast<uint8_t>(OperationID::RADTODEG)] = DEFAULT_RADTODEG;
        operators[static_cast<uint8_t>(OperationID::SINE)] = DEFAULT_SINE;
        operators[static_cast<uint8_t>(OperationID::SUBTRACTION)] = DEFAULT_SUBTRACTION;
        operators[static_cast<uint8_t>(OperationID::TANGENT)] = DEFAULT_TANGENT;

        for (const auto& pair : operatorOverrides) {
            operators[static_cast<uint8_t>(pair.first)] = pair.second;
        }

        _unaryOperators.Insert(std::make_pair(operators[static_cast<uint8_t>(OperationID::ABS)], &ReversePolishEvaluator::AbsoluteValue));
        _unaryOperators.Insert(std::make_pair(operators[static_cast<uint8_t>(OperationID::COSINE)], &ReversePolishEvaluator::Cosine));
        _unaryOperators.Insert(std::make_pair(operators[static_cast<uint8_t>(OperationID::DECREMENT)], &ReversePolishEvaluator::Decrement));
        _unaryOperators.Insert(std::make_pair(operators[static_cast<uint8_t>(OperationID::DEGTORAD)], &ReversePolishEvaluator::DegToRad));
        _unaryOperators.Insert(std::make_pair(operators[static_cast<uint8_t>(OperationID::INCREMENT)], &ReversePolishEvaluator::Increment));
        _unaryOperators.Insert(std::make_pair(operators[static_cast<uint8_t>(OperationID::NEGATE)], &ReversePolishEvaluator::Negate));
        _unaryOperators.Insert(std::make_pair(operators[static_cast<uint8_t>(OperationID::RADTODEG)], &ReversePolishEvaluator::RadToDeg));
        _unaryOperators.Insert(std::make_pair(operators[static_cast<uint8_t>(OperationID::SINE)], &ReversePolishEvaluator::Sine));
        _unaryOperators.Insert(std::make_pair(operators[static_cast<uint8_t>(OperationID::TANGENT)], &ReversePolishEvaluator::Tangent));

        _binaryOperators.Insert(std::make_pair(operators[static_cast<uint8_t>(OperationID::ADDITION)], &ReversePolishEvaluator::Addition));
        _binaryOperators.Insert(std::make_pair(operators[static_cast<uint8_t>(OperationID::AND)], &ReversePolishEvaluator::And));
        _binaryOperators.Insert(std::make_pair(operators[static_cast<uint8_t>(OperationID::ASSIGN)], &ReversePolishEvaluator::Assign));
        _binaryOperators.Insert(std::make_pair(operators[static_cast<uint8_t>(OperationID::DIVISION)], &ReversePolishEvaluator::Division));
        _binaryOperators.Insert(std::make_pair(operators[static_cast<uint8_t>(OperationID::EXPONENT)], &ReversePolishEvaluator::Exponent));
        _binaryOperators.Insert(std::make_pair(operators[static_cast<uint8_t>(OperationID::IS_CONGRUENT)], &ReversePolishEvaluator::IsCongruent));
        _binaryOperators.Insert(std::make_pair(operators[static_cast<uint8_t>(OperationID::IS_EQUIVALENT)], &ReversePolishEvaluator::IsEquivalent));
        _binaryOperators.Insert(std::make_pair(operators[static_cast<uint8_t>(OperationID::IS_GREATER)], &ReversePolishEvaluator::IsGreater));
        _binaryOperators.Insert(std::make_pair(operators[static_cast<uint8_t>(OperationID::IS_LESS)], &ReversePolishEvaluator::IsLess));
        _binaryOperators.Insert(std::make_pair(operators[static_cast<uint8_t>(OperationID::MULTIPLICATION)], &ReversePolishEvaluator::Multiplication));
        _binaryOperators.Insert(std::make_pair(operators[static_cast<uint8_t>(OperationID::OR)], &ReversePolishEvaluator::Or));
        _binaryOperators.Insert(std::make_pair(operators[static_cast<uint8_t>(OperationID::SUBTRACTION)], &ReversePolishEvaluator::Subtraction));
    }

    Datum ReversePolishEvaluator::Evaluate(std::string expression, Scope& scope) const {
        auto anons = Vector<std::unique_ptr<Datum>>{expression.size() / std::size_t(2)};
        auto stack = Stack<std::reference_wrapper<Datum>>{};

        std::regex extractTokens{"\\s*([^\\s]+)(.*)"};
        std::regex extractInteger{"^\\d+$"};
        std::regex extractFloat{"^\\d+\\.\\d+$"};

        std::smatch stringMatches{};

        while (std::regex_match(expression, stringMatches, extractTokens)) {
            std::string token = stringMatches[1].str();
            expression = stringMatches[2].str();

            auto binaryOperator = _binaryOperators.Find(token);

            if (binaryOperator != _binaryOperators.end()) {
                const Datum& rhs = *(stack.Top());
                stack.Pop();
                Datum& lhs = *(stack.Top());
                stack.Pop();
                anons.EmplaceBack(std::make_unique<Datum>((this->*(binaryOperator->second))(lhs, rhs)));
                stack.Push(std::ref(*(anons.Back())));
                continue;
            }

            auto unaryOperator = _unaryOperators.Find(token);

            if (unaryOperator != _unaryOperators.end()) {
                Datum& input = *(stack.Top());
                stack.Pop();
                anons.EmplaceBack(std::make_unique<Datum>((this->*(unaryOperator->second))(input)));
                stack.Push(std::ref(*(anons.Back())));
                continue;
            }

            if (std::regex_match(token, extractInteger)) {
                anons.EmplaceBack(std::make_unique<Datum>(std::stoi(token)));
                stack.Push(std::ref(*(anons.Back())));
                continue;
            }

            if (std::regex_match(token, extractFloat)) {
                anons.EmplaceBack(std::make_unique<Datum>(std::stof(token)));
                stack.Push(std::ref(*(anons.Back())));
                continue;
            }

            if (token.substr(0, 1 + Datum::VECTOR_STRING_FORMAT_PREFIX.size()) == Datum::VECTOR_STRING_FORMAT.substr(0, 1 + Datum::VECTOR_STRING_FORMAT_PREFIX.size())) {
                anons.EmplaceBack(std::make_unique<Datum>());
                auto* anon = anons.Back().get();
                anon->SetType(DatumType::Vector);
                anon->PushBackFromString(token);
                stack.Push(std::ref(*anon));
                continue;
            }

            if (token.substr(0, 1 + Datum::MATRIX_STRING_FORMAT_PREFIX.size()) == Datum::MATRIX_STRING_FORMAT.substr(0, 1 + Datum::MATRIX_STRING_FORMAT_PREFIX.size())) {
                anons.EmplaceBack(std::make_unique<Datum>());
                auto* anon = anons.Back().get();
                anon->SetType(DatumType::Matrix);
                anon->PushBackFromString(token);
                stack.Push(std::ref(*anon));
                continue;
            }

            Datum* extracted = TryExtractDatum(token, scope);

            if (extracted != nullptr) {
                stack.Push(std::ref(*extracted));
            } else {
                anons.EmplaceBack(std::make_unique<Datum>(token));
                stack.Push(std::ref(*(anons.Back())));
            }
        }

        if (stack.Size() != decltype(stack)::size_type(1)) {
            throw std::invalid_argument("Bad expression, ambiguous result!"s);
        }

        return *(stack.Top());
    }

    Datum* ReversePolishEvaluator::TryExtractDatum(std::string path, Scope& scope) const {
        auto thisKeyIndexEnd = 1 + Scope::THIS_KEY.size();
        if (path.substr(0, thisKeyIndexEnd) == (Scope::THIS_KEY + "."s)) {
            return scope.Search(path.substr(thisKeyIndexEnd));
        }

        Scope* context = &scope;
        std::regex extractScopes{"([^\\.]+)\\.(.+)"};
        std::regex extractIndex{"([^\\.]+)\\[(\\d+)\\]\\.(.+)"};
        std::smatch stringMatches{};

        while (std::regex_match(path, stringMatches, extractScopes)) {
            std::string name{};
            int index = 0;

            std::smatch stringIndexMatches{};

            if (std::regex_match(path, stringIndexMatches, extractIndex)) {
                name = stringIndexMatches[1].str();
                index = std::stoi(stringIndexMatches[2].str());
                path = stringIndexMatches[3].str();
            } else {
                name = stringMatches[1].str();
                path = stringMatches[2].str();
            }

            auto found = context->Find(name);

            if (found == context->end()) {
                return nullptr;
            }

            context = &(found->second.GetTableElement(index));
        }

        return context->Search(path);
    }

    typename ReversePolishEvaluator::DatumType ReversePolishEvaluator::ValidateUnaryInputType(
        OperationID operationID,
        const Datum& input,
        std::initializer_list<DatumType> validTypes
    ) const {
        auto type = input.ActualType();
        bool isInvalid = true;

        for (const auto& valid : validTypes) {
            if (type == valid) {
                isInvalid = false;
                break;
            }
        }

        if (isInvalid) {
            throw std::invalid_argument("Cannot execute operation \""s + TO_STRING_OPERATION_ID[static_cast<uint8_t>(operationID)] + "\" on data of type "s + ToStringDatumType(type) + "!"s);
        }

        return type;
    }

    std::pair<bool, bool> ReversePolishEvaluator::ValidateBinaryInputSizes(OperationID operationID, const Datum& lhs, const Datum& rhs) const {
        if ((lhs.Size() != rhs.Size()) && (rhs.Size() != Datum::size_type(1)) && (lhs.Size() != Datum::size_type(1))) {
            throw std::invalid_argument("Operation \""s + TO_STRING_OPERATION_ID[static_cast<uint8_t>(operationID)] + "\" needs either same-size datums or for at least one of the datums to be of size 1."s);
        }

        return std::make_pair(lhs.Size() == Datum::size_type(1), rhs.Size() == Datum::size_type(1));
    }

    typename ReversePolishEvaluator::DatumType ReversePolishEvaluator::ValidateBinaryInputSameType(OperationID operationID, const Datum& lhs, const Datum& rhs, bool& isLhsScalar, bool& isRhsScalar) const {
        auto areInputsScalar = ValidateBinaryInputSizes(operationID, lhs, rhs);

        auto ltype = lhs.ActualType();
        if (ltype != rhs.ActualType()) {
            throw std::invalid_argument("Cannot execute operation \""s + TO_STRING_OPERATION_ID[static_cast<uint8_t>(operationID)] + "\" on data of different types!"s);
        }

        isLhsScalar = areInputsScalar.first;
        isRhsScalar = areInputsScalar.second;

        return ltype;
    }

    __REVERSEPOLISHEVALUATOR_FOREACHUNARYDATUMELEMENT_DEFINITION(Integer)
    __REVERSEPOLISHEVALUATOR_FOREACHUNARYDATUMELEMENT_DEFINITION(Float)
    __REVERSEPOLISHEVALUATOR_FOREACHUNARYDATUMELEMENT_DEFINITION(Vector)
    __REVERSEPOLISHEVALUATOR_FOREACHUNARYDATUMELEMENT_DEFINITION(Matrix)

    Datum ReversePolishEvaluator::ForEachUnaryConstDatumPredicate(const Datum& input, std::function<bool(const Datum& datum, Datum::size_type index)> predicate) const {
        Datum output{};

        for (auto i = Datum::size_type(0); i < input.Size(); ++i) {
            output.PushBack(predicate(input, i) ? 1 : 0);
        }

        return output;
    }

    Datum ReversePolishEvaluator::ForEachBinaryConstDatumPredicate(
        const Datum& lhs,
        const Datum& rhs,
        std::function<bool(const Datum& d1, Datum::size_type i1, const Datum& d2, Datum::size_type i2)> predicate
    ) const {
        auto size = std::max(lhs.Size(), rhs.Size());
        using dst = decltype(size);

        bool isLhsScalar = lhs.Size() == dst(1);
        bool isRhsScalar = rhs.Size() == dst(1);

        Datum output{};

        for (auto i = dst(0); i < size; ++i) {
            output.PushBack(predicate(lhs, isLhsScalar ? 0 : i, rhs, isRhsScalar ? 0 : i) ? 1 : 0);
        }

        return output;
    }

    Datum ReversePolishEvaluator::Addition(Datum& lhs, const Datum& rhs) const {
        const auto& operationName = TO_STRING_OPERATION_ID[static_cast<uint8_t>(OperationID::ADDITION)];

        auto lhsSize = lhs.Size();
        auto rhsSize = rhs.Size();

        bool isLhsScalar = lhsSize == decltype(lhsSize)(1);
        bool isRhsScalar = rhsSize == decltype(rhsSize)(1);

        if (!(isLhsScalar || isRhsScalar) && (lhsSize != rhsSize)) {
            throw std::invalid_argument("Operation \""s + operationName + "\" needs either datums of the same size or for one or both datums to be of size 1!"s);
        }

        auto lhsType = lhs.ActualType();

        if ((lhsType != DatumType::Float) && (lhsType != DatumType::Integer) && (lhsType != DatumType::Vector) && (lhsType != DatumType::Matrix) && (lhsType != DatumType::String)) {
            throw std::invalid_argument("Operation \""s + operationName + "\" cannot be done with a LHS of type "s + ToStringDatumType(lhsType) + "!"s);
        }

        auto rhsType = rhs.ActualType();

        if ((rhsType != DatumType::Float) && (rhsType != DatumType::Integer) && (rhsType != DatumType::Vector) && (rhsType != DatumType::Matrix) && (rhsType != DatumType::String)) {
            throw std::invalid_argument("Operation \""s + operationName + "\" cannot be done with a RHS of type "s + ToStringDatumType(rhsType) + "!"s);
        }

        if ((lhsType != rhsType) && !(((lhsType == DatumType::Float) && (rhsType == DatumType::Integer)) || ((lhsType == DatumType::Integer) && (rhsType == DatumType::Float)))) {
            throw std::invalid_argument("Operation \""s + operationName + "\" is invalid with the LHS-RHS type combination of "s + ToStringDatumType(lhsType) + "-"s + ToStringDatumType(rhsType) + "!"s);
        }

        auto size = isLhsScalar ? rhsSize : lhsSize;
        Datum result{};

        switch (lhsType) {

        case DatumType::Float:
            for (auto i = decltype(size)(0); i < size; ++i) {
                result.PushBack(
                    lhs.CGetFloatElement(isLhsScalar ? decltype(i)(0) : i)
                    + ((rhsType == DatumType::Float)
                        ? rhs.CGetFloatElement(isRhsScalar ? decltype(i)(0) : i)
                        : static_cast<Datum::Float>(rhs.CGetIntegerElement(isRhsScalar ? decltype(i)(0) : i))
                        )
                );
            }
            break;

        case DatumType::Integer:
            if (rhsType == DatumType::Integer) {
                for (auto i = decltype(size)(0); i < size; ++i) {
                    result.PushBack(lhs.CGetIntegerElement(isLhsScalar ? decltype(i)(0) : i) + rhs.CGetIntegerElement(isRhsScalar ? decltype(i)(0) : i));
                }
            } else {
                for (auto i = decltype(size)(0); i < size; ++i) {
                    result.PushBack(lhs.CGetIntegerElement(isLhsScalar ? decltype(i)(0) : i) + rhs.CGetFloatElement(isRhsScalar ? decltype(i)(0) : i));
                }
            }
            break;

        case DatumType::Vector:
            for (auto i = decltype(size)(0); i < size; ++i) {
                result.PushBack(lhs.CGetVectorElement(isLhsScalar ? decltype(i)(0) : i) + rhs.CGetVectorElement(isRhsScalar ? decltype(i)(0) : i));
            }
            break;

        case DatumType::Matrix:
            for (auto i = decltype(size)(0); i < size; ++i) {
                result.PushBack(lhs.CGetMatrixElement(isLhsScalar ? decltype(i)(0) : i) + rhs.CGetMatrixElement(isRhsScalar ? decltype(i)(0) : i));
            }
            break;

        case DatumType::String:
            for (auto i = decltype(size)(0); i < size; ++i) {
                result.PushBack(lhs.CGetStringElement(isLhsScalar ? decltype(i)(0) : i) + rhs.CGetStringElement(isRhsScalar ? decltype(i)(0) : i));
            }
            break;

        }

        return result;
    }

    Datum ReversePolishEvaluator::Assign(Datum& lhs, const Datum& rhs) const {
        if (lhs.Parent() == nullptr) {
            throw std::invalid_argument("Operation \""s + TO_STRING_OPERATION_ID[static_cast<uint8_t>(OperationID::ASSIGN)] + "\" cannot be done if the LHS has no parent!"s);
        }

        lhs = rhs;
        return Datum{rhs};
    }

    Datum ReversePolishEvaluator::DegToRad(Datum& input) const {
        auto type = ValidateUnaryInputType(OperationID::DEGTORAD, input, { DatumType::Integer, DatumType::Float });

        if (type == DatumType::Float) {
            return ForEachUnaryConstFloatDatumElement(input, [](Datum::size_type, const Datum::Float& element){ return element * FLOAT_PI / 180.f; });
        }

        Datum result;

        for (auto i = Datum::size_type(0); i < input.Size(); ++i) {
            result.PushBack(input.CGetIntegerElement(i) * FLOAT_PI / 180.f);
        }

        return result;
    }

    Datum ReversePolishEvaluator::Division(Datum& lhs, const Datum& rhs) const {
        const auto& operationName = TO_STRING_OPERATION_ID[static_cast<uint8_t>(OperationID::DIVISION)];

        auto lhsSize = lhs.Size();
        auto rhsSize = rhs.Size();

        bool isLhsScalar = lhsSize == decltype(lhsSize)(1);
        bool isRhsScalar = rhsSize == decltype(rhsSize)(1);

        if (!(isLhsScalar || isRhsScalar) && (lhsSize != rhsSize)) {
            throw std::invalid_argument("Operation \""s + operationName + "\" needs either datums of the same size or for one or both datums to be of size 1!"s);
        }

        auto lhsType = lhs.ActualType();

        if ((lhsType != DatumType::Float) && (lhsType != DatumType::Integer) && (lhsType != DatumType::Vector) && (lhsType != DatumType::Matrix)) {
            throw std::invalid_argument("Operation \""s + operationName + "\" cannot be done with a LHS of type "s + ToStringDatumType(lhsType) + "!"s);
        }

        auto rhsType = rhs.ActualType();

        if ((rhsType != DatumType::Float) && (rhsType != DatumType::Integer) && (rhsType != DatumType::Matrix)) {
            throw std::invalid_argument("Operation \""s + operationName + "\" cannot be done with a RHS of type "s + ToStringDatumType(rhsType) + "!"s);
        }

        if ((rhsType == DatumType::Matrix) && (rhsType != lhsType)) {
            throw std::invalid_argument("Operation \""s + operationName + "\" can only use a RHS of type "s + ToStringDatumType(rhsType) + " if the LHS is of the same type!"s);
        }

        auto size = isLhsScalar ? rhsSize : lhsSize;
        Datum result{};

        switch (lhsType) {

        case DatumType::Float:
            switch (rhsType) {

            case DatumType::Float:
                for (auto i = decltype(size)(0); i < size; ++i) {
                    result.PushBack(lhs.CGetFloatElement(isLhsScalar ? decltype(i)(0) : i) / rhs.CGetFloatElement(isRhsScalar ? decltype(i)(0) : i));
                }
                break;

            case DatumType::Integer:
                for (auto i = decltype(size)(0); i < size; ++i) {
                    result.PushBack(lhs.CGetFloatElement(isLhsScalar ? decltype(i)(0) : i) / rhs.CGetIntegerElement(isRhsScalar ? decltype(i)(0) : i));
                }
                break;

            }
            break;

        case DatumType::Integer:
            switch (rhsType) {

            case DatumType::Float:
                for (auto i = decltype(size)(0); i < size; ++i) {
                    result.PushBack(lhs.CGetIntegerElement(isLhsScalar ? decltype(i)(0) : i) / rhs.CGetFloatElement(isRhsScalar ? decltype(i)(0) : i));
                }
                break;

            case DatumType::Integer:
                for (auto i = decltype(size)(0); i < size; ++i) {
                    result.PushBack(lhs.CGetIntegerElement(isLhsScalar ? decltype(i)(0) : i) / rhs.CGetIntegerElement(isRhsScalar ? decltype(i)(0) : i));
                }
                break;

            }
            break;

        case DatumType::Vector:
            switch (rhsType) {

            case DatumType::Float:
                for (auto i = decltype(size)(0); i < size; ++i) {
                    result.PushBack(lhs.CGetVectorElement(isLhsScalar ? decltype(i)(0) : i) / rhs.CGetFloatElement(isRhsScalar ? decltype(i)(0) : i));
                }
                break;

            case DatumType::Integer:
                for (auto i = decltype(size)(0); i < size; ++i) {
                    result.PushBack(lhs.CGetVectorElement(isLhsScalar ? decltype(i)(0) : i) / static_cast<Datum::Float>(rhs.CGetIntegerElement(isRhsScalar ? decltype(i)(0) : i)));
                }
                break;

            }
            break;

        case DatumType::Matrix:
            switch (rhsType) {

            case DatumType::Float:
                for (auto i = decltype(size)(0); i < size; ++i) {
                    result.PushBack(lhs.CGetMatrixElement(isLhsScalar ? decltype(i)(0) : i) / rhs.CGetFloatElement(isRhsScalar ? decltype(i)(0) : i));
                }
                break;

            case DatumType::Integer:
                for (auto i = decltype(size)(0); i < size; ++i) {
                    result.PushBack(lhs.CGetMatrixElement(isLhsScalar ? decltype(i)(0) : i) / static_cast<Datum::Float>(rhs.CGetIntegerElement(isRhsScalar ? decltype(i)(0) : i)));
                }
                break;

            case DatumType::Matrix:
                for (auto i = decltype(size)(0); i < size; ++i) {
                    result.PushBack(lhs.CGetMatrixElement(isLhsScalar ? decltype(i)(0) : i) / rhs.CGetMatrixElement(isRhsScalar ? decltype(i)(0) : i));
                }
                break;

            }
            break;

        }

        return result;
    }

    Datum ReversePolishEvaluator::Exponent(Datum& lhs, const Datum& rhs) const {
        bool isLhsScalar, isRhsScalar;
        ValidateBinaryInputSameType(OperationID::EXPONENT, lhs, rhs, isLhsScalar, isRhsScalar, { DatumType::Float });
        auto size = isLhsScalar ? rhs.Size() : lhs.Size();

        Datum result{};
        for (auto i = decltype(size)(0); i < size; ++i) {
            result.PushBack(std::powf(lhs.CGetFloatElement(isLhsScalar ? decltype(i)(0) : i), rhs.CGetFloatElement(isRhsScalar ? decltype(i)(0) : i)));
        }
        return result;
    }

    Datum ReversePolishEvaluator::IsEquivalent(Datum& lhs, const Datum& rhs) const {
        auto type = ValidateBinaryInputSameType(OperationID::IS_EQUIVALENT, lhs, rhs);
        std::function<bool(const Datum&, Datum::size_type, const Datum&, Datum::size_type)> predicate = [this](const Datum&, Datum::size_type, const Datum&, Datum::size_type){
            return false;
        };

        switch (type) {

        case DatumType::Integer:
            predicate = [](const Datum& d1, Datum::size_type i1, const Datum& d2, Datum::size_type i2){ return d1.CGetIntegerElement(i1) == d2.CGetIntegerElement(i2); };
            break;

        case DatumType::Float:
            predicate = [](const Datum& d1, Datum::size_type i1, const Datum& d2, Datum::size_type i2){ return d1.CGetFloatElement(i1) == d2.CGetFloatElement(i2); };
            break;

        case DatumType::String:
            predicate = [](const Datum& d1, Datum::size_type i1, const Datum& d2, Datum::size_type i2){ return d1.CGetStringElement(i1) == d2.CGetStringElement(i2); };
            break;

        case DatumType::Vector:
            predicate = [](const Datum& d1, Datum::size_type i1, const Datum& d2, Datum::size_type i2){ return d1.CGetVectorElement(i1) == d2.CGetVectorElement(i2); };
            break;

        case DatumType::Matrix:
            predicate = [](const Datum& d1, Datum::size_type i1, const Datum& d2, Datum::size_type i2){ return d1.CGetMatrixElement(i1) == d2.CGetMatrixElement(i2); };
            break;

        case DatumType::Pointer:
            predicate = [](const Datum& d1, Datum::size_type i1, const Datum& d2, Datum::size_type i2){ return d1.CGetPointerElement(i1)->Equals(d2.CGetPointerElement(i2)); };
            break;

        case DatumType::Table:
        case DatumType::InternalTable:
        case DatumType::ExternalTable:
            predicate = [](const Datum& d1, Datum::size_type i1, const Datum& d2, Datum::size_type i2){ return d1.CGetTableElement(i1) == d2.CGetTableElement(i2); };
            break;

        case DatumType::Unknown:
            return Datum{};

        }

        return ForEachBinaryConstDatumPredicate(lhs, rhs, predicate);
    }

    Datum ReversePolishEvaluator::Modulus(Datum& lhs, const Datum& rhs) const {
        bool isLhsScalar, isRhsScalar;
        ValidateBinaryInputSameType(OperationID::MODULUS, lhs, rhs, isLhsScalar, isRhsScalar, { DatumType::Integer });
        auto size = isLhsScalar ? rhs.Size() : lhs.Size();

        Datum result{};
        for (auto i = decltype(size)(0); i < size; ++i) {
            result.PushBack(lhs.CGetIntegerElement(isLhsScalar ? decltype(i)(0) : i) % rhs.CGetIntegerElement(isRhsScalar ? decltype(i)(0) : i));
        }
        return result;
    }

    Datum ReversePolishEvaluator::Multiplication(Datum& lhs, const Datum& rhs) const {
        const auto& operationName = TO_STRING_OPERATION_ID[static_cast<uint8_t>(OperationID::MULTIPLICATION)];

        auto lhsSize = lhs.Size();
        auto rhsSize = rhs.Size();

        bool isLhsScalar = lhsSize == decltype(lhsSize)(1);
        bool isRhsScalar = rhsSize == decltype(rhsSize)(1);

        if (!(isLhsScalar || isRhsScalar) && (lhsSize != rhsSize)) {
            throw std::invalid_argument("Operation \""s + operationName + "\" needs either datums of the same size or for one or both datums to be of size 1!"s);
        }

        auto lhsType = lhs.ActualType();

        if ((lhsType != DatumType::Float) && (lhsType != DatumType::Integer) && (lhsType != DatumType::Vector) && (lhsType != DatumType::Matrix)) {
            throw std::invalid_argument("Operation \""s + operationName + "\" cannot be done with a LHS of type "s + ToStringDatumType(lhsType) + "!"s);
        }

        auto rhsType = rhs.ActualType();

        if ((rhsType != DatumType::Float) && (rhsType != DatumType::Integer) && (rhsType != DatumType::Vector) && (rhsType != DatumType::Matrix)) {
            throw std::invalid_argument("Operation \""s + operationName + "\" cannot be done with a RHS of type "s + ToStringDatumType(rhsType) + "!"s);
        }

        if ((lhsType == DatumType::Matrix) && (rhsType == DatumType::Vector)) {
            throw std::invalid_argument("Operation \""s + operationName + "\" is invalid with the LHS-RHS type combination of "s + ToStringDatumType(lhsType) + "-"s + ToStringDatumType(rhsType) + "!"s);
        }

        auto size = isLhsScalar ? rhsSize : lhsSize;
        Datum result{};

        switch (lhsType) {

        case DatumType::Float:
            switch (rhsType) {

            case DatumType::Float:
                for (auto i = decltype(size)(0); i < size; ++i) {
                    result.PushBack(lhs.CGetFloatElement(isLhsScalar ? decltype(i)(0) : i) * rhs.CGetFloatElement(isRhsScalar ? decltype(i)(0) : i));
                }
                break;

            case DatumType::Integer:
                for (auto i = decltype(size)(0); i < size; ++i) {
                    result.PushBack(lhs.CGetFloatElement(isLhsScalar ? decltype(i)(0) : i) * rhs.CGetIntegerElement(isRhsScalar ? decltype(i)(0) : i));
                }
                break;

            case DatumType::Vector:
                for (auto i = decltype(size)(0); i < size; ++i) {
                    result.PushBack(lhs.CGetFloatElement(isLhsScalar ? decltype(i)(0) : i) * rhs.CGetVectorElement(isRhsScalar ? decltype(i)(0) : i));
                }
                break;

            case DatumType::Matrix:
                for (auto i = decltype(size)(0); i < size; ++i) {
                    result.PushBack(lhs.CGetFloatElement(isLhsScalar ? decltype(i)(0) : i) * rhs.CGetMatrixElement(isRhsScalar ? decltype(i)(0) : i));
                }
                break;

            }
            break;

        case DatumType::Integer:
            switch (rhsType) {

            case DatumType::Float:
                for (auto i = decltype(size)(0); i < size; ++i) {
                    result.PushBack(lhs.CGetIntegerElement(isLhsScalar ? decltype(i)(0) : i) * rhs.CGetFloatElement(isRhsScalar ? decltype(i)(0) : i));
                }
                break;

            case DatumType::Integer:
                for (auto i = decltype(size)(0); i < size; ++i) {
                    result.PushBack(lhs.CGetIntegerElement(isLhsScalar ? decltype(i)(0) : i) * rhs.CGetIntegerElement(isRhsScalar ? decltype(i)(0) : i));
                }
                break;

            case DatumType::Vector:
                for (auto i = decltype(size)(0); i < size; ++i) {
                    result.PushBack(static_cast<Datum::Float>(lhs.CGetIntegerElement(isLhsScalar ? decltype(i)(0) : i)) * rhs.CGetVectorElement(isRhsScalar ? decltype(i)(0) : i));
                }
                break;

            case DatumType::Matrix:
                for (auto i = decltype(size)(0); i < size; ++i) {
                    result.PushBack(static_cast<Datum::Float>(lhs.CGetIntegerElement(isLhsScalar ? decltype(i)(0) : i)) * rhs.CGetMatrixElement(isRhsScalar ? decltype(i)(0) : i));
                }
                break;

            }
            break;

        case DatumType::Vector:
            switch (rhsType) {

            case DatumType::Float:
                for (auto i = decltype(size)(0); i < size; ++i) {
                    result.PushBack(lhs.CGetVectorElement(isLhsScalar ? decltype(i)(0) : i) * rhs.CGetFloatElement(isRhsScalar ? decltype(i)(0) : i));
                }
                break;

            case DatumType::Integer:
                for (auto i = decltype(size)(0); i < size; ++i) {
                    result.PushBack(lhs.CGetVectorElement(isLhsScalar ? decltype(i)(0) : i) * static_cast<Datum::Float>(rhs.CGetIntegerElement(isRhsScalar ? decltype(i)(0) : i)));
                }
                break;

            case DatumType::Vector:
                for (auto i = decltype(size)(0); i < size; ++i) {
                    result.PushBack(lhs.CGetVectorElement(isLhsScalar ? decltype(i)(0) : i) * rhs.CGetVectorElement(isRhsScalar ? decltype(i)(0) : i));
                }
                break;

            case DatumType::Matrix:
                for (auto i = decltype(size)(0); i < size; ++i) {
                    result.PushBack(lhs.CGetVectorElement(isLhsScalar ? decltype(i)(0) : i) * rhs.CGetMatrixElement(isRhsScalar ? decltype(i)(0) : i));
                }
                break;

            }
            break;

        case DatumType::Matrix:
            switch (rhsType) {

            case DatumType::Float:
                for (auto i = decltype(size)(0); i < size; ++i) {
                    result.PushBack(lhs.CGetMatrixElement(isLhsScalar ? decltype(i)(0) : i) * rhs.CGetFloatElement(isRhsScalar ? decltype(i)(0) : i));
                }
                break;

            case DatumType::Integer:
                for (auto i = decltype(size)(0); i < size; ++i) {
                    result.PushBack(lhs.CGetMatrixElement(isLhsScalar ? decltype(i)(0) : i) * static_cast<Datum::Float>(rhs.CGetIntegerElement(isRhsScalar ? decltype(i)(0) : i)));
                }
                break;

            case DatumType::Matrix:
                for (auto i = decltype(size)(0); i < size; ++i) {
                    result.PushBack(lhs.CGetMatrixElement(isLhsScalar ? decltype(i)(0) : i) * rhs.CGetMatrixElement(isRhsScalar ? decltype(i)(0) : i));
                }
                break;

            }
            break;

        }

        return result;
    }

    Datum ReversePolishEvaluator::Negate(Datum& input) const {
        switch (input.ActualType()) {

        case DatumType::Float:
            return ForEachUnaryConstFloatDatumElement(input, [](Datum::size_type, const Datum::Float& element){ return -element; });

        case DatumType::Matrix:
            return ForEachUnaryConstMatrixDatumElement(input, [](Datum::size_type, const Datum::Matrix& element){ return glm::inverse(element); });

        case DatumType::Vector:
            return ForEachUnaryConstVectorDatumElement(input, [](Datum::size_type, const Datum::Vector& element){ return -element; });

        }

        return ForEachUnaryConstDatumPredicate(input, [](const Datum& d, Datum::size_type i){ return !(d.IsTruthy(i)); });
    }

    Datum ReversePolishEvaluator::Subtraction(Datum& lhs, const Datum& rhs) const {
        const auto& operationName = TO_STRING_OPERATION_ID[static_cast<uint8_t>(OperationID::SUBTRACTION)];

        auto lhsSize = lhs.Size();
        auto rhsSize = rhs.Size();

        bool isLhsScalar = lhsSize == decltype(lhsSize)(1);
        bool isRhsScalar = rhsSize == decltype(rhsSize)(1);

        if (!(isLhsScalar || isRhsScalar) && (lhsSize != rhsSize)) {
            throw std::invalid_argument("Operation \""s + operationName + "\" needs either datums of the same size or for one or both datums to be of size 1!"s);
        }

        auto lhsType = lhs.ActualType();

        if ((lhsType != DatumType::Float) && (lhsType != DatumType::Integer) && (lhsType != DatumType::Vector) && (lhsType != DatumType::Matrix)) {
            throw std::invalid_argument("Operation \""s + operationName + "\" cannot be done with a LHS of type "s + ToStringDatumType(lhsType) + "!"s);
        }

        auto rhsType = rhs.ActualType();

        if ((rhsType != DatumType::Float) && (rhsType != DatumType::Integer) && (rhsType != DatumType::Vector) && (rhsType != DatumType::Matrix)) {
            throw std::invalid_argument("Operation \""s + operationName + "\" cannot be done with a RHS of type "s + ToStringDatumType(rhsType) + "!"s);
        }

        if ((lhsType != rhsType) && !(((lhsType == DatumType::Float) && (rhsType == DatumType::Integer)) || ((lhsType == DatumType::Integer) && (rhsType == DatumType::Float)))) {
            throw std::invalid_argument("Operation \""s + operationName + "\" is invalid with the LHS-RHS type combination of "s + ToStringDatumType(lhsType) + "-"s + ToStringDatumType(rhsType) + "!"s);
        }

        auto size = isLhsScalar ? rhsSize : lhsSize;
        Datum result{};

        switch (lhsType) {

        case DatumType::Float:
            for (auto i = decltype(size)(0); i < size; ++i) {
                result.PushBack(
                    lhs.CGetFloatElement(isLhsScalar ? decltype(i)(0) : i)
                    - ((rhsType == DatumType::Float)
                        ? rhs.CGetFloatElement(isRhsScalar ? decltype(i)(0) : i)
                        : static_cast<Datum::Float>(rhs.CGetIntegerElement(isRhsScalar ? decltype(i)(0) : i))
                    )
                );
            }
            break;

        case DatumType::Integer:
            if (rhsType == DatumType::Integer) {
                for (auto i = decltype(size)(0); i < size; ++i) {
                    result.PushBack(lhs.CGetIntegerElement(isLhsScalar ? decltype(i)(0) : i) - rhs.CGetIntegerElement(isRhsScalar ? decltype(i)(0) : i));
                }
            } else {
                for (auto i = decltype(size)(0); i < size; ++i) {
                    result.PushBack(lhs.CGetIntegerElement(isLhsScalar ? decltype(i)(0) : i) - rhs.CGetFloatElement(isRhsScalar ? decltype(i)(0) : i));
                }
            }
            break;

        case DatumType::Vector:
            for (auto i = decltype(size)(0); i < size; ++i) {
                result.PushBack(lhs.CGetVectorElement(isLhsScalar ? decltype(i)(0) : i) - rhs.CGetVectorElement(isRhsScalar ? decltype(i)(0) : i));
            }
            break;

        case DatumType::Matrix:
            for (auto i = decltype(size)(0); i < size; ++i) {
                result.PushBack(lhs.CGetMatrixElement(isLhsScalar ? decltype(i)(0) : i) - rhs.CGetMatrixElement(isRhsScalar ? decltype(i)(0) : i));
            }
            break;

        }

        return result;
    }
}
