#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "Bar.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std::literals::string_literals;

namespace LibraryDesktopTests {
    TEST_CLASS(RTTITests) {

    private:
        inline static _CrtMemState _startMemState;

    public:
        TEST_METHOD_INITIALIZE(Initialize) {
    #if defined(DEBUG) || defined(_DEBUG)
            _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
            _CrtMemCheckpoint(&_startMemState);
    #endif
        }

        TEST_METHOD_CLEANUP(Cleanup) {
    #if defined(DEBUG) || defined(_DEBUG)
            _CrtMemState endMemState, diffMemState;
            _CrtMemCheckpoint(&endMemState);

            if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState)) {
                _CrtMemDumpStatistics(&diffMemState);
                Assert::Fail(L"Memory Leaks!");
            }
    #endif
        }

        TEST_METHOD(BarTest) {
            Bar b{1};
            Foo f{2};

            Assert::IsFalse(b.Is(f.TypeIdInstance()));
            Assert::IsTrue(b.Is(Bar::TypeIdClass()));
            Assert::IsFalse(b.Equals(&f));
            Assert::IsTrue(b.Equals(&b));
            Assert::IsNull(b.As<Foo>());
            Assert::IsNotNull(b.As<Bar>());
            Assert::IsNull(f.As<Bar>());
            Assert::IsNotNull(f.As<Foo>());

            const Bar ar{3};

            Assert::IsTrue(b.Is(ar.TypeIdInstance()));
            Assert::IsFalse(b.Equals(&ar));
            Assert::IsNotNull(ar.As<Bar>());
            Assert::IsNull(ar.As<Foo>());
        }
    };
}
