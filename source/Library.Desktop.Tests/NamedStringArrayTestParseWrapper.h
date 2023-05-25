#pragma once
#include "TestParseWrapper.h"
#include "Vector.h"

namespace LibraryDesktopTests {
    class NamedStringArrayTestParseWrapper final : public TestParseWrapper {
        RTTI_DECLARATIONS(NamedStringArrayTestParseWrapper, Wrapper);

    public:
        using array_type = FieaGameEngine::Vector<std::string>;

        inline explicit NamedStringArrayTestParseWrapper(array_type::size_type capacity = array_type::size_type{1})
            : TestParseWrapper()
            , Strings{capacity, FieaGameEngine::DefaultGrowCapacity{}} {}

        inline WrapperSharedPointer Create() const override { return std::make_shared<NamedStringArrayTestParseWrapper>(); }

        array_type Strings;
        std::string Name;

    };
}
