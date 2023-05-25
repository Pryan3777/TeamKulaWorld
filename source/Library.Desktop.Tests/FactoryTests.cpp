#include "pch.h"
#include "CppUnitTest.h"
#include "Bar.h"
#include "AttributedSignatureRegistry.h"
#include "AttributedTestMonster.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std::literals::string_literals;

namespace LibraryDesktopTests {
    TEST_CLASS(FactoryTests) {

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

        TEST_METHOD(CreateBar) {
            const Bar::number_type TEST_NUMBER = 10;

            BarFactory factory{};
            std::unique_ptr<Bar> created = factory.CreateBar(TEST_NUMBER);

            Assert::AreEqual(Bar::TypeNameClass(), factory.CreatedClassName());
            Assert::IsTrue(created.operator bool());

            BarFactory::Register();
            std::unique_ptr<Scope> staticCreated = Factory<Scope>::StaticCreate(Bar::TypeNameClass());
            BarFactory::Unregister();

            Assert::IsTrue(staticCreated.operator bool());
            Assert::IsTrue(staticCreated->Is(Bar::TypeIdClass()));
            Assert::AreEqual(Bar::TypeIdClass(), staticCreated->TypeIdInstance());
            Assert::AreNotEqual(created->Number(), staticCreated->As<Bar>()->Number());
            Assert::AreEqual(TEST_NUMBER, created->Number());
            Assert::AreEqual(0, staticCreated->As<Bar>()->Number());
            Assert::IsFalse(created.get() == staticCreated.get());
        }

        TEST_METHOD(CreateAttributedThing) {
            AttributedThingFactory factory{};
            std::unique_ptr<AttributedThing> created = factory.CreateAttributedThing();

            Assert::AreEqual(AttributedThing::TypeNameClass(), factory.CreatedClassName());
            Assert::IsTrue(created.operator bool());

            AttributedThingFactory::Register();
            std::unique_ptr<Scope> staticCreated = Factory<Scope>::StaticCreate(AttributedThing::TypeNameClass());
            AttributedThingFactory::Unregister();

            Assert::IsTrue(staticCreated.operator bool());
            Assert::IsTrue(staticCreated->Is(AttributedThing::TypeIdClass()));
            Assert::AreEqual(AttributedThing::TypeIdClass(), staticCreated->TypeIdInstance());
            Assert::AreEqual(created->Level(), staticCreated->As<AttributedThing>()->Level());
            Assert::AreEqual(created->MaxLevel(), staticCreated->As<AttributedThing>()->MaxLevel());
            Assert::AreEqual(created->MaxHealth(), staticCreated->As<AttributedThing>()->MaxHealth());
            Assert::AreEqual(created->CurrentHealth(), staticCreated->As<AttributedThing>()->CurrentHealth());
            Assert::IsTrue(created->Transform() == staticCreated->As<AttributedThing>()->Transform());
            Assert::IsFalse(created.get() == staticCreated.get());
        }

        TEST_METHOD(CreateAttributedTestMonster) {
            AttributedTestMonsterFactory factory{};
            std::unique_ptr<AttributedTestMonster> created = factory.CreateAttributedTestMonster();

            Assert::AreEqual(AttributedTestMonster::TypeNameClass(), factory.CreatedClassName());
            Assert::IsTrue(created.operator bool());

            AttributedTestMonsterFactory::Register();
            std::unique_ptr<Scope> staticCreated = Factory<Scope>::StaticCreate(AttributedTestMonster::TypeNameClass());
            AttributedTestMonsterFactory::Unregister();

            Assert::IsTrue(staticCreated.operator bool());
            Assert::IsTrue(staticCreated->Is(AttributedTestMonster::TypeIdClass()));
            Assert::AreEqual(AttributedTestMonster::TypeIdClass(), staticCreated->TypeIdInstance());
            Assert::AreEqual(created->Level(), staticCreated->As<AttributedTestMonster>()->Level());
            Assert::AreEqual(created->MaxLevel(), staticCreated->As<AttributedTestMonster>()->MaxLevel());
            Assert::AreEqual(created->MaxHealth(), staticCreated->As<AttributedTestMonster>()->MaxHealth());
            Assert::AreEqual(created->CurrentHealth(), staticCreated->As<AttributedTestMonster>()->CurrentHealth());
            Assert::IsTrue(created->Transform() == staticCreated->As<AttributedTestMonster>()->Transform());
            Assert::AreEqual(created->EntryMessage(), staticCreated->As<AttributedTestMonster>()->EntryMessage());
            Assert::IsTrue(created->Reward() == staticCreated->As<AttributedTestMonster>()->Reward());
            Assert::IsFalse(created.get() == staticCreated.get());
        }

        TEST_METHOD(TryFind) {
            Factory<Scope>* found = nullptr;

            Assert::IsFalse(Factory<Scope>::TryFind(Bar::TypeNameClass(), found));
            Assert::IsNull(found);

            BarFactory::Register();

            Assert::IsTrue(Factory<Scope>::TryFind(Bar::TypeNameClass(), found));
            Assert::IsNotNull(found);
            Assert::AreEqual(Bar::TypeNameClass(), found->CreatedClassName());
            Assert::AreEqual(Bar::TypeIdClass(), found->Create()->TypeIdInstance());

            BarFactory::Unregister();

            Assert::IsFalse(Factory<Scope>::TryFind(Bar::TypeNameClass(), found));
            Assert::IsNull(found);

            Assert::IsFalse(Factory<Scope>::TryFind(AttributedThing::TypeNameClass(), found));
            Assert::IsNull(found);

            AttributedThingFactory::Register();

            Assert::IsTrue(Factory<Scope>::TryFind(AttributedThing::TypeNameClass(), found));
            Assert::IsNotNull(found);
            Assert::AreEqual(AttributedThing::TypeNameClass(), found->CreatedClassName());
            Assert::AreEqual(AttributedThing::TypeIdClass(), found->Create()->TypeIdInstance());

            AttributedThingFactory::Unregister();

            Assert::IsFalse(Factory<Scope>::TryFind(AttributedThing::TypeNameClass(), found));
            Assert::IsNull(found);

            Assert::IsFalse(Factory<Scope>::TryFind(AttributedTestMonster::TypeNameClass(), found));
            Assert::IsNull(found);

            AttributedTestMonsterFactory::Register();

            Assert::IsTrue(Factory<Scope>::TryFind(AttributedTestMonster::TypeNameClass(), found));
            Assert::IsNotNull(found);
            Assert::AreEqual(AttributedTestMonster::TypeNameClass(), found->CreatedClassName());
            Assert::AreEqual(AttributedTestMonster::TypeIdClass(), found->Create()->TypeIdInstance());

            AttributedTestMonsterFactory::Unregister();

            Assert::IsFalse(Factory<Scope>::TryFind(AttributedTestMonster::TypeNameClass(), found));
            Assert::IsNull(found);
        }
    };
}