#include "pch.h"
#include "TypeScopeJsonParseHelper.h"
#include "Datum.h"

namespace FieaGameEngine::ScopeJsonParse {
    RTTI_DEFINITIONS(TypeScopeJsonParseHelper);

    bool TypeScopeJsonParseHelper::TryParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex) {
        using namespace std::literals::string_literals;

        ScopeParseWrapper* scopeWrapper;

        if (!IsAbleToParse(wrapper, key, isArrayElement, scopeWrapper)) {
            return false;
        }

        if (!value.isString()) {
            throw std::logic_error("Keyword \""s + ScopeParseWrapper::KEYWORD_TYPE + "\" must have a string value!"s);
        }

        auto str = value.asString();
        auto type = StringToDatumType(str);

        auto frame = scopeWrapper->TopStackFrame();
        UpdateAndValidateWrapperStackFrame(*frame, key);

        switch (type) {

        case Datum::DatumType::Integer:
        case Datum::DatumType::Float:
        case Datum::DatumType::String:
        case Datum::DatumType::Vector:
        case Datum::DatumType::Matrix:
            frame->_scope->Append(frame->_name).SetType(type);
            break;

        case Datum::DatumType::Table:
        case Datum::DatumType::InternalTable:
        case Datum::DatumType::ExternalTable:
        case Datum::DatumType::Pointer:
            throw std::logic_error("Datum type "s + str + " cannot be used to deserialize. Did you mean to put \""s + ScopeParseWrapper::TYPE_OBJECT + "\"?"s);

        default:
            if (str != ScopeParseWrapper::TYPE_OBJECT) {
                throw std::logic_error("Invalid type "s + str);
            }
            {
                if (frame->_scope->IsContainingKey(frame->_name)) {
                    auto apparent = frame->_scope->CAt(frame->_name).ApparentType();
                    if ((apparent != Datum::DatumType::Unknown) && (apparent != Datum::DatumType::Table)) {
                        throw std::logic_error("Cannot set "s + frame->_name + " to type \""s + ScopeParseWrapper::TYPE_OBJECT + "\", it is already type "s + ToStringDatumType(apparent));
                    }
                }
            }
            break;

        }

        return true;
    }
}
