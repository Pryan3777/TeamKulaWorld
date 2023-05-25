#include "pch.h"
#include "CppUnitTest.h"
#include "DefaultHash.h"
#include "ToStringSpecializations.h"
#include <cstdint>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace LibraryDesktopTests {
    TEST_CLASS(DefaultHashTests) {

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

        TEST_METHOD(SizeT) {
            using test_t = std::size_t;
            DefaultHash<test_t> func{};

            for (test_t i = test_t(0); i < test_t(16); ++i) {
                test_t value = test_t(1) << i;
                Assert::AreEqual(value, func(value));
                Assert::AreNotEqual(std::size_t(0), func(value));
            }
        }

        TEST_METHOD(PtrDiffT) {
            using test_t = std::ptrdiff_t;
            DefaultHash<test_t> func{};

            for (test_t i = test_t(0); i < test_t(16); ++i) {
                test_t value = test_t(1) << i;
                Assert::AreEqual(static_cast<std::size_t>(value), func(value));
                Assert::AreNotEqual(std::size_t(0), func(value));
            }
        }

        TEST_METHOD(UInt16T) {
            using test_t = std::uint16_t;
            DefaultHash<test_t> func{};

            for (test_t i = test_t(0); i < test_t(12); ++i) {
                test_t value = test_t(1) << i;
                Assert::AreEqual(static_cast<std::size_t>(value), func(value));
                Assert::AreNotEqual(std::size_t(0), func(value));
            }
        }

        TEST_METHOD(Int16T) {
            using test_t = std::int16_t;
            DefaultHash<test_t> func{};

            for (test_t i = test_t(0); i < test_t(12); ++i) {
                test_t value = test_t(1) << i;
                Assert::AreEqual(static_cast<std::size_t>(value), func(value));
                Assert::AreNotEqual(std::size_t(0), func(value));
            }
        }

        TEST_METHOD(UInt8T) {
            using test_t = std::uint8_t;
            DefaultHash<test_t> func{};

            for (test_t i = test_t(0); i < test_t(6); ++i) {
                test_t value = test_t(1) << i;
                Assert::AreEqual(static_cast<std::size_t>(value), func(value));
                Assert::AreNotEqual(std::size_t(0), func(value));
            }
        }

        TEST_METHOD(Int8T) {
            using test_t = std::int8_t;
            DefaultHash<test_t> func{};

            for (test_t i = test_t(0); i < test_t(6); ++i) {
                test_t value = test_t(1) << i;
                Assert::AreEqual(static_cast<std::size_t>(value), func(value));
                Assert::AreNotEqual(std::size_t(0), func(value));
            }
        }

        TEST_METHOD(CharStar) {
            DefaultHash<char*> func{};

            const char* test = "test";
            char copy[5];
            strcpy_s(copy, test);

            Assert::AreEqual(func(test), func(copy));
        }

        TEST_METHOD(ConstCharStar) {
            DefaultHash<const char*> func{};

            const char* test = "test";

            Assert::AreEqual(func("test"), func(test));
        }

        TEST_METHOD(CharStarConst) {
            DefaultHash<char* const> func{};

            const char* const test = "test";
            char copy[5];
            strcpy_s(copy, test);

            Assert::AreEqual(func(test), func(copy));
        }

        TEST_METHOD(ConstCharStarConst) {
            DefaultHash<const char* const> func{};

            const char* const test = "test";

            Assert::AreEqual(func("test"), func(test));
        }

        TEST_METHOD(String) {
            using namespace std::literals::string_literals;

            const std::string TEST = "test"s;
            DefaultHash<std::string> func{};

            Assert::AreEqual(func("test"s), func(TEST));
        }

        TEST_METHOD(Bar) {
            class Bar {
                std::size_t _first{std::size_t(1)};
                std::ptrdiff_t _second{std::ptrdiff_t(-1)};

            public:
                Bar() = default;
                inline Bar(std::size_t first, std::ptrdiff_t second) : _first{first}, _second{second}{}

                inline std::size_t GetFirst() const { return _first; }
                inline std::ptrdiff_t GetSecond() const { return _second; }

                inline void SetFirst(std::size_t first) { _first = first; }
                inline void SetSecond(std::ptrdiff_t second) { _second = second; }
            } tst{std::size_t(0), std::ptrdiff_t(-2)}, cmp{};

            DefaultHash<Bar> func{};

            Assert::AreNotEqual(func(tst), func(cmp));

            cmp.SetFirst(tst.GetFirst());
            cmp.SetSecond(tst.GetSecond());

            Assert::AreEqual(func(tst), func(cmp));
        }
    };
}