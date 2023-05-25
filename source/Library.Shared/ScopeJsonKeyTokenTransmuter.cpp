#include "pch.h"
#include "ScopeJsonKeyTokenTransmuter.h"
#include <cassert>
#include <regex>

namespace FieaGameEngine::ScopeJsonParse {
    RTTI_DEFINITIONS(ScopeJsonKeyTokenTransmuter);

    typename ScopeJsonKeyTokenTransmuter::pair_type ScopeJsonKeyTokenTransmuter::Transmute(Wrapper& wrapper, const Json::String& key, const Json::Value& value) {
        if (!IsAbleToTransmute(wrapper, key, value)) {
            auto output = std::make_pair(key, std::cref(value));

            _history.Push(std::make_unique<ScopeJsonTransmuteHistory>(output, false, wrapper.Depth()));
            return output;
        }

        std::string tokenString{key};
        std::regex extractTokens{"\\s*(\\b[^\\s]+\\b)(.*)"};
        std::smatch stringMatches{};
        Vector<std::string> tokens{};

        while (std::regex_match(tokenString, stringMatches, extractTokens)) {
            tokens.PushBack(stringMatches[1].str());
            tokenString = stringMatches[2].str();
        }

        assert(!tokens.IsEmpty());
        auto last = tokens.Size() - decltype(tokens)::size_type(1);

        bool isIncremented = wrapper.IncrementDepth(tokens[last], false, 0);
        assert(isIncremented);
        UNREFERENCED_LOCAL(isIncremented);

        _history.Push(std::make_unique<ScopeJsonTransmuteHistory>(std::make_pair(key, std::cref(value)), true, wrapper.Depth()));

        for (auto i = decltype(tokens)::size_type(0); i < last; ++i) {
            ParseKeyToken(wrapper, tokens[i], i, last - i);
        }

        return std::make_pair(ScopeParseWrapper::KEYWORD_VALUE, std::cref(value));
    }

    typename ScopeJsonKeyTokenTransmuter::pair_type ScopeJsonKeyTokenTransmuter::EndTransmutation(Wrapper& wrapper, const Json::String&, const Json::Value&) {
        const auto* history = static_cast<ScopeJsonTransmuteHistory*>(_history.CTop()->get());

        assert(wrapper.Depth() == (history->Depth()));

        if (history->IsAbleToTransmute()) {
            bool isDecremented = wrapper.DecrementDepth();
            assert(isDecremented);
            UNREFERENCED_LOCAL(isDecremented);
        }

        pair_type pair = history->Input();
        _history.Pop();
        return pair;
    }

    void ScopeJsonKeyTokenTransmuter::ParseKeyToken(Wrapper& wrapper, const std::string& token, std::size_t tokenIndex, std::size_t remainingTokenCount) {
        using namespace std::literals::string_literals;

        assert(remainingTokenCount > std::size_t(0));

        if (tokenIndex == std::size_t(0)) {
            switch (remainingTokenCount) {

            case std::size_t(1):
                if ((token == ScopeParseWrapper::TYPE_OBJECT) || IsTokenValidDatumType(token)) {
                    bool result = _typeHelper.TryParse(wrapper, ScopeParseWrapper::KEYWORD_TYPE, Json::Value{token}, false, 0);
                    assert(result);
                    UNREFERENCED_LOCAL(result);
                } else {
                    bool result = _classHelper.TryParse(wrapper, ScopeParseWrapper::KEYWORD_CLASS, Json::Value{token}, false, 0);
                    assert(result);
                    UNREFERENCED_LOCAL(result);
                }
                break;

            case std::size_t(2):
                if (token != ScopeParseWrapper::TYPE_OBJECT) {
                    throw std::invalid_argument("The first token in a collection of three tokens must be \""s + ScopeParseWrapper::TYPE_OBJECT + "\"!"s);
                }
                break;

            default:
                throw std::invalid_argument("Cannot parse token collection with more than three elements!"s);

            }
        } else {
            assert(tokenIndex == std::size_t(1));
            assert(remainingTokenCount == std::size_t(1));
            bool result = _classHelper.TryParse(wrapper, ScopeParseWrapper::KEYWORD_CLASS, Json::Value{token}, false, 0);
            assert(result);
            UNREFERENCED_LOCAL(result);
        }
    }

    bool ScopeJsonKeyTokenTransmuter::IsTokenValidDatumType(const std::string& token) const {
        auto type = StringToDatumType(token);
        return (type == Datum::DatumType::Integer)
            || (type == Datum::DatumType::Float)
            || (type == Datum::DatumType::String)
            || (type == Datum::DatumType::Vector)
            || (type == Datum::DatumType::Matrix);
    }
}
