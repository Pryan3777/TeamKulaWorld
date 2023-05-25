#include "pch.h"
#include "CppUnitTest.h"
#include "ActionEvent.h"
#include "ActionIf.h"
#include "ActionIncrement.h"
#include "AttributedEventArgs.h"
#include "AttributedReaction.h"
#include "GameObject.h"
#include "GameplayState.h"
#include "JsonParseCoordinator.h"
#include "ScopeParseWrapper.h"
#include "AllScopeJsonParseHelper.h"
#include "ScopeJsonKeyTokenTransmuter.h"
#include "ToStringSpecializations.h"
#include <thread>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std::literals::string_literals;
using namespace std::chrono;
using namespace std::this_thread;

namespace LibraryDesktopTests {
    TEST_CLASS(AttributedReactionTests) {

    private:
        inline static _CrtMemState _startMemState;

        using size_type = Scope::size_type;

    public:
        TEST_METHOD_INITIALIZE(Initialize) {
            AttributedSignatureRegistry::RegisterSignatures<Transform>();
            AttributedSignatureRegistry::RegisterSignatures<GameObject>();
            AttributedSignatureRegistry::RegisterSignatures<Action>();
            AttributedSignatureRegistry::RegisterSignatures<Actions::ActionEvent, Action>();
            AttributedSignatureRegistry::RegisterSignatures<Actions::ActionIncrement, Action>();
            AttributedSignatureRegistry::RegisterSignatures<Actions::ActionList, Action>();
            AttributedSignatureRegistry::RegisterSignatures<Actions::ActionIf, Actions::ActionList>();
            AttributedSignatureRegistry::RegisterSignatures<Reaction, Actions::ActionList>();
            AttributedSignatureRegistry::RegisterSignatures<AttributedReaction, Reaction>();
            AttributedSignatureRegistry::RegisterSignatures<AttributedEventArgs>();

            GameObjectFactory::Register();
            Actions::ActionEventFactory::Register();
            Actions::ActionIfFactory::Register();
            Actions::ActionIncrementFactory::Register();
            AttributedReactionFactory::Register();
            AttributedEventArgsFactory::Register();

            Event::CreateSubscriptionsSingleton();
    #if defined(DEBUG) || defined(_DEBUG)
            _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
            _CrtMemCheckpoint(&_startMemState);
    #endif
        }

        TEST_METHOD_CLEANUP(Cleanup) {
            GameplayState::DestroySingleton();
            GameplayState::CreateSingleton();
            Event::DestroySubscriptionsSingleton();
            Event::CreateSubscriptionsSingleton();
    #if defined(DEBUG) || defined(_DEBUG)
            _CrtMemState endMemState, diffMemState;
            _CrtMemCheckpoint(&endMemState);

            if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState)) {
                _CrtMemDumpStatistics(&diffMemState);
                Assert::Fail(L"Memory Leaks!");
            }
    #endif
            Event::DestroySubscriptionsSingleton();

            AttributedEventArgsFactory::Unregister();
            AttributedReactionFactory::Unregister();
            Actions::ActionIncrementFactory::Unregister();
            Actions::ActionIfFactory::Unregister();
            Actions::ActionEventFactory::Unregister();
            GameObjectFactory::Unregister();

            AttributedSignatureRegistry::UnregisterSignatures<AttributedEventArgs>();
            AttributedSignatureRegistry::UnregisterSignatures<AttributedReaction>();
            AttributedSignatureRegistry::UnregisterSignatures<Reaction>();
            AttributedSignatureRegistry::UnregisterSignatures<Actions::ActionIf>();
            AttributedSignatureRegistry::UnregisterSignatures<Actions::ActionList>();
            AttributedSignatureRegistry::UnregisterSignatures<Actions::ActionIncrement>();
            AttributedSignatureRegistry::UnregisterSignatures<Actions::ActionEvent>();
            AttributedSignatureRegistry::UnregisterSignatures<Action>();
            AttributedSignatureRegistry::UnregisterSignatures<GameObject>();
            AttributedSignatureRegistry::UnregisterSignatures<Transform>();
        }

        TEST_METHOD(ParseEvents) {
            auto root = std::make_shared<Scope>();
            auto wrapper = std::make_shared<ScopeParseWrapper>(root);
            auto coordinator = JsonParseCoordinator(wrapper);

            Assert::IsTrue(coordinator.PushBackHelper(std::make_unique<ScopeJsonParse::AllScopeJsonParseHelper>()));
            Assert::IsTrue(coordinator.PushBackTransmuter(std::make_unique<ScopeJsonParse::ScopeJsonKeyTokenTransmuter>()));

            coordinator.DeserializeIntoWrapperFromFile(R"(Files\TestAttributedReactionParse.json)"s);

            Assert::IsTrue(root->IsContainingKey("Scene"s));
            Assert::IsTrue(root->CAt("Scene"s).CFrontTable().Is(GameObject::TypeIdClass()));

            auto& scene = static_cast<GameObject&>(root->At("Scene"s).FrontTable());

            Assert::IsTrue(scene.IsContainingKey("Actions"s));
            Assert::AreEqual(size_type(2), scene.GetActions().Size());
            Assert::IsTrue(scene.GetActions().CGetTableElement(size_type(0)).Is(Actions::ActionIncrement::TypeIdClass()));

            const auto& updates = static_cast<const Actions::ActionIncrement&>(scene.GetActions().CGetTableElement(size_type(0)));

            Assert::IsTrue(scene.GetActions().CGetTableElement(size_type(1)).Is(Actions::ActionIf::TypeIdClass()));

            auto& generator = static_cast<Actions::ActionIf&>(scene.At("Actions"s).GetTableElement(size_type(1)));

            Assert::IsTrue(scene.IsContainingKey("Response"s));
            Assert::IsTrue(scene.CAt("Response"s).CFrontTable().Is(AttributedReaction::TypeIdClass()));

            const auto& response = static_cast<const AttributedReaction&>(scene.CAt("Response"s).CFrontTable());

            Assert::IsTrue(response.IsContainingKey("Actions"s));
            Assert::AreEqual(size_type(1), response.GetActions().Size());
            Assert::IsTrue(response.GetActions().CFrontTable().Is(Actions::ActionIncrement::TypeIdClass()));

            const auto& counter = static_cast<const Actions::ActionIncrement&>(response.GetActions().CFrontTable());

            Assert::AreEqual(0, updates.GetCurrent());
            Assert::AreNotEqual(0, generator.GetCondition());
            Assert::AreEqual(0, counter.GetCurrent());

            auto& gameplay = GameplayState::Singleton();
            gameplay.ProgressTime();
            auto& events = gameplay.Events();

            Assert::IsTrue(events.IsEmpty());

            scene.Update(gameplay.ProgressTime());

            Assert::AreEqual(1, updates.GetCurrent());
            Assert::AreEqual(0, counter.GetCurrent());
            Assert::IsFalse(events.IsEmpty());
            auto eventcount = events.Size();

            generator.SetCondition(0);
            scene.Update(gameplay.GetTime());

            Assert::AreEqual(eventcount, events.Size());
            Assert::AreEqual(0, counter.GetCurrent());

            while (!(events.IsEmpty())) {
                scene.Update(gameplay.ProgressTime());
            }

            Assert::AreEqual(15, counter.GetCurrent());
        }

        TEST_METHOD(AttributedEventArgsCreate) {
            using String = Datum::String;
            const String SUBTYPE = "Subtype"s;

            AttributedEventArgs args{SUBTYPE};

            Assert::AreEqual(SUBTYPE, args.GetSubtype());
            Assert::AreEqual(SUBTYPE, args.Subtype());
            Assert::AreEqual(SUBTYPE, args.CAt(SUBTYPE).CFrontString());

            auto factoryArgs = Factory<Scope>::StaticCreate(AttributedEventArgs::TypeNameClass());
        }

        TEST_METHOD(CopyCloneAndMove) {
            auto root = std::make_shared<Scope>();
            auto wrapper = std::make_shared<ScopeParseWrapper>(root);
            auto coordinator = JsonParseCoordinator(wrapper);

            Assert::IsTrue(coordinator.PushBackHelper(std::make_unique<ScopeJsonParse::AllScopeJsonParseHelper>()));
            Assert::IsTrue(coordinator.PushBackTransmuter(std::make_unique<ScopeJsonParse::ScopeJsonKeyTokenTransmuter>()));

            coordinator.DeserializeIntoWrapperFromFile(R"(Files\TestAttributedReactionParse.json)"s);

            Assert::IsTrue(root->IsContainingKey("Scene"s));
            Assert::IsTrue(root->CAt("Scene"s).CFrontTable().Is(GameObject::TypeIdClass()));

            auto& scene = static_cast<GameObject&>(root->At("Scene"s).FrontTable());

            Assert::IsTrue(scene.IsContainingKey("Response"s));
            Assert::IsTrue(scene.CAt("Response"s).CFrontTable().Is(AttributedReaction::TypeIdClass()));

            auto& response = static_cast<AttributedReaction&>(scene.At("Response"s).FrontTable());

            Assert::IsTrue(response.IsContainingKey("Actions"s));
            Assert::AreEqual(size_type(1), response.GetActions().Size());
            Assert::IsTrue(response.GetActions().CFrontTable().Is(Actions::ActionIncrement::TypeIdClass()));

            auto clone = response.Clone();

            Assert::IsTrue(clone->Is(AttributedReaction::TypeIdClass()));
            Assert::AreEqual(response.Size(), clone->Size());
            Assert::AreEqual(response.IsContainingKey("Actions"s), clone->IsContainingKey("Actions"s));
            Assert::AreEqual(response.GetActions().Size(), clone->CAt("Actions"s).Size());
            Assert::IsTrue((response.GetActions().CFrontTable()) == (clone->CAt("Actions"s).CFrontTable()));

            response.AppendAuxiliaryAttribute("Number"s) = 4;

            Assert::AreNotEqual(response.Size(), clone->Size());

            *clone = response;

            Assert::AreEqual(response.Size(), clone->Size());
            Assert::AreEqual(response.IsContainingKey("Actions"s), clone->IsContainingKey("Actions"s));
            Assert::AreEqual(response.GetActions().Size(), clone->CAt("Actions"s).Size());
            Assert::IsTrue((response.GetActions().CFrontTable()) == (clone->CAt("Actions"s).CFrontTable()));
            Assert::AreEqual(response.CAt("Number"s).CFrontInteger(), clone->CAt("Number"s).CFrontInteger());

            AttributedReaction moved = std::move(static_cast<AttributedReaction&>(*clone));

            Assert::AreEqual(response.Size(), moved.Size());
            Assert::AreEqual(response.IsContainingKey("Actions"s), moved.IsContainingKey("Actions"s));
            Assert::AreEqual(response.GetActions().Size(), moved.GetActions().Size());
            Assert::IsTrue((response.GetActions().CFrontTable()) == (moved.CAt("Actions"s).CFrontTable()));
            Assert::AreEqual(response.CAt("Number"s).CFrontInteger(), moved.CAt("Number"s).CFrontInteger());

            *clone = std::move(moved);

            Assert::AreEqual(response.Size(), clone->Size());
            Assert::AreEqual(response.IsContainingKey("Actions"s), clone->IsContainingKey("Actions"s));
            Assert::AreEqual(response.GetActions().Size(), clone->CAt("Actions"s).Size());
            Assert::IsTrue((response.GetActions().CFrontTable()) == (clone->CAt("Actions"s).CFrontTable()));
            Assert::AreEqual(response.CAt("Number"s).CFrontInteger(), clone->CAt("Number"s).CFrontInteger());
        }
    };
}