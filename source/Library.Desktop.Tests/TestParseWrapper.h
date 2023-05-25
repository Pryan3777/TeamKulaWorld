#pragma once
#include "ParseCoordinator.h"

namespace LibraryDesktopTests {
    class TestParseWrapper : public FieaGameEngine::ParseCoordinator::Wrapper {
        RTTI_DECLARATIONS(TestParseWrapper, FieaGameEngine::ParseCoordinator::Wrapper);

    public:
        inline TestParseWrapper()
            : FieaGameEngine::ParseCoordinator::Wrapper()
            , _deepestDepth{depth_type(0)}
            , _incrementDepthCallsCount{0}
            , _decrementDepthCallsCount{0} {}

        virtual ~TestParseWrapper() = default;

        bool IncrementDepth(const std::string& subobjectName, bool isArray, std::size_t arrayIndex) override;

        inline bool DecrementDepth() override {
            ++_decrementDepthCallsCount;
            return FieaGameEngine::ParseCoordinator::Wrapper::DecrementDepth();
        }

        inline depth_type DeepestDepth() const { return _deepestDepth; }
        inline int IncrementDepthCallsCount() const { return _incrementDepthCallsCount; }
        inline int DecrementDepthCallsCount() const { return _decrementDepthCallsCount; }

    private:
        depth_type _deepestDepth;
        int _incrementDepthCallsCount;
        int _decrementDepthCallsCount;

    };
}
