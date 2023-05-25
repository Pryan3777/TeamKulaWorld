#include "pch.h"
#include "CppUnitTest.h"
#include "JsonParseCoordinator.h"
#include "ScopeParseWrapper.h"
#include "AllScopeJsonParseHelper.h"
#include "ScopeJsonKeyTokenTransmuter.h"
#include "ToStringSpecializations.h"
#include "AttributedSignatureRegistry.h"
#include "ActionIf.h"
#include "ActionExpression.h"
#include "ActionIncrement.h"
#include "GameObject.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace FieaGameEngine::ScopeJsonParse;
using namespace FieaGameEngine::Actions;
using namespace std::literals::string_literals;

namespace LibraryDesktopTests {
    inline bool FloatsAreEquivalent(float lhs, float rhs) {
        return 0.001f > std::abs(lhs - rhs);
    }

    TEST_CLASS(ActionTests) {

    private:
        inline static _CrtMemState _startMemState;

        using depth_type = ParseCoordinator::Wrapper::depth_type;
        using size_type = Scope::size_type;

    public:
        TEST_METHOD_INITIALIZE(Initialize) {
            AttributedSignatureRegistry::RegisterSignatures<Transform>();
            AttributedSignatureRegistry::RegisterSignatures<GameObject>();
            AttributedSignatureRegistry::RegisterSignatures<Action>();
            AttributedSignatureRegistry::RegisterSignatures<ActionExpression, Action>();
            AttributedSignatureRegistry::RegisterSignatures<ActionIncrement, Action>();
            AttributedSignatureRegistry::RegisterSignatures<ActionList, Action>();
            AttributedSignatureRegistry::RegisterSignatures<ActionIf, ActionList>();
            ScopeFactory::Register();
            TransformFactory::Register();
            GameObjectFactory::Register();
            ActionExpressionFactory::Register();
            ActionListFactory::Register();
            ActionIfFactory::Register();
            ActionIncrementFactory::Register();

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

            ActionIncrementFactory::Unregister();
            ActionIfFactory::Unregister();
            ActionListFactory::Unregister();
            ActionExpressionFactory::Unregister();
            GameObjectFactory::Unregister();
            TransformFactory::Unregister();
            ScopeFactory::Unregister();
            AttributedSignatureRegistry::UnregisterSignatures<ActionIf>();
            AttributedSignatureRegistry::UnregisterSignatures<ActionList>();
            AttributedSignatureRegistry::UnregisterSignatures<ActionIncrement>();
            AttributedSignatureRegistry::UnregisterSignatures<ActionExpression>();
            AttributedSignatureRegistry::UnregisterSignatures<Action>();
            AttributedSignatureRegistry::UnregisterSignatures<GameObject>();
            AttributedSignatureRegistry::UnregisterSignatures<Transform>();
        }

        TEST_METHOD(Update) {
            auto root = std::make_shared<Scope>();
            auto wrapper = std::make_shared<ScopeParseWrapper>(root);
            auto coordinator = JsonParseCoordinator(wrapper);

            Assert::IsTrue(coordinator.PushBackHelper(std::make_unique<AllScopeJsonParseHelper>()));
            Assert::IsTrue(coordinator.PushBackTransmuter(std::make_unique<ScopeJsonKeyTokenTransmuter>()));

            coordinator.DeserializeIntoWrapperFromFile(R"(Files\TestActionParse.json)"s);

            Assert::IsTrue(root->IsContainingKey("Scene"s));

            auto* scene = root->At("Scene"s).FrontTable().As<GameObject>();

            Assert::IsNotNull(scene);
            Assert::AreEqual(size_type(1), scene->GetChildren().Size());
            Assert::AreEqual(size_type(1), scene->GetActions().Size());

            auto* actionif = scene->At("Actions"s).FrontTable().As<ActionIf>();

            Assert::IsNotNull(actionif);
            Assert::AreEqual(size_type(1), actionif->GetActions().Size());
            Assert::AreEqual(size_type(1), actionif->GetElseActions().Size());
            Assert::AreEqual(0, actionif->Condition());

            auto* trueaction = actionif->At("Actions").FrontTable().As<ActionIncrement>();

            Assert::IsNotNull(trueaction);
            Assert::AreEqual(0, trueaction->Current());
            Assert::AreEqual(2, trueaction->Step());

            auto* falseaction = actionif->At("ElseActions"s).FrontTable().As<ActionIncrement>();

            Assert::IsNotNull(falseaction);
            Assert::AreEqual(5, falseaction->Current());
            Assert::AreEqual(-1, falseaction->Step());

            auto* child = scene->At("Children"s).FrontTable().As<GameObject>();

            Assert::IsNotNull(child);
            Assert::IsTrue(child->GetChildren().IsEmpty());
            Assert::AreEqual(size_type(1), child->GetActions().Size());

            auto* counter = child->At("Actions"s).FrontTable().As<ActionIncrement>();

            Assert::IsNotNull(counter);
            Assert::AreEqual(0, counter->Current());
            Assert::AreEqual(1, counter->Step());

            scene->Update(GameTime{});

            Assert::AreEqual(0, trueaction->Current());
            Assert::AreEqual(4, falseaction->Current());
            Assert::AreEqual(1, counter->Current());

            actionif->SetCondition(1);
            scene->Update(GameTime{});

            Assert::AreEqual(2, trueaction->Current());
            Assert::AreEqual(4, falseaction->Current());
            Assert::AreEqual(2, counter->Current());

            actionif->SetCondition(-5000);
            scene->Update(GameTime{});

            Assert::AreEqual(4, trueaction->Current());
            Assert::AreEqual(4, falseaction->Current());
            Assert::AreEqual(3, counter->Current());

            actionif->SetCondition(0);
            scene->Update(GameTime{});

            Assert::AreEqual(4, trueaction->Current());
            Assert::AreEqual(3, falseaction->Current());
            Assert::AreEqual(4, counter->Current());
        }

        TEST_METHOD(UpdateExpression) {
            auto root = std::make_shared<Scope>();
            auto wrapper = std::make_shared<ScopeParseWrapper>(root);
            wrapper->CreateShuntingYardParser();
            auto coordinator = JsonParseCoordinator(wrapper);

            Assert::IsTrue(coordinator.PushBackHelper(std::make_unique<AllScopeJsonParseHelper>()));
            Assert::IsTrue(coordinator.PushBackTransmuter(std::make_unique<ScopeJsonKeyTokenTransmuter>()));

            coordinator.DeserializeIntoWrapperFromFile(R"(Files\TestActionExpressionParse.json)"s);

            Assert::IsTrue(root->IsContainingKey("Scene"s));
            Assert::IsTrue(root->At("Scene"s).FrontTable().Is(GameObject::TypeIdClass()));

            auto& scene = static_cast<GameObject&>(root->At("Scene"s).FrontTable());

            Assert::AreEqual(size_type(7), scene.Size());
            Assert::AreEqual(size_type(4), scene.GetActions().Size());

            Assert::IsTrue(scene.GetAction(size_type(0)).Is(ActionExpression::TypeIdClass()));

            const auto& isRoot = static_cast<const ActionExpression&>(scene.GetAction(size_type(0)));

            Assert::IsTrue(scene.GetAction(size_type(1)).Is(ActionExpression::TypeIdClass()));

            const auto& incrementCounter = static_cast<const ActionExpression&>(scene.GetAction(size_type(1)));

            Assert::IsTrue(scene.GetAction(size_type(2)).Is(ActionExpression::TypeIdClass()));

            const auto& isCounterGteTen = static_cast<const ActionExpression&>(scene.GetAction(size_type(2)));

            Assert::IsTrue(scene.GetAction(size_type(3)).Is(ActionExpression::TypeIdClass()));

            const auto& setAngle = static_cast<const ActionExpression&>(scene.GetAction(size_type(3)));

            Assert::IsTrue(isRoot.GetLastResult().IsEmpty());
            Assert::IsTrue(incrementCounter.GetLastResult().IsEmpty());
            Assert::IsTrue(isCounterGteTen.GetLastResult().IsEmpty());
            Assert::IsTrue(setAngle.GetLastResult().IsEmpty());

            Assert::IsFalse(isRoot.GetLastResult().IsTruthy());
            Assert::IsFalse(incrementCounter.GetLastResult().IsTruthy());
            Assert::IsFalse(isCounterGteTen.GetLastResult().IsTruthy());
            Assert::IsFalse(setAngle.GetLastResult().IsTruthy());

            Assert::AreEqual("this.Name Root =="s, isRoot.GetReversePolishNotatedExpression());
            Assert::AreEqual("this.Counter ++"s, incrementCounter.GetReversePolishNotatedExpression());
            Assert::AreEqual(size_t(0), isCounterGteTen.GetReversePolishNotatedExpression().find("this.Counter"));
            Assert::AreEqual(isCounterGteTen.GetReversePolishNotatedExpression().size() - "||"s.size(), isCounterGteTen.GetReversePolishNotatedExpression().find("||"));
            Assert::AreEqual(size_t(0), setAngle.GetReversePolishNotatedExpression().find("this.Angle"));
            Assert::AreEqual(setAngle.GetReversePolishNotatedExpression().size() - "="s.size(), setAngle.GetReversePolishNotatedExpression().find("="));

            const auto& counter = scene.CAt("Counter"s);

            Assert::AreEqual(Datum::DatumType::Integer, counter.ActualType());
            Assert::AreEqual(0, counter.CFrontInteger());

            const auto& angle = scene.CAt("Angle"s);

            Assert::AreEqual(Datum::DatumType::Float, angle.ActualType());
            Assert::IsTrue(FloatsAreEquivalent(0.f, angle.CFrontFloat()));

            scene.Update(GameTime{});

            Assert::IsFalse(isRoot.GetLastResult().IsEmpty());
            Assert::IsFalse(incrementCounter.GetLastResult().IsEmpty());
            Assert::IsFalse(isCounterGteTen.GetLastResult().IsEmpty());
            Assert::IsFalse(setAngle.GetLastResult().IsEmpty());

            auto isSceneNameRoot = isRoot.GetLastResult().IsTruthy();
            auto isCounterGteTenResult = isCounterGteTen.GetLastResult().IsTruthy();

            Assert::IsFalse(isCounterGteTenResult);
            Assert::IsTrue(isSceneNameRoot);
            Assert::AreEqual(1, incrementCounter.GetLastResult().CFrontInteger());
            Assert::AreEqual(1, counter.CFrontInteger());
            Assert::AreEqual(counter.CFrontInteger(), incrementCounter.GetLastResult().CFrontInteger());
            Assert::IsTrue(setAngle.GetLastResult().CFrontFloat() > 0.f);
            Assert::IsTrue(angle.CFrontFloat() > 0.f);
            Assert::IsTrue(FloatsAreEquivalent(angle.CFrontFloat(), setAngle.GetLastResult().CFrontFloat()));

            scene.Update(GameTime{});

            Assert::AreEqual(isSceneNameRoot, isRoot.GetLastResult().IsTruthy());
            Assert::AreEqual(2, incrementCounter.GetLastResult().CFrontInteger());
            Assert::AreEqual(2, counter.CFrontInteger());
            Assert::AreEqual(counter.CFrontInteger(), incrementCounter.GetLastResult().CFrontInteger());

            scene.Update(GameTime{});

            Assert::AreEqual(isSceneNameRoot, isRoot.GetLastResult().IsTruthy());
            Assert::AreEqual(3, incrementCounter.GetLastResult().CFrontInteger());
            Assert::AreEqual(3, counter.CFrontInteger());
            Assert::AreEqual(counter.CFrontInteger(), incrementCounter.GetLastResult().CFrontInteger());
            Assert::IsTrue(FloatsAreEquivalent(2.f, setAngle.GetLastResult().CFrontFloat()));
            Assert::IsTrue(FloatsAreEquivalent(2.f, angle.CFrontFloat()));
            Assert::IsTrue(FloatsAreEquivalent(angle.CFrontFloat(), setAngle.GetLastResult().CFrontFloat()));

            scene.Update(GameTime{});

            Assert::AreEqual(isSceneNameRoot, isRoot.GetLastResult().IsTruthy());
            Assert::AreEqual(4, incrementCounter.GetLastResult().CFrontInteger());
            Assert::AreEqual(4, counter.CFrontInteger());
            Assert::AreEqual(counter.CFrontInteger(), incrementCounter.GetLastResult().CFrontInteger());

            scene.Update(GameTime{});

            Assert::AreEqual(isSceneNameRoot, isRoot.GetLastResult().IsTruthy());
            Assert::AreEqual(5, incrementCounter.GetLastResult().CFrontInteger());
            Assert::AreEqual(5, counter.CFrontInteger());
            Assert::AreEqual(counter.CFrontInteger(), incrementCounter.GetLastResult().CFrontInteger());

            scene.Update(GameTime{});

            Assert::AreEqual(isSceneNameRoot, isRoot.GetLastResult().IsTruthy());
            Assert::AreEqual(6, incrementCounter.GetLastResult().CFrontInteger());
            Assert::AreEqual(6, counter.CFrontInteger());
            Assert::AreEqual(counter.CFrontInteger(), incrementCounter.GetLastResult().CFrontInteger());
            Assert::IsTrue(FloatsAreEquivalent(0.f, setAngle.GetLastResult().CFrontFloat()));
            Assert::IsTrue(FloatsAreEquivalent(0.f, angle.CFrontFloat()));
            Assert::IsTrue(FloatsAreEquivalent(angle.CFrontFloat(), setAngle.GetLastResult().CFrontFloat()));

            scene.Update(GameTime{});
            scene.Update(GameTime{});
            scene.Update(GameTime{});

            Assert::AreEqual(isSceneNameRoot, isRoot.GetLastResult().IsTruthy());
            Assert::AreEqual(9, incrementCounter.GetLastResult().CFrontInteger());
            Assert::AreEqual(9, counter.CFrontInteger());
            Assert::AreEqual(counter.CFrontInteger(), incrementCounter.GetLastResult().CFrontInteger());
            Assert::IsTrue(FloatsAreEquivalent(-2.f, setAngle.GetLastResult().CFrontFloat()));
            Assert::IsTrue(FloatsAreEquivalent(-2.f, angle.CFrontFloat()));
            Assert::IsTrue(FloatsAreEquivalent(angle.CFrontFloat(), setAngle.GetLastResult().CFrontFloat()));

            Assert::AreEqual(isCounterGteTenResult, isCounterGteTen.GetLastResult().IsTruthy());

            scene.Update(GameTime{});

            Assert::AreNotEqual(isCounterGteTenResult, isCounterGteTen.GetLastResult().IsTruthy());

            isCounterGteTenResult = isCounterGteTen.GetLastResult().IsTruthy();

            scene.Update(GameTime{});

            Assert::AreEqual(isCounterGteTenResult, isCounterGteTen.GetLastResult().IsTruthy());

            scene.Update(GameTime{});

            Assert::AreEqual(isSceneNameRoot, isRoot.GetLastResult().IsTruthy());
            Assert::AreEqual(12, incrementCounter.GetLastResult().CFrontInteger());
            Assert::AreEqual(12, counter.CFrontInteger());
            Assert::AreEqual(counter.CFrontInteger(), incrementCounter.GetLastResult().CFrontInteger());
            Assert::IsTrue(FloatsAreEquivalent(0.f, setAngle.GetLastResult().CFrontFloat()));
            Assert::IsTrue(FloatsAreEquivalent(0.f, angle.CFrontFloat()));
            Assert::IsTrue(FloatsAreEquivalent(angle.CFrontFloat(), setAngle.GetLastResult().CFrontFloat()));
        }
    };
}