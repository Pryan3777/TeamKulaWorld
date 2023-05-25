#pragma once
#include "DefaultHash.h"

namespace FieaGameEngine {
    template <typename T> std::size_t DefaultHash<T>::operator()(const T& value) {
        std::size_t randAdd = std::size_t(29);
        std::size_t randMult = std::size_t(7);
        const std::byte* bytes = reinterpret_cast<const std::byte*>(&value);
        std::size_t size = sizeof(T);
        std::size_t hash = std::size_t(0);

        for (std::size_t i = std::size_t(0); i < size; ++i) {
            hash += randAdd + (randMult * static_cast<std::size_t>(bytes[i]));
        }

        return hash;
    }

    inline std::size_t DefaultHash<std::size_t>::operator()(const std::size_t& value) { return value; }
    inline std::size_t DefaultHash<std::ptrdiff_t>::operator()(const std::ptrdiff_t& value) { return static_cast<std::size_t>(value); }
    inline std::size_t DefaultHash<std::uint16_t>::operator()(const std::uint16_t& value) { return static_cast<std::size_t>(value); }
    inline std::size_t DefaultHash<std::int16_t>::operator()(const std::int16_t& value) { return static_cast<std::size_t>(value); }
    inline std::size_t DefaultHash<std::uint8_t>::operator()(const std::uint8_t& value) { return static_cast<std::size_t>(value); }
    inline std::size_t DefaultHash<std::int8_t>::operator()(const std::int8_t& value) { return static_cast<std::size_t>(value); }

    inline std::size_t DefaultHash<char*>::operator()(const char* str) {
        std::size_t randRawMult = std::size_t(11);
        std::size_t randModMult = std::size_t(7);
        std::size_t randAdd = std::size_t(31);
        std::size_t hash = std::size_t(0);

        for (std::size_t i = std::size_t(0); *(str + i) != 0; ++i) {
            hash += randAdd + (randModMult * (hash % __DefaultHash_NumericLimits::max_char_value())) + (randRawMult * static_cast<std::size_t>(*(str + i)));
        }

        return hash;
    }

    inline std::size_t DefaultHash<const char*>::operator()(const char* str) {
        std::size_t randRawMult = std::size_t(11);
        std::size_t randModMult = std::size_t(7);
        std::size_t randAdd = std::size_t(31);
        std::size_t hash = std::size_t(0);

        for (std::size_t i = std::size_t(0); *(str + i) != 0; ++i) {
            hash += randAdd + (randModMult * (hash % __DefaultHash_NumericLimits::max_char_value())) + (randRawMult * static_cast<std::size_t>(*(str + i)));
        }

        return hash;
    }

    inline std::size_t DefaultHash<char* const>::operator()(const char* const str) {
        std::size_t randRawMult = std::size_t(11);
        std::size_t randModMult = std::size_t(7);
        std::size_t randAdd = std::size_t(31);
        std::size_t hash = std::size_t(0);

        for (std::size_t i = std::size_t(0); *(str + i) != 0; ++i) {
            hash += randAdd + (randModMult * (hash % __DefaultHash_NumericLimits::max_char_value())) + (randRawMult * static_cast<std::size_t>(*(str + i)));
        }

        return hash;
    }

    inline std::size_t DefaultHash<const char* const>::operator()(const char* const str) {
        std::size_t randRawMult = std::size_t(11);
        std::size_t randModMult = std::size_t(7);
        std::size_t randAdd = std::size_t(31);
        std::size_t hash = std::size_t(0);

        for (std::size_t i = std::size_t(0); *(str + i) != 0; ++i) {
            hash += randAdd + (randModMult * (hash % __DefaultHash_NumericLimits::max_char_value())) + (randRawMult * static_cast<std::size_t>(*(str + i)));
        }

        return hash;
    }

    inline std::size_t DefaultHash<std::string>::operator()(const std::string& str) {
        std::size_t randRawMult = std::size_t(11);
        std::size_t randModMult = std::size_t(7);
        std::size_t randAdd = std::size_t(31);
        std::size_t hash = std::size_t(0);

        for (const char& c : str) {
            hash += randAdd + (randModMult * (hash % __DefaultHash_NumericLimits::max_char_value())) + (randRawMult * static_cast<std::size_t>(c));
        }

        return hash;
    }
}
