#pragma once
#include "ElementScopeJsonParseHelper.h"

namespace FieaGameEngine::ScopeJsonParse {
    RTTI_TEMPLATED_DEFINITIONS(ElementScopeJsonParseHelper, T);

    template <typename T> inline bool ElementScopeJsonParseHelper<T>::IsAbleToParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool, Json::ArrayIndex index) {
        ScopeParseWrapper* _;
        Scope::key_type __;
        return IsAbleToParse(wrapper, key, value, index, _, __);
    }

    template <typename T> inline bool ElementScopeJsonParseHelper<T>::EndParse(Wrapper& wrapper, const Json::String& key) { return IJsonParseHelper::EndParse(wrapper, key); }
    template <typename T> inline std::unique_ptr<IJsonParseHelper> ElementScopeJsonParseHelper<T>::Create() const { return std::make_unique<ElementScopeJsonParseHelper>(); }

    template <typename T> inline bool ElementScopeJsonParseHelper<T>::IsAbleToParse(
        Wrapper& wrapper,
        const Json::String& key,
        const Json::Value& value,
        Json::ArrayIndex index,
        ScopeParseWrapper*& converted,
        Scope::key_type& scopeKey
    ) {
        if (!ScopeJsonParseHelper::IsAbleToParse(wrapper, converted) || !IsJsonValueCorrectType(value)) {
            scopeKey = key;
            return false;
        }

        auto cframe = converted->CTopStackFrame();
        scopeKey = (key == ScopeParseWrapper::KEYWORD_VALUE) ? cframe->_name : key;
        auto cfound = cframe->_scope->CFind(scopeKey);

        if (cfound == cframe->_scope->cend()) {
            return (index == Json::ArrayIndex(0)) && IsJsonValueAsStringCorrectlyFormatted(value);
        }

        return (cfound->second.Size() >= Datum::size_type(index)) && (cfound->second.ActualType() == ElementDatumActualType());
    }

    template <typename T> inline bool ElementScopeJsonParseHelper<T>::TryParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool, Json::ArrayIndex index) {
        ScopeParseWrapper* scopeWrapper;
        Scope::key_type scopeKey;

        if (IsAbleToParse(wrapper, key, value, index, scopeWrapper, scopeKey)) {
            auto frame = scopeWrapper->TopStackFrame();
            UpdateAndValidateWrapperStackFrame(*frame, key);
            auto& appended = frame->_scope->Append(scopeKey);
            appended.SetType(ElementDatumActualType());

            auto s = Datum::size_type(index);
            if (appended.Size() > s) {
                SetToDatumFromJsonValue(appended, s, value);
            } else {
                assert(appended.Size() == s);
                PushBackToDatumFromJsonValue(appended, value);
            }

            return true;
        }

        return false;
    }

    template <typename T> inline bool ElementScopeJsonParseHelper<T>::IsJsonValueAsStringCorrectlyFormatted(const Json::Value&) const { return true; }
    template<> inline bool ElementScopeJsonParseHelper<Datum::Vector>::IsJsonValueAsStringCorrectlyFormatted(const Json::Value& value) const {
        return value.asString().substr(0, Datum::VECTOR_STRING_FORMAT_PREFIX.size()) == Datum::VECTOR_STRING_FORMAT_PREFIX;
    }
    template<> inline bool ElementScopeJsonParseHelper<Datum::Matrix>::IsJsonValueAsStringCorrectlyFormatted(const Json::Value& value) const {
        return value.asString().substr(0, Datum::MATRIX_STRING_FORMAT_PREFIX.size()) == Datum::MATRIX_STRING_FORMAT_PREFIX;
    }

    template <typename T> inline bool ElementScopeJsonParseHelper<T>::IsJsonValueCorrectType(const Json::Value&) const { static_assert(__notImplemented, "IsJsonValueCorrectType not implemented!"); }
    template<> inline bool ElementScopeJsonParseHelper<Datum::Integer>::IsJsonValueCorrectType(const Json::Value& value) const { return value.isInt(); }
    template<> inline bool ElementScopeJsonParseHelper<Datum::Float>::IsJsonValueCorrectType(const Json::Value& value) const { return value.isNumeric(); }
    template<> inline bool ElementScopeJsonParseHelper<Datum::String>::IsJsonValueCorrectType(const Json::Value& value) const { return value.isString(); }
    template<> inline bool ElementScopeJsonParseHelper<Datum::Vector>::IsJsonValueCorrectType(const Json::Value& value) const { return value.isString(); }
    template<> inline bool ElementScopeJsonParseHelper<Datum::Matrix>::IsJsonValueCorrectType(const Json::Value& value) const { return value.isString(); }

    template <typename T> inline Datum::DatumType ElementScopeJsonParseHelper<T>::ElementDatumActualType() const { static_assert(__notImplemented, "ElementDatumActualType not implemented!"); }
    template<> inline Datum::DatumType ElementScopeJsonParseHelper<Datum::Integer>::ElementDatumActualType() const { return Datum::DatumType::Integer; }
    template<> inline Datum::DatumType ElementScopeJsonParseHelper<Datum::Float>::ElementDatumActualType() const { return Datum::DatumType::Float; }
    template<> inline Datum::DatumType ElementScopeJsonParseHelper<Datum::String>::ElementDatumActualType() const { return Datum::DatumType::String; }
    template<> inline Datum::DatumType ElementScopeJsonParseHelper<Datum::Vector>::ElementDatumActualType() const { return Datum::DatumType::Vector; }
    template<> inline Datum::DatumType ElementScopeJsonParseHelper<Datum::Matrix>::ElementDatumActualType() const { return Datum::DatumType::Matrix; }

    template <typename T> inline void ElementScopeJsonParseHelper<T>::SetToDatumFromJsonValue(Datum&, Datum::size_type, const Json::Value&) { static_assert(__notImplemented, "SetToDatumFromJsonValue not implemented!"); }
    template<> inline void ElementScopeJsonParseHelper<Datum::Integer>::SetToDatumFromJsonValue(Datum& d, Datum::size_type s, const Json::Value& v) { d.SetElement(v.asInt(), s); }
    template<> inline void ElementScopeJsonParseHelper<Datum::Float>::SetToDatumFromJsonValue(Datum& d, Datum::size_type s, const Json::Value& v) { d.SetElement(v.asFloat(), s); }
    template<> inline void ElementScopeJsonParseHelper<Datum::String>::SetToDatumFromJsonValue(Datum& d, Datum::size_type s, const Json::Value& v) { d.SetElement(v.asString(), s); }
    template<> inline void ElementScopeJsonParseHelper<Datum::Vector>::SetToDatumFromJsonValue(Datum& d, Datum::size_type s, const Json::Value& v) { d.SetElementFromString(v.asString(), s); }
    template<> inline void ElementScopeJsonParseHelper<Datum::Matrix>::SetToDatumFromJsonValue(Datum& d, Datum::size_type s, const Json::Value& v) { d.SetElementFromString(v.asString(), s); }

    template <typename T> inline void ElementScopeJsonParseHelper<T>::PushBackToDatumFromJsonValue(Datum&, const Json::Value&) { static_assert(__notImplemented, "PushBackToDatumFromJsonValue not implemented!"); }
    template<> inline void ElementScopeJsonParseHelper<Datum::Integer>::PushBackToDatumFromJsonValue(Datum& d, const Json::Value& v) { d.PushBack(v.asInt()); }
    template<> inline void ElementScopeJsonParseHelper<Datum::Float>::PushBackToDatumFromJsonValue(Datum& d, const Json::Value& v) { d.PushBack(v.asFloat()); }
    template<> inline void ElementScopeJsonParseHelper<Datum::String>::PushBackToDatumFromJsonValue(Datum& d, const Json::Value& v) { d.PushBack(v.asString()); }
    template<> inline void ElementScopeJsonParseHelper<Datum::Vector>::PushBackToDatumFromJsonValue(Datum& d, const Json::Value& v) { d.PushBackFromString(v.asString()); }
    template<> inline void ElementScopeJsonParseHelper<Datum::Matrix>::PushBackToDatumFromJsonValue(Datum& d, const Json::Value& v) { d.PushBackFromString(v.asString()); }
}
