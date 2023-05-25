#pragma once
#include <algorithm>
#include <cctype>
#include <locale>
#include <string>

namespace FieaGameEngine {
    extern const std::string FALSE_STRING_LOWER;
    extern const std::string FALSE_STRING_UPPER;
    extern const std::string TRUE_STRING_LOWER;
    extern const std::string TRUE_STRING_UPPER;
    extern const float FLOAT_PI;

    inline std::string string_tolower(const std::string& str) {
        std::string lower{str};
        std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c){ return static_cast<unsigned char>(std::tolower(c)); });
        return lower;
    }

    inline void string_ltrim_inplace(std::string& ref) {
        ref.erase(ref.begin(), std::find_if(ref.begin(), ref.end(), [](unsigned char c){ return !std::isspace(c); }));
    }

    inline void string_rtrim_inplace(std::string& ref) {
        ref.erase(std::find_if(ref.rbegin(), ref.rend(), [](unsigned char c){ return !std::isspace(c); }).base(), ref.end());
    }

    inline void string_trim_inplace(std::string& ref) {
        string_rtrim_inplace(ref);
        string_ltrim_inplace(ref);
    }

    inline std::string string_ltrim(const std::string& str) { std::string trimd{str}; string_ltrim_inplace(trimd); return trimd; }
    inline std::string string_rtrim(const std::string& str) { std::string trimd{str}; string_rtrim_inplace(trimd); return trimd; }
    inline std::string string_trim(const std::string& str) { std::string trimd{str}; string_trim_inplace(trimd); return trimd; }
}
