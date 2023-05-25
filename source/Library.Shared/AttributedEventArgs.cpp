#include "pch.h"
#include "AttributedEventArgs.h"

using namespace std::literals::string_literals;

namespace FieaGameEngine {
    RTTI_DEFINITIONS(AttributedEventArgs);

    const typename AttributedEventArgs::key_type AttributedEventArgs::SUBTYPE_KEY = "Subtype"s;
}
