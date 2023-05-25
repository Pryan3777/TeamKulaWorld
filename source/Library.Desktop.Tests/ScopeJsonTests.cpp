#include "pch.h"
#include "CppUnitTest.h"
#include "JsonParseCoordinator.h"
#include "ScopeParseWrapper.h"
#include "AllScopeJsonParseHelper.h"
#include "ScopeJsonKeyTokenTransmuter.h"
#include "ToStringSpecializations.h"
#include "AttributedThing.h"
#include "AttributedSignatureRegistry.h"
#include "NoOpJsonValueTransmuter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace FieaGameEngine::ScopeJsonParse;
using namespace std::literals::string_literals;

namespace LibraryDesktopTests {
    TEST_CLASS(ScopeJsonTests) {

    private:
        inline static _CrtMemState _startMemState;

        using depth_type = ParseCoordinator::Wrapper::depth_type;
        using size_type = Scope::size_type;

    public:
        TEST_METHOD_INITIALIZE(Initialize) {
            AttributedSignatureRegistry::RegisterSignatures<AttributedThing>();
            ScopeFactory::Register();
            AttributedThingFactory::Register();

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

            AttributedThingFactory::Unregister();
            ScopeFactory::Unregister();
            AttributedSignatureRegistry::UnregisterSignatures<AttributedThing>();
        }

        TEST_METHOD(ParseScope) {
            auto root = std::make_shared<Scope>();
            auto wrapper = std::make_shared<ScopeParseWrapper>(root);
            auto coordinator = JsonParseCoordinator(wrapper);

            Assert::IsTrue(coordinator.PushBackHelper(std::make_unique<AllScopeJsonParseHelper>()));
            Assert::IsTrue(coordinator.PushBackTransmuter(std::make_unique<ScopeJsonKeyTokenTransmuter>()));
            Assert::IsTrue(coordinator.PushBackTransmuter(std::make_unique<NoOpJsonValueTransmuter>()));
            Assert::IsFalse(coordinator.PushBackTransmuter(std::make_unique<NoOpJsonValueTransmuter>()));

            Assert::IsNotNull(coordinator.GetTransmuter(NoOpJsonValueTransmuter::TypeIdClass()));
            Assert::IsTrue(coordinator.RemoveTransmuter(NoOpJsonValueTransmuter::TypeIdClass()));
            Assert::IsNull(coordinator.GetTransmuter(NoOpJsonValueTransmuter::TypeIdClass()));
            Assert::IsFalse(coordinator.RemoveTransmuter(NoOpJsonValueTransmuter::TypeIdClass()));
            Assert::IsTrue(coordinator.PushBackTransmuter(std::make_unique<NoOpJsonValueTransmuter>()));

            coordinator.DeserializeIntoWrapperFromFile(R"(Files\TestScopeJsonParse.json)"s);

            Assert::AreEqual(depth_type(0), wrapper->Depth());
            Assert::AreEqual(size_type(1), root->Size());

            const auto& encounter = root->CAt("Encounter"s).CFrontTable();

            Assert::AreEqual(size_type(6), encounter.Size());
            Assert::AreEqual(size_type(1), encounter.CAt("Difficulty"s).Size());
            Assert::AreEqual(5, encounter.CAt("Difficulty"s).CFrontInteger());
            Assert::AreEqual(size_type(1), encounter.CAt("LocationName"s).Size());
            Assert::AreEqual("Dark Forest"s, encounter.CAt("LocationName"s).CFrontString());
            Assert::AreEqual(size_type(4), encounter.CAt("Occurrences"s).Size());
            Assert::AreEqual(2, encounter.CAt("Occurrences"s).CGetIntegerElement(size_type(0)));
            Assert::AreEqual(4, encounter.CAt("Occurrences"s).CGetIntegerElement(size_type(1)));
            Assert::AreEqual(6, encounter.CAt("Occurrences"s).CGetIntegerElement(size_type(2)));
            Assert::AreEqual(8, encounter.CAt("Occurrences"s).CGetIntegerElement(size_type(3)));

            const auto& weather = encounter.CAt("Weather"s).CFrontTable();

            Assert::AreEqual(size_type(2), weather.Size());
            Assert::AreEqual(size_type(2), weather.CAt("IDs"s).Size());
            Assert::AreEqual("Normal"s, weather.CAt("IDs"s).CGetStringElement(size_type(0)));
            Assert::AreEqual("Rainy"s, weather.CAt("IDs"s).CGetStringElement(size_type(1)));
            Assert::AreEqual(size_type(2), weather.CAt("probabilities"s).Size());
            Assert::IsTrue(0.001f > std::abs(0.8f - weather.CAt("probabilities"s).CGetFloatElement(size_type(0))));
            Assert::IsTrue(0.001f > std::abs(0.2f - weather.CAt("probabilities"s).CGetFloatElement(size_type(1))));

            const auto& rewards = encounter.CAt("Rewards"s);

            Assert::AreEqual(size_type(2), rewards.Size());

            const auto& darkHelm = rewards.CGetTableElement(size_type(0));

            Assert::AreEqual(size_type(2), darkHelm.Size());
            Assert::AreEqual(100, darkHelm.CAt("value"s).CFrontInteger());
            Assert::AreEqual("Dark Helm"s, darkHelm.CAt("name"s).CFrontString());

            const auto& rareGem = rewards.CGetTableElement(size_type(1));

            Assert::AreEqual(size_type(3), rareGem.Size());
            Assert::AreEqual(200, rareGem.CAt("value"s).CFrontInteger());
            Assert::AreEqual("Rare Gem"s, rareGem.CAt("name"s).CFrontString());
            Assert::AreEqual(size_type(3), rareGem.CAt("probability"s).Size());
            Assert::IsTrue(0.001f > std::abs(0.5f - rareGem.CAt("probability"s).CGetFloatElement(size_type(0))));
            Assert::IsTrue(0.001f > std::abs(0.6f - rareGem.CAt("probability"s).CGetFloatElement(size_type(1))));
            Assert::IsTrue(0.001f > std::abs(0.75f - rareGem.CAt("probability"s).CGetFloatElement(size_type(2))));

            const auto& enemies = encounter.CAt("Enemies"s);

            Assert::AreEqual(size_type(3), enemies.Size());

            const auto& healthEnemy = enemies.CGetTableElement(size_type(0));

            Assert::AreEqual(size_type(7), healthEnemy.Size());
            Assert::IsTrue(healthEnemy.Is(AttributedThing::TypeIdClass()));
            Assert::AreEqual(size_type(2), healthEnemy.CAt("Health"s).Size());
            Assert::IsTrue(0.001f > std::abs(100.f - healthEnemy.CAt("Health"s).CGetFloatElement(size_type(0))));
            Assert::IsTrue(0.001f > std::abs(200.f - healthEnemy.CAt("Health"s).CGetFloatElement(size_type(1))));

            const auto& spawnEnemy = enemies.CGetTableElement(size_type(1));

            Assert::AreEqual(size_type(8), spawnEnemy.Size());
            Assert::IsTrue(healthEnemy.Is(AttributedThing::TypeIdClass()));
            Assert::AreEqual(size_type(1), spawnEnemy.CAt("Health"s).Size());
            Assert::IsTrue(0.001f > std::abs(60.6f - spawnEnemy.CAt("Health"s).CGetFloatElement(size_type(0))));
            Assert::AreEqual(size_type(1), spawnEnemy.CAt("Level"s).Size());
            Assert::AreEqual(5, spawnEnemy.CAt("Level"s).CGetIntegerElement(size_type(0)));
            Assert::AreEqual(size_type(1), spawnEnemy.CAt("Spawn"s).Size());
            Assert::IsTrue(0.001f > std::abs(5.f - spawnEnemy.CAt("Spawn"s).CFrontVector().x));
            Assert::IsTrue(0.001f > std::abs(6.f - spawnEnemy.CAt("Spawn"s).CFrontVector().y));
            Assert::IsTrue(0.001f > std::abs(7.f - spawnEnemy.CAt("Spawn"s).CFrontVector().z));
            Assert::IsTrue(0.001f > std::abs(8.f - spawnEnemy.CAt("Spawn"s).CFrontVector().w));

            const auto& transformEnemy = enemies.CGetTableElement(size_type(2));

            Assert::AreEqual(size_type(8), transformEnemy.Size());
            Assert::IsTrue(healthEnemy.Is(AttributedThing::TypeIdClass()));
            Assert::AreEqual(size_type(3), transformEnemy.CAt("Health"s).Size());
            Assert::IsTrue(0.001f > std::abs(16.1f - transformEnemy.CAt("Health"s).CGetFloatElement(size_type(0))));
            Assert::IsTrue(0.001f > std::abs(32.f - transformEnemy.CAt("Health"s).CGetFloatElement(size_type(1))));
            Assert::IsTrue(0.001f > std::abs(63.9f - transformEnemy.CAt("Health"s).CGetFloatElement(size_type(2))));
            Assert::AreEqual(size_type(1), transformEnemy.CAt("Spawn"s).Size());
            Assert::IsTrue(0.001f > std::abs(1.f - transformEnemy.CAt("Spawn"s).CFrontVector().x));
            Assert::IsTrue(0.001f > std::abs(2.2f - transformEnemy.CAt("Spawn"s).CFrontVector().y));
            Assert::IsTrue(0.001f > std::abs(3.6f - transformEnemy.CAt("Spawn"s).CFrontVector().z));
            Assert::IsTrue(0.001f > std::abs(0.f - transformEnemy.CAt("Spawn"s).CFrontVector().w));
            Assert::AreEqual(size_type(1), transformEnemy.CAt("Transform"s).Size());
            Assert::IsTrue(0.001f > std::abs(3.f - transformEnemy.CAt("Transform"s).CFrontMatrix()[0].x));
            Assert::IsTrue(0.001f > std::abs(4.f - transformEnemy.CAt("Transform"s).CFrontMatrix()[0].y));
            Assert::IsTrue(0.001f > std::abs(4.f - transformEnemy.CAt("Transform"s).CFrontMatrix()[0].z));
            Assert::IsTrue(0.001f > std::abs(3.f - transformEnemy.CAt("Transform"s).CFrontMatrix()[0].w));
            Assert::IsTrue(0.001f > std::abs(0.f - transformEnemy.CAt("Transform"s).CFrontMatrix()[1].x));
            Assert::IsTrue(0.001f > std::abs(0.f - transformEnemy.CAt("Transform"s).CFrontMatrix()[1].y));
            Assert::IsTrue(0.001f > std::abs(0.f - transformEnemy.CAt("Transform"s).CFrontMatrix()[1].z));
            Assert::IsTrue(0.001f > std::abs(0.f - transformEnemy.CAt("Transform"s).CFrontMatrix()[1].w));
            Assert::IsTrue(0.001f > std::abs(0.f - transformEnemy.CAt("Transform"s).CFrontMatrix()[2].x));
            Assert::IsTrue(0.001f > std::abs(0.f - transformEnemy.CAt("Transform"s).CFrontMatrix()[2].y));
            Assert::IsTrue(0.001f > std::abs(0.f - transformEnemy.CAt("Transform"s).CFrontMatrix()[2].z));
            Assert::IsTrue(0.001f > std::abs(0.f - transformEnemy.CAt("Transform"s).CFrontMatrix()[2].w));
            Assert::IsTrue(0.001f > std::abs(5.5f - transformEnemy.CAt("Transform"s).CFrontMatrix()[3].x));
            Assert::IsTrue(0.001f > std::abs(5.5f - transformEnemy.CAt("Transform"s).CFrontMatrix()[3].y));
            Assert::IsTrue(0.001f > std::abs(5.5f - transformEnemy.CAt("Transform"s).CFrontMatrix()[3].z));
            Assert::IsTrue(0.001f > std::abs(5.5f - transformEnemy.CAt("Transform"s).CFrontMatrix()[3].w));
        }

        TEST_METHOD(TokenParseEdgeCases) {
            auto root = std::make_shared<Scope>();
            auto wrapper = std::make_shared<ScopeParseWrapper>(root);
            auto coordinator = JsonParseCoordinator(wrapper);
            coordinator.PushBackHelper(std::make_unique<AllScopeJsonParseHelper>());
            coordinator.PushBackTransmuter(std::make_unique<ScopeJsonKeyTokenTransmuter>());

            Assert::ExpectException<std::invalid_argument>([&coordinator](){ coordinator.DeserializeIntoWrapperFromString(R"({ "This string cannot work": "Oh no!" })"s); });
            Assert::ExpectException<std::invalid_argument>([&coordinator](){ coordinator.DeserializeIntoWrapperFromString(R"({ "number float thing": 123.456f })"s); });
        }
    };
}