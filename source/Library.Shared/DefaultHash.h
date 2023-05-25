#pragma once
#include <cstdlib>
#include <cstdint>
#include <cstring>

namespace FieaGameEngine {
    template <typename T> struct DefaultHash final {
        std::size_t operator()(const T&);
    };

    template<> struct DefaultHash<std::size_t> final {
        std::size_t operator()(const std::size_t&);
    };

    template<> struct DefaultHash<std::ptrdiff_t> final {
        std::size_t operator()(const std::ptrdiff_t&);
    };

    template<> struct DefaultHash<std::uint16_t> final {
        std::size_t operator()(const std::uint16_t&);
    };

    template<> struct DefaultHash<std::int16_t> final {
        std::size_t operator()(const std::int16_t&);
    };

    template<> struct DefaultHash<std::uint8_t> final {
        std::size_t operator()(const std::uint8_t&);
    };

    template<> struct DefaultHash<std::int8_t> final {
        std::size_t operator()(const std::int8_t&);
    };

    template<> struct DefaultHash<char*> final {
        std::size_t operator()(const char*);
    };

    template<> struct DefaultHash<const char*> final {
        std::size_t operator()(const char*);
    };

    template<> struct DefaultHash<char* const> final {
        std::size_t operator()(const char* const);
    };

    template<> struct DefaultHash<const char* const> final {
        std::size_t operator()(const char* const);
    };

    template<> struct DefaultHash<std::string> final {
        std::size_t operator()(const std::string&);
    };

    namespace __DefaultHash_NumericLimits {
        std::size_t max_char_value();
    }
}

#include "DefaultHash.inl"
