#include "pch.h"
#include "CppUnitTest.h"
#include "AttributedSignatureRegistry.h"
#include "AttributedTestMonster.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std::literals::string_literals;

namespace LibraryDesktopTests {
    TEST_CLASS(AttributedSignatureRegistryTests) {

    private:
        inline static _CrtMemState _startMemState;

        using size_type = Datum::size_type;
        using key_type = Attributed::key_type;

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

        TEST_METHOD(CreateAndDestroySingleton) {
            // false because it should have been initialized in the TestModule
            Assert::IsFalse(AttributedSignatureRegistry::CreateSingleton());
            Assert::IsTrue(AttributedSignatureRegistry::DestroySingleton());
            Assert::IsFalse(AttributedSignatureRegistry::DestroySingleton());
            Assert::IsTrue(AttributedSignatureRegistry::CreateSingleton());
        }

        TEST_METHOD(RegisterAndUnregister) {
            Assert::IsTrue(AttributedSignatureRegistry::RegisterSignatures<AttributedThing>());
            Assert::IsFalse(AttributedSignatureRegistry::RegisterSignatures<AttributedThing>());
            Assert::IsTrue(AttributedSignatureRegistry::RegisterSignatures<AttributedTestMonster, AttributedThing>());
            Assert::IsFalse(AttributedSignatureRegistry::RegisterSignatures<AttributedTestMonster, AttributedThing>());

            AttributedThing thing{};
            AttributedTestMonster monster{};

            UNREFERENCED_LOCAL(thing);
            UNREFERENCED_LOCAL(monster);

            Assert::IsTrue(AttributedSignatureRegistry::UnregisterSignatures<AttributedTestMonster>());
            Assert::IsFalse(AttributedSignatureRegistry::UnregisterSignatures<AttributedTestMonster>());
            Assert::IsTrue(AttributedSignatureRegistry::UnregisterSignatures<AttributedThing>());
            Assert::IsFalse(AttributedSignatureRegistry::UnregisterSignatures<AttributedThing>());

            Assert::ExpectException<std::logic_error>([]() {
                AttributedSignatureRegistry::RegisterSignatures<AttributedTestMonster, AttributedThing>();
            });
        }

        TEST_METHOD(AttributedsNeedRegistration) {
            Assert::ExpectException<std::logic_error>([](){ AttributedThing thing{}; UNREFERENCED_LOCAL(thing); });

            AttributedSignatureRegistry::RegisterSignatures<AttributedThing>();

            AttributedThing zero{};
            AttributedThing one{};
            one.LevelUp();

            AttributedSignatureRegistry::UnregisterSignatures<AttributedThing>();

            Assert::ExpectException<std::logic_error>([&zero, &one](){ zero = one; });
            Assert::ExpectException<std::logic_error>([&zero, &one](){ AttributedThing two{zero}; UNREFERENCED_LOCAL(two); });
        }

        TEST_METHOD(FindSignatures) {
            AttributedSignatureRegistry::RegisterSignatures<AttributedThing>();
            SignatureVector fromClass = AttributedThing::Signatures();
            SignatureVector fromRegistry = AttributedSignatureRegistry::FindSignatures(AttributedThing::TypeIdClass())->second;

            Assert::IsTrue(std::equal(fromClass.cbegin(), fromClass.cend(), fromRegistry.cbegin()));

            AttributedSignatureRegistry::UnregisterSignatures<AttributedThing>();
        }
    };
}