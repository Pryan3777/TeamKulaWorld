#pragma once
#include "TestParseWrapper.h"
#include "Vector.h"

namespace LibraryDesktopTests {
    class NamedFloatArrayTestParseWrapper final : public TestParseWrapper {
        RTTI_DECLARATIONS(NamedFloatArrayTestParseWrapper, TestParseWrapper);

    public:
        using array_type = FieaGameEngine::Vector<float>;

        inline explicit NamedFloatArrayTestParseWrapper(array_type::size_type capacity = array_type::size_type{1})
            : TestParseWrapper()
            , Floats{capacity, FieaGameEngine::DefaultGrowCapacity{}} {}

        inline WrapperSharedPointer Create() const override { return std::make_shared<NamedFloatArrayTestParseWrapper>(); }

        array_type Floats;
        std::string Name;

    };
}
