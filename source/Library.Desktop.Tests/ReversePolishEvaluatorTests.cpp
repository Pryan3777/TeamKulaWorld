#include "pch.h"
#include "CppUnitTest.h"
#include "AttributedSignatureRegistry.h"
#include "AttributedTestMonster.h"
#include "ReversePolishEvaluator.h"
#include "ShuntingYardParser.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std::literals::string_literals;

namespace LibraryDesktopTests {
    inline bool FloatsAreEquivalent(float lhs, float rhs) {
        return 0.001f > std::abs(lhs - rhs);
    }

    TEST_CLASS(ReversePolishEvaluatorTests) {

    private:
        inline static _CrtMemState _startMemState;

        using size_type = Datum::size_type;
        using DatumType = FieaGameEngine::Datum::DatumType;
        using key_type = Attributed::key_type;

    public:
        TEST_METHOD_INITIALIZE(Initialize) {
            AttributedSignatureRegistry::RegisterSignatures<AttributedThing>();
            AttributedSignatureRegistry::RegisterSignatures<AttributedTestMonster, AttributedThing>();

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

            AttributedSignatureRegistry::UnregisterSignatures<AttributedTestMonster>();
            AttributedSignatureRegistry::UnregisterSignatures<AttributedThing>();
        }

        TEST_METHOD(ScalarAddition) {
            ReversePolishEvaluator eval{};
            ShuntingYardParser yard{};

            auto result = eval.Evaluate(yard.Parse("3 + 5"s));

            Assert::AreEqual(size_type(1), result.Size());
            Assert::AreEqual(8, result.CFrontInteger());

            result = eval.Evaluate("8 4.5 +"s);

            Assert::AreEqual(size_type(1), result.Size());
            Assert::IsTrue(FloatsAreEquivalent(12.5f, result.CFrontFloat()));

            result = eval.Evaluate("12.5 44.2 7 + +"s);

            Assert::AreEqual(size_type(1), result.Size());
            Assert::IsTrue(FloatsAreEquivalent(63.7f, result.CFrontFloat()));

            result = eval.Evaluate("vector<1.0|2.0|3.0|4.0> vector<4.0|3.0|2.0|1.0> +"s);

            Assert::AreEqual(size_type(1), result.Size());

            Datum::Vector vectorSum = result.CFrontVector();

            Assert::IsTrue(FloatsAreEquivalent(5.f, vectorSum.x));
            Assert::IsTrue(FloatsAreEquivalent(5.f, vectorSum.y));
            Assert::IsTrue(FloatsAreEquivalent(5.f, vectorSum.z));
            Assert::IsTrue(FloatsAreEquivalent(5.f, vectorSum.w));

            result = eval.Evaluate("fish bone +"s);

            Assert::AreEqual(size_type(1), result.Size());
            Assert::AreEqual("fishbone"s, result.CFrontString());
        }

        TEST_METHOD(ScalarSubtraction) {
            ReversePolishEvaluator eval{};

            auto result = eval.Evaluate("3 5 -"s);

            Assert::AreEqual(size_type(1), result.Size());
            Assert::AreEqual(-2, result.CFrontInteger());

            result = eval.Evaluate("8 4.5 -"s);

            Assert::AreEqual(size_type(1), result.Size());
            Assert::IsTrue(FloatsAreEquivalent(3.5f, result.CFrontFloat()));

            result = eval.Evaluate("12.5 44.2 7 - -"s);

            Assert::AreEqual(size_type(1), result.Size());
            Assert::IsTrue(FloatsAreEquivalent(-24.7f, result.CFrontFloat()));

            result = eval.Evaluate("vector<1.0|2.0|3.0|4.0> vector<4.0|3.0|2.0|1.0> -"s);

            Assert::AreEqual(size_type(1), result.Size());

            Datum::Vector vectorSum = result.CFrontVector();

            Assert::IsTrue(FloatsAreEquivalent(-3.f, vectorSum.x));
            Assert::IsTrue(FloatsAreEquivalent(-1.f, vectorSum.y));
            Assert::IsTrue(FloatsAreEquivalent(1.f, vectorSum.z));
            Assert::IsTrue(FloatsAreEquivalent(3.f, vectorSum.w));
        }

        TEST_METHOD(ScalarTrigonometry) {
            ReversePolishEvaluator eval{};
            ShuntingYardParser yard{};

            auto result = eval.Evaluate("180 deg->rad cos"s);

            Assert::AreEqual(size_type(1), result.Size());
            Assert::IsTrue(FloatsAreEquivalent(-1.f, result.CFrontFloat()));

            Scope scope{};
            scope.Append("Number") = 5;

            result = eval.Evaluate(yard.Parse("this.Number + (this.Number = 2 * cos(deg->rad(180)))"s), scope);

            Assert::IsTrue(FloatsAreEquivalent(-2.f, scope["Number"].CFrontFloat()));
            Assert::AreEqual(size_type(1), result.Size());
            Assert::IsTrue(FloatsAreEquivalent(-4.f, result.CFrontFloat()));
        }

        TEST_METHOD(DatumAddition) {
            ReversePolishEvaluator eval{};
            Scope root{};

            auto& firstIntegers = root.AppendScope("Integers"s);
            firstIntegers.Append("Values"s) = { 0, 1, 2, 3, 4 };
            const Datum originalFirstIntegers = firstIntegers["Values"s];
            auto& secondIntegers = root.AppendScope("Integers"s);
            secondIntegers.Append("Values"s) = { -1, 1, -1, 1, -1 };
            const Datum originalSecondIntegers = secondIntegers["Values"s];
            auto& firstFloats = root.AppendScope("Floats"s);
            firstFloats.Append("Values"s) = { 0.2f, 0.3f, 0.4f, 0.5f, 0.6f };
            const Datum originalFirstFloats = firstFloats["Values"s];
            auto& firstStrings = root.AppendScope("Strings"s);
            firstStrings.Append("Values"s) = { "My favorite "s, "Your "s, "The school's worst "s };
            const Datum originalFirstStrings = firstStrings["Values"s];
            auto& secondStrings = root.AppendScope("Strings"s);
            secondStrings.Append("Values"s) = { " gumbo"s, " tank"s, " scandal!!!"s };
            const Datum originalSecondStrings = secondStrings["Values"s];

            auto result = eval.Evaluate("Integers[1].Values Integers[0].Values 1 + +"s, root);

            Assert::AreEqual(Datum{ 0, 3, 2, 5, 4 }, result);
            Assert::AreEqual(originalFirstIntegers, firstIntegers["Values"s]);
            Assert::AreEqual(originalSecondIntegers, secondIntegers["Values"s]);

            result = eval.Evaluate("Floats.Values Integers.Values 2.0 + +"s, root);

            Assert::AreEqual(Datum{ 2.2f, 3.3f, 4.4f, 5.5f, 6.6f }, result);
            Assert::AreEqual(originalFirstFloats, firstFloats["Values"s]);
            Assert::AreEqual(originalFirstIntegers, firstIntegers["Values"s]);

            result = eval.Evaluate("Strings[0].Values shrimp Strings[1].Values + +"s, root);

            Assert::AreEqual(Datum{ "My favorite shrimp gumbo"s, "Your shrimp tank"s, "The school's worst shrimp scandal!!!"s }, result);
            Assert::AreEqual(originalFirstStrings, firstStrings["Values"s]);
            Assert::AreEqual(originalSecondStrings, secondStrings["Values"s]);
        }

        TEST_METHOD(MultiplicationAndAddition) {
            ReversePolishEvaluator eval{};
            ShuntingYardParser yard{};
            Scope root{};

            root.Append("val"s) = 3.f;
            root.AppendScope("Ints"s).Append("one"s) = 1;

            Datum result = eval.Evaluate(yard.Parse("(5 + this.val) * (6 / (this.val - Ints.one)) + Ints.one"s), root);

            Assert::AreEqual(size_type(1), result.Size());
            Assert::IsTrue(FloatsAreEquivalent(25.f, result.CFrontFloat()));
        }
    };
}