#include "pch.h"
#include "AttributedReaction.h"
#include "AttributedEventArgs.h"
#include <regex>

using namespace std::literals::string_literals;

namespace FieaGameEngine {
    RTTI_DEFINITIONS(AttributedReaction);

    const typename AttributedReaction::String AttributedReaction::REACTS_TO_SUBTYPE_REGEX_KEY = "ReactsToSubtypeRegex"s;

    AttributedReaction::AttributedReaction(const String& name)
        : Reaction{AttributedReaction::TypeIdClass(), name, { AttributedEventArgs::TypeIdClass() }}
    {
        _reactsToSubtypeRegexes = &(At(REACTS_TO_SUBTYPE_REGEX_KEY));
    }
    AttributedReaction::AttributedReaction(IdType idOfSignaturesToAppend, const String& name)
        : Reaction{idOfSignaturesToAppend, name, { AttributedEventArgs::TypeIdClass() }}
    {
        _reactsToSubtypeRegexes = &(At(REACTS_TO_SUBTYPE_REGEX_KEY));
    }

    AttributedReaction& AttributedReaction::operator=(const AttributedReaction& other) {
        if (this != &other) {
            Reaction::operator=(other);
            _reactsToSubtypeRegexes = &(At(REACTS_TO_SUBTYPE_REGEX_KEY));
        }
        return *this;
    }

    AttributedReaction& AttributedReaction::operator=(AttributedReaction&& other) noexcept {
        if (this != &other) {
            Reaction::operator=(std::move(other));
            _reactsToSubtypeRegexes = &(At(REACTS_TO_SUBTYPE_REGEX_KEY));
            other._reactsToSubtypeRegexes = nullptr;
        }
        return *this;
    }

    bool AttributedReaction::IsReactingTo(const IEventArgs& args) const {
        assert(_reactsToSubtypeRegexes != nullptr);
        assert(args.Is(AttributedEventArgs::TypeIdClass()));
        const auto& attrargs = static_cast<const AttributedEventArgs&>(args);

        auto size = _reactsToSubtypeRegexes->Size();
        for (auto i = decltype(size)(0); i < size; ++i) {
            auto regex = std::regex{GetReactsToSubtypeRegex(i)};
            if (std::regex_match(attrargs.GetSubtype(), regex)) {
                return Reaction::IsReactingTo(args);
            }
        }

        return false;
    }

    void AttributedReaction::ReactBeforeActionList(const IEventArgs& args) {
        Reaction::ReactBeforeActionList(args);

        assert(args.Is(AttributedEventArgs::TypeIdClass()));

        Scope pushedArguments{};
        static_cast<const AttributedEventArgs&>(args).CForEachAuxiliaryAttribute([&pushedArguments](const key_type& key, const Datum& datum) {
            pushedArguments.Append(key) = datum;
            return false;
        });

        auto& stack = GameplayState::Singleton().ActionArgumentStack();
        stack.Push(std::move(pushedArguments));
        _pushedArguments.Push(&(*(stack.CTop())));
    }

    void AttributedReaction::ReactAfterActionList(const IEventArgs& args) {
        Reaction::ReactAfterActionList(args);

        auto& stack = GameplayState::Singleton().ActionArgumentStack();
        assert((&(*(stack.CTop()))) == *(_pushedArguments.CTop()));
        _pushedArguments.Pop();
        stack.Pop();
    }
}
