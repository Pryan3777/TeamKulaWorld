#include "pch.h"
#include "DefaultHash.h"
#include <limits>

namespace FieaGameEngine::__DefaultHash_NumericLimits {
    std::size_t max_char_value() {
        using namespace std;
        return static_cast<std::size_t>(numeric_limits<char>::max());
    }
}
