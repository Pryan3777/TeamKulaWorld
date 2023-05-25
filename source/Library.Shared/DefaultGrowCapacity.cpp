#include "pch.h"
#include "DefaultGrowCapacity.h"
#include <stdexcept>

namespace FieaGameEngine {
    std::size_t DefaultGrowCapacity::operator()(std::size_t size, std::size_t capacity) const {
        const static std::size_t INITIAL = 8;
        const static std::size_t ADD_THRESHOLD = 1024;

        if (size < capacity) {
            return capacity;
        }

        size += (size < ADD_THRESHOLD)
            ? ((size > std::size_t(0)) ? size : INITIAL)
            : ADD_THRESHOLD;

        if (size < capacity) {
            using namespace std::literals::string_literals;

            throw std::overflow_error("Grown capacity would exceed size limit of a std::size_t"s);
        }

        return size;
    }
}
