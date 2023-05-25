#pragma once
#include "TestParseWrapper.h"
#include "Vector.h"

namespace LibraryDesktopTests {
    class NamedIntArrayTestParseWrapper final : public TestParseWrapper {
        RTTI_DECLARATIONS(NamedIntArrayTestParseWrapper, TestParseWrapper);

    public:
        using array_type = FieaGameEngine::Vector<int>;

        inline explicit NamedIntArrayTestParseWrapper(array_type::size_type capacity = array_type::size_type{1})
            : TestParseWrapper()
            , Ints{capacity, FieaGameEngine::DefaultGrowCapacity{}} {}

        inline WrapperSharedPointer Create() const override { return std::make_shared<NamedIntArrayTestParseWrapper>(); }

        array_type Ints;
        std::string Name;

    };
}
