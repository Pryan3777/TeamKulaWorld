#include "pch.h"
#include "TestParseWrapper.h"

namespace LibraryDesktopTests {
    RTTI_DEFINITIONS(TestParseWrapper);

    bool TestParseWrapper::IncrementDepth(const std::string& subobjectName, bool isArray, std::size_t arrayIndex) {
        ++_incrementDepthCallsCount;

        if (!FieaGameEngine::ParseCoordinator::Wrapper::IncrementDepth(subobjectName, isArray, arrayIndex)) {
            return false;
        }

        _deepestDepth = std::max(_deepestDepth, _depth);
        return true;
    }
}
