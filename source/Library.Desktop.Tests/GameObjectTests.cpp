#include "pch.h"
#include "CppUnitTest.h"
#include "JsonParseCoordinator.h"
#include "ScopeParseWrapper.h"
#include "AllScopeJsonParseHelper.h"
#include "ScopeJsonKeyTokenTransmuter.h"
#include "ToStringSpecializations.h"
#include "AttributedSignatureRegistry.h"
#include "TestGameObject.h"
#include "NoOpJsonValueTransmuter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace FieaGameEngine::ScopeJsonParse;
using namespace std::literals::string_literals;

namespace LibraryDesktopTests {
    inline bool FloatsAreEquivalent(float lhs, float rhs) {
        return 0.001f > std::abs(lhs - rhs);
    }

    TEST_CLASS(GameObjectTests) {

    private:
        inline static _CrtMemState _startMemState;

        using depth_type = ParseCoordinator::Wrapper::depth_type;
        using size_type = Scope::size_type;
        using vec4 = Transform::vec4;

    public:
        TEST_METHOD_INITIALIZE(Initialize) {
            AttributedSignatureRegistry::RegisterSignatures<Transform>();
            AttributedSignatureRegistry::RegisterSignatures<GameObject>();
            AttributedSignatureRegistry::RegisterSignatures<TestGameObject, GameObject>();
            ScopeFactory::Register();
            TransformFactory::Register();
            GameObjectFactory::Register();
            TestGameObjectFactory::Register();

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

            TestGameObjectFactory::Unregister();
            GameObjectFactory::Unregister();
            TransformFactory::Unregister();
            ScopeFactory::Unregister();
            AttributedSignatureRegistry::UnregisterSignatures<TestGameObject>();
            AttributedSignatureRegistry::UnregisterSignatures<GameObject>();
            AttributedSignatureRegistry::UnregisterSignatures<Transform>();
        }

        TEST_METHOD(Update) {
            auto root = std::make_shared<Scope>();
            auto wrapper = std::make_shared<ScopeParseWrapper>(root);
            auto coordinator = JsonParseCoordinator(wrapper);

            Assert::IsTrue(coordinator.PushBackHelper(std::make_unique<AllScopeJsonParseHelper>()));
            Assert::IsTrue(coordinator.PushBackTransmuter(std::make_unique<NoOpJsonValueTransmuter>()));
            Assert::IsTrue(coordinator.PushBackTransmuter(std::make_unique<ScopeJsonKeyTokenTransmuter>()));
            Assert::IsFalse(coordinator.PushBackTransmuter(std::make_unique<NoOpJsonValueTransmuter>()));

            coordinator.DeserializeIntoWrapperFromFile(R"(Files\TestGameObjectParse.json)"s);

            Assert::IsTrue(root->IsContainingKey("Hierarchy"s));

            auto* hierarchy = root->At("Hierarchy"s).FrontTable().As<TestGameObject>();

            Assert::IsNotNull(hierarchy);
            Assert::AreEqual("My Super Awesome Root GameObject"s, hierarchy->GetName());
            Assert::IsTrue(FloatsAreEquivalent(1.f, hierarchy->GetTransform().GetLocalPosition()[0]));
            Assert::IsTrue(FloatsAreEquivalent(2.1f, hierarchy->GetTransform().GetLocalPosition()[1]));
            Assert::IsTrue(FloatsAreEquivalent(3.2f, hierarchy->GetTransform().GetLocalPosition()[2]));
            Assert::IsTrue(FloatsAreEquivalent(5.3f, hierarchy->GetTransform().GetLocalPosition()[3]));
            Assert::IsTrue(FloatsAreEquivalent(8.5f, hierarchy->GetTransform().GetLocalRotation()[0]));
            Assert::IsTrue(FloatsAreEquivalent(13.8f, hierarchy->GetTransform().GetLocalRotation()[1]));
            Assert::IsTrue(FloatsAreEquivalent(21.13f, hierarchy->GetTransform().GetLocalRotation()[2]));
            Assert::IsTrue(FloatsAreEquivalent(34.21f, hierarchy->GetTransform().GetLocalRotation()[3]));
            Assert::IsTrue(FloatsAreEquivalent(1.f, hierarchy->GetTransform().GetLocalScale()[0]));
            Assert::IsTrue(FloatsAreEquivalent(1.f, hierarchy->GetTransform().GetLocalScale()[1]));
            Assert::IsTrue(FloatsAreEquivalent(1.f, hierarchy->GetTransform().GetLocalScale()[2]));
            Assert::IsTrue(FloatsAreEquivalent(0.f, hierarchy->GetTransform().GetLocalScale()[3]));
            Assert::AreEqual(size_type(3), hierarchy->GetChildren().Size());

            auto* firstChild = hierarchy->PublicChildren().FrontTable().As<TestGameObject>();

            Assert::IsNotNull(firstChild);
            Assert::AreEqual("The first child"s, firstChild->GetName());
            Assert::IsTrue(FloatsAreEquivalent(0.f, firstChild->GetTransform().GetLocalPosition()[0]));
            Assert::IsTrue(FloatsAreEquivalent(0.f, firstChild->GetTransform().GetLocalPosition()[1]));
            Assert::IsTrue(FloatsAreEquivalent(0.f, firstChild->GetTransform().GetLocalPosition()[2]));
            Assert::IsTrue(FloatsAreEquivalent(0.f, firstChild->GetTransform().GetLocalPosition()[3]));
            Assert::IsTrue(FloatsAreEquivalent(0.f, firstChild->GetTransform().GetLocalRotation()[0]));
            Assert::IsTrue(FloatsAreEquivalent(0.f, firstChild->GetTransform().GetLocalRotation()[1]));
            Assert::IsTrue(FloatsAreEquivalent(0.f, firstChild->GetTransform().GetLocalRotation()[2]));
            Assert::IsTrue(FloatsAreEquivalent(0.f, firstChild->GetTransform().GetLocalRotation()[3]));
            Assert::IsTrue(FloatsAreEquivalent(1.f, firstChild->GetTransform().GetLocalScale()[0]));
            Assert::IsTrue(FloatsAreEquivalent(1.f, firstChild->GetTransform().GetLocalScale()[1]));
            Assert::IsTrue(FloatsAreEquivalent(1.f, firstChild->GetTransform().GetLocalScale()[2]));
            Assert::IsTrue(FloatsAreEquivalent(0.f, firstChild->GetTransform().GetLocalScale()[3]));
            Assert::AreEqual(size_type(1), firstChild->GetChildren().Size());

            auto* pure = firstChild->PublicChildren().FrontTable().As<GameObject>();

            Assert::IsNotNull(pure);
            Assert::IsFalse(pure->Is(TestGameObject::TypeIdClass()));
            Assert::AreEqual("The only pure GameObject"s, pure->GetName());
            Assert::IsTrue(FloatsAreEquivalent(0.f, pure->GetTransform().GetLocalPosition()[0]));
            Assert::IsTrue(FloatsAreEquivalent(0.f, pure->GetTransform().GetLocalPosition()[1]));
            Assert::IsTrue(FloatsAreEquivalent(0.f, pure->GetTransform().GetLocalPosition()[2]));
            Assert::IsTrue(FloatsAreEquivalent(0.f, pure->GetTransform().GetLocalPosition()[3]));
            Assert::IsTrue(FloatsAreEquivalent(0.f, pure->GetTransform().GetLocalRotation()[0]));
            Assert::IsTrue(FloatsAreEquivalent(0.f, pure->GetTransform().GetLocalRotation()[1]));
            Assert::IsTrue(FloatsAreEquivalent(0.f, pure->GetTransform().GetLocalRotation()[2]));
            Assert::IsTrue(FloatsAreEquivalent(0.f, pure->GetTransform().GetLocalRotation()[3]));
            Assert::IsTrue(FloatsAreEquivalent(1.f, pure->GetTransform().GetLocalScale()[0]));
            Assert::IsTrue(FloatsAreEquivalent(1.f, pure->GetTransform().GetLocalScale()[1]));
            Assert::IsTrue(FloatsAreEquivalent(1.f, pure->GetTransform().GetLocalScale()[2]));
            Assert::IsTrue(FloatsAreEquivalent(0.f, pure->GetTransform().GetLocalScale()[3]));
            Assert::AreEqual(size_type(0), pure->GetChildren().Size());

            auto* secondChild = hierarchy->PublicChildren().GetTableElement(size_type(1)).As<TestGameObject>();

            Assert::IsNotNull(secondChild);
            Assert::AreEqual("Test"s, secondChild->GetName());
            Assert::IsTrue(FloatsAreEquivalent(0.1f, secondChild->GetTransform().GetLocalPosition()[0]));
            Assert::IsTrue(FloatsAreEquivalent(1.f, secondChild->GetTransform().GetLocalPosition()[1]));
            Assert::IsTrue(FloatsAreEquivalent(0.2f, secondChild->GetTransform().GetLocalPosition()[2]));
            Assert::IsTrue(FloatsAreEquivalent(2.1f, secondChild->GetTransform().GetLocalPosition()[3]));
            Assert::IsTrue(FloatsAreEquivalent(1.f, secondChild->GetTransform().GetLocalRotation()[0]));
            Assert::IsTrue(FloatsAreEquivalent(0.3f, secondChild->GetTransform().GetLocalRotation()[1]));
            Assert::IsTrue(FloatsAreEquivalent(3.2f, secondChild->GetTransform().GetLocalRotation()[2]));
            Assert::IsTrue(FloatsAreEquivalent(2.1f, secondChild->GetTransform().GetLocalRotation()[3]));
            Assert::IsTrue(FloatsAreEquivalent(1.f, secondChild->GetTransform().GetLocalScale()[0]));
            Assert::IsTrue(FloatsAreEquivalent(0.4f, secondChild->GetTransform().GetLocalScale()[1]));
            Assert::IsTrue(FloatsAreEquivalent(4.3f, secondChild->GetTransform().GetLocalScale()[2]));
            Assert::IsTrue(FloatsAreEquivalent(3.2f, secondChild->GetTransform().GetLocalScale()[3]));
            Assert::AreEqual(size_type(0), secondChild->GetChildren().Size());

            auto* thirdChild = hierarchy->PublicChildren().BackTable().As<TestGameObject>();

            Assert::IsNotNull(thirdChild);
            Assert::AreEqual("Down"s, thirdChild->GetName());
            Assert::AreEqual(size_type(1), thirdChild->GetChildren().Size());

            auto* thirdGrandchild = thirdChild->PublicChildren().BackTable().As<TestGameObject>();

            Assert::IsNotNull(thirdGrandchild);
            Assert::AreEqual("Downer"s, thirdGrandchild->GetName());
            Assert::AreEqual(size_type(1), thirdGrandchild->GetChildren().Size());

            const auto* thirdGreatgrandchild = thirdGrandchild->GetChildren().CBackTable().As<TestGameObject>();

            Assert::IsNotNull(thirdGreatgrandchild);
            Assert::AreEqual("Downest"s, thirdGreatgrandchild->GetName());
            Assert::AreEqual(size_type(0), thirdGreatgrandchild->PublicChildren().Size());

            Assert::AreEqual(0, hierarchy->UpdateCount());
            Assert::AreEqual(0, firstChild->UpdateCount());
            Assert::AreEqual(0, secondChild->UpdateCount());
            Assert::AreEqual(0, thirdChild->UpdateCount());
            Assert::AreEqual(0, thirdGrandchild->UpdateCount());
            Assert::AreEqual(0, thirdGreatgrandchild->UpdateCount());

            hierarchy->Update(GameTime{});

            Assert::AreEqual(1, hierarchy->UpdateCount());
            Assert::AreEqual(1, firstChild->UpdateCount());
            Assert::AreEqual(1, secondChild->UpdateCount());
            Assert::AreEqual(1, thirdChild->UpdateCount());
            Assert::AreEqual(1, thirdGrandchild->UpdateCount());
            Assert::AreEqual(1, thirdGreatgrandchild->UpdateCount());

            secondChild->Update(GameTime{});

            Assert::AreEqual(1, hierarchy->UpdateCount());
            Assert::AreEqual(1, firstChild->UpdateCount());
            Assert::AreEqual(2, secondChild->UpdateCount());
            Assert::AreEqual(1, thirdChild->UpdateCount());
            Assert::AreEqual(1, thirdGrandchild->UpdateCount());
            Assert::AreEqual(1, thirdGreatgrandchild->UpdateCount());

            thirdChild->Update(GameTime{});

            Assert::AreEqual(1, hierarchy->UpdateCount());
            Assert::AreEqual(1, firstChild->UpdateCount());
            Assert::AreEqual(2, secondChild->UpdateCount());
            Assert::AreEqual(2, thirdChild->UpdateCount());
            Assert::AreEqual(2, thirdGrandchild->UpdateCount());
            Assert::AreEqual(2, thirdGreatgrandchild->UpdateCount());

            thirdGrandchild->Update(GameTime{});

            Assert::AreEqual(1, hierarchy->UpdateCount());
            Assert::AreEqual(1, firstChild->UpdateCount());
            Assert::AreEqual(2, secondChild->UpdateCount());
            Assert::AreEqual(2, thirdChild->UpdateCount());
            Assert::AreEqual(3, thirdGrandchild->UpdateCount());
            Assert::AreEqual(3, thirdGreatgrandchild->UpdateCount());
        }

        TEST_METHOD(ClearAndToString) {
            TestGameObject root{"Root"s};

            root.CreateChild("FirstChild"s);
            root.CreateChild("SecondChild"s);

            Assert::AreEqual("TestGameObject: { \"Name\": Root, number of children: 2, number of actions: 0 }"s, root.ToString());

            root.Clear();

            Assert::AreEqual("TestGameObject: { \"Name\": Root, number of children: 0, number of actions: 0 }"s, root.ToString());
        }

        TEST_METHOD(CopyAndMoveSemantics) {
            GameObject root{"Root"s};

            root.LocalTranslate(vec4{1.f, 1.f, 0.f, 0.f});

            Assert::AreNotEqual(vec4{}, root.GetTransform().GetLocalPosition());
            Assert::AreEqual(size_type(0), root.GetChildren().Size());

            root.CreateChild("Child"s);

            Assert::AreNotEqual(size_type(0), root.GetChildren().Size());

            GameObject copyAssigned{""s};
            GameObject moveAssigned{""s};

            Assert::AreNotEqual(root.GetName(), copyAssigned.GetName());
            Assert::IsFalse(root.GetTransform() == copyAssigned.GetTransform());
            Assert::AreNotEqual(root.GetChildren(), copyAssigned.GetChildren());
            Assert::AreNotEqual(root.GetName(), moveAssigned.GetName());
            Assert::IsFalse(root.GetTransform() == moveAssigned.GetTransform());
            Assert::AreNotEqual(root.GetChildren(), moveAssigned.GetChildren());

            GameObject tmp{root};
            Scope::ScopeUniquePointer clone = tmp.Clone();
            GameObject* cloneConstructed = clone->As<GameObject>();

            Assert::IsNotNull(cloneConstructed);
            Assert::AreEqual(root.GetName(), cloneConstructed->GetName());
            Assert::IsTrue(root.GetTransform() == cloneConstructed->GetTransform());
            Assert::AreEqual(root.GetChildren(), cloneConstructed->GetChildren());

            copyAssigned = tmp;
            moveAssigned = std::move(tmp);

            Assert::AreEqual(root.GetName(), copyAssigned.GetName());
            Assert::IsTrue(root.GetTransform() == copyAssigned.GetTransform());
            Assert::AreEqual(root.GetChildren(), copyAssigned.GetChildren());
            Assert::AreEqual(root.GetName(), moveAssigned.GetName());
            Assert::IsTrue(root.GetTransform() == moveAssigned.GetTransform());
            Assert::AreEqual(root.GetChildren(), moveAssigned.GetChildren());

            GameObject copyConstructed{copyAssigned};
            GameObject moveConstructed{std::move(moveAssigned)};

            Assert::AreEqual(root.GetName(), copyConstructed.GetName());
            Assert::IsTrue(root.GetTransform() == copyConstructed.GetTransform());
            Assert::AreEqual(root.GetChildren(), copyConstructed.GetChildren());
            Assert::AreEqual(root.GetName(), moveConstructed.GetName());
            Assert::IsTrue(root.GetTransform() == moveConstructed.GetTransform());
            Assert::AreEqual(root.GetChildren(), moveConstructed.GetChildren());
        }
    };
}