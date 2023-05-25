#include "pch.h"
#include "CppUnitTest.h"
#include "JsonParseCoordinator.h"
#include "JsonIntTestParseHelper.h"
#include "JsonIntArrayTestParseHelper.h"
#include "JsonFloatTestParseHelper.h"
#include "JsonFloatArrayTestParseHelper.h"
#include "JsonStringTestParseHelper.h"
#include "JsonStringArrayTestParseHelper.h"
#include "JsonTypeTestParseHelper.h"
#include "NamedIntArrayTestParseWrapper.h"
#include "NamedFloatArrayTestParseWrapper.h"
#include "NamedStringArrayTestParseWrapper.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std::literals::string_literals;

namespace LibraryDesktopTests {
    TEST_CLASS(JsonParseCoordinatorTests) {

    private:
        inline static _CrtMemState _startMemState;

        using depth_type = ParseCoordinator::Wrapper::depth_type;
        using size_type = decltype(NamedIntArrayTestParseWrapper::Ints)::size_type;

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

        TEST_METHOD(ParseSingleInt) {
            auto wrapper = std::make_shared<NamedIntArrayTestParseWrapper>();
            auto coordinator = JsonParseCoordinator(wrapper);
            coordinator.PushBackHelper(std::make_unique<JsonIntTestParseHelper>());

            const JsonIntTestParseHelper* helper = coordinator.GetHelper(JsonIntTestParseHelper::TypeIdClass())->As<JsonIntTestParseHelper>();

            Assert::IsTrue(helper->Is(IJsonParseHelper::TypeIdClass()));
            Assert::IsNotNull(helper);
            Assert::IsTrue(helper->IsInitialized());
            Assert::AreEqual(0, helper->TryParseCallsCount());
            Assert::AreEqual(0, helper->EndParseCallsCount());

            coordinator.DeserializeIntoWrapperFromString(R"({ "integer": 5 })"s);

            Assert::AreEqual(1, helper->TryParseCallsCount());
            Assert::AreEqual(1, helper->EndParseCallsCount());
            Assert::AreEqual(depth_type(0), wrapper->Depth());
            Assert::AreEqual(depth_type(0), wrapper->DeepestDepth());
            Assert::AreEqual(0, wrapper->IncrementDepthCallsCount());
            Assert::AreEqual(0, wrapper->DecrementDepthCallsCount());
            Assert::AreEqual(size_type(1), wrapper->Ints.Size());
            Assert::AreEqual(size_type(1), wrapper->Ints.Capacity());
            Assert::AreEqual(5, wrapper->Ints[size_type(0)]);
            Assert::AreEqual("integer"s, wrapper->Name);
        }

        TEST_METHOD(ParseNestedStringArray) {
            auto wrapper = std::make_shared<NamedStringArrayTestParseWrapper>();
            auto coordinator = JsonParseCoordinator(wrapper);
            coordinator.PushBackHelper(std::make_unique<JsonStringArrayTestParseHelper>());

            const JsonStringArrayTestParseHelper* helper = coordinator.GetHelper(JsonStringArrayTestParseHelper::TypeIdClass())
                ->As<JsonStringArrayTestParseHelper>();

            Assert::IsNotNull(helper);
            Assert::IsTrue(helper->IsInitialized());
            Assert::AreEqual(0, helper->TryParseCallsCount());
            Assert::AreEqual(0, helper->EndParseCallsCount());

            coordinator.DeserializeIntoWrapperFromString(R"({
  "Mage": {
    "Spells": [ "Magic Missile", "Fireball" ]
  }
})"s);

            Assert::AreEqual(2, helper->TryParseCallsCount());
            Assert::AreEqual(2, helper->EndParseCallsCount());
            Assert::AreEqual(depth_type(0), wrapper->Depth());
            Assert::AreEqual(depth_type(1), wrapper->DeepestDepth());
            Assert::AreEqual(1, wrapper->IncrementDepthCallsCount());
            Assert::AreEqual(1, wrapper->DecrementDepthCallsCount());
            Assert::AreEqual(size_type(2), wrapper->Strings.Size());
            Assert::IsTrue(size_type(1) < wrapper->Strings.Capacity());
            Assert::AreEqual("Magic Missile"s, wrapper->Strings[size_type(0)]);
            Assert::AreEqual("Fireball"s, wrapper->Strings[size_type(1)]);
            Assert::AreEqual("Spells"s, wrapper->Name);
        }

        TEST_METHOD(ParseSingleNestedFloatWithType) {
            auto wrapper = std::make_shared<NamedFloatArrayTestParseWrapper>();
            auto coordinator = JsonParseCoordinator(wrapper);

            coordinator.PushBackHelper(std::make_unique<JsonIntTestParseHelper>());
            const JsonIntTestParseHelper* lazyhelper = coordinator.GetHelper(JsonIntTestParseHelper::TypeIdClass())
                ->As<JsonIntTestParseHelper>();

            Assert::IsNotNull(lazyhelper);
            Assert::IsTrue(lazyhelper->IsInitialized());
            Assert::AreEqual(0, lazyhelper->TryParseCallsCount());
            Assert::AreEqual(0, lazyhelper->EndParseCallsCount());

            coordinator.PushBackHelper(std::make_unique<JsonFloatTestParseHelper>());
            const JsonFloatTestParseHelper* valhelper = coordinator.GetHelper(JsonFloatTestParseHelper::TypeIdClass())
                ->As<JsonFloatTestParseHelper>();

            Assert::IsNotNull(valhelper);
            Assert::IsTrue(valhelper->IsInitialized());
            Assert::AreEqual(0, valhelper->TryParseCallsCount());
            Assert::AreEqual(0, valhelper->EndParseCallsCount());

            coordinator.PushBackHelper(std::make_unique<JsonTypeTestParseHelper>());
            const JsonTypeTestParseHelper* typehelper = coordinator.GetHelper(JsonTypeTestParseHelper::TypeIdClass())
                ->As<JsonTypeTestParseHelper>();

            Assert::IsNotNull(typehelper);
            Assert::IsTrue(typehelper->IsInitialized());
            Assert::AreEqual(0, typehelper->TryParseCallsCount());
            Assert::AreEqual(0, typehelper->EndParseCallsCount());

            coordinator.DeserializeIntoWrapperFromString(R"({
  "Character": {
    "Attribute": {
      "type": "float",
      "number": 42.24
    }
  }
})"s);

            Assert::AreEqual(2, lazyhelper->TryParseCallsCount());
            Assert::AreEqual(0, lazyhelper->EndParseCallsCount());
            Assert::AreEqual(2, valhelper->TryParseCallsCount());
            Assert::AreEqual(1, valhelper->EndParseCallsCount());
            Assert::AreEqual(1, typehelper->TryParseCallsCount());
            Assert::AreEqual(1, typehelper->EndParseCallsCount());
            Assert::AreEqual(depth_type(0), wrapper->Depth());
            Assert::AreEqual(depth_type(2), wrapper->DeepestDepth());
            Assert::AreEqual(2, wrapper->IncrementDepthCallsCount());
            Assert::AreEqual(2, wrapper->DecrementDepthCallsCount());
            Assert::AreEqual(size_type(1), wrapper->Floats.Size());
            Assert::AreEqual(size_type(1), wrapper->Floats.Capacity());
            Assert::IsTrue(0.001f > std::abs(42.24f - wrapper->Floats[size_type(0)]));
            Assert::AreEqual("number"s, wrapper->Name);
        }

        TEST_METHOD(CannotSetNullWrapper) {
            Assert::ExpectException<std::invalid_argument>([](){ JsonParseCoordinator nulled{nullptr}; UNREFERENCED_LOCAL(nulled); });

            auto strapper = std::make_shared<NamedStringArrayTestParseWrapper>();
            auto flapper = std::make_shared<NamedFloatArrayTestParseWrapper>();

            JsonParseCoordinator coordinator{strapper};

            Assert::IsTrue(strapper.get() == coordinator.GetWrapper().get());

            Assert::ExpectException<std::invalid_argument>([&coordinator](){ coordinator.SetWrapper(nullptr); });

            coordinator.SetWrapper(flapper);

            Assert::IsFalse(strapper.get() == coordinator.GetWrapper().get());
            Assert::IsTrue(flapper.get() == coordinator.GetWrapper().get());
        }

        TEST_METHOD(CannotDecrementWrapperDepthBelowZero) {
            NamedStringArrayTestParseWrapper wrapper{};
            Assert::IsFalse(wrapper.DecrementDepth());
            Assert::AreEqual(1, wrapper.DecrementDepthCallsCount());
        }

        TEST_METHOD(CannotDeserializeNonobjectRoot) {
            auto wrapper = std::make_shared<NamedStringArrayTestParseWrapper>();
            JsonParseCoordinator coordinator{wrapper};
            coordinator.PushBackHelper(std::make_unique<JsonStringTestParseHelper>());

            Assert::ExpectException<std::runtime_error>([&coordinator](){
                coordinator.DeserializeIntoWrapperFromString(R"("hello": "world")"s);
            });
        }

        TEST_METHOD(MoveSemanticsAndClone) {
            auto strapper = std::make_shared<NamedStringArrayTestParseWrapper>();
            auto flapper = std::make_shared<NamedFloatArrayTestParseWrapper>();

            JsonParseCoordinator source{strapper};
            source.PushBackHelper(std::make_unique<JsonIntTestParseHelper>());
            source.PushBackHelper(std::make_unique<JsonIntArrayTestParseHelper>());
            source.PushBackHelper(std::make_unique<JsonStringTestParseHelper>());
            source.PushBackHelper(std::make_unique<JsonStringArrayTestParseHelper>());
            source.PushBackHelper(std::make_unique<JsonTypeTestParseHelper>());

            auto clone = source.Clone();
            JsonParseCoordinator destination{std::move(source)};

            Assert::AreEqual(clone->GetWrapper()->TypeIdInstance(), destination.GetWrapper()->TypeIdInstance());
            Assert::IsFalse(destination.PushBackHelper(std::make_unique<JsonIntTestParseHelper>()));
            Assert::IsFalse(destination.PushBackHelper(std::make_unique<JsonIntArrayTestParseHelper>()));
            Assert::IsFalse(destination.PushBackHelper(std::make_unique<JsonStringTestParseHelper>()));
            Assert::IsFalse(destination.PushBackHelper(std::make_unique<JsonStringArrayTestParseHelper>()));
            Assert::IsFalse(destination.PushBackHelper(std::make_unique<JsonTypeTestParseHelper>()));
            Assert::IsFalse(static_cast<JsonParseCoordinator*>(clone.get())->RemoveHelper(JsonFloatTestParseHelper::TypeIdClass()));
            Assert::IsFalse(destination.RemoveHelper(JsonFloatTestParseHelper::TypeIdClass()));
            Assert::IsNull(static_cast<JsonParseCoordinator*>(clone.get())->GetHelper(JsonFloatArrayTestParseHelper::TypeIdClass()));
            Assert::IsNull(destination.GetHelper(JsonFloatArrayTestParseHelper::TypeIdClass()));

            Assert::IsTrue(destination.RemoveHelper(JsonIntTestParseHelper::TypeIdClass()));
            Assert::IsTrue(destination.RemoveHelper(JsonStringTestParseHelper::TypeIdClass()));

            clone = destination.Clone();
            source = std::move(destination);

            Assert::AreEqual(clone->GetWrapper()->TypeIdInstance(), source.GetWrapper()->TypeIdInstance());
            Assert::IsFalse(source.PushBackHelper(std::make_unique<JsonIntArrayTestParseHelper>()));
            Assert::IsFalse(source.PushBackHelper(std::make_unique<JsonStringArrayTestParseHelper>()));
            Assert::IsFalse(source.PushBackHelper(std::make_unique<JsonTypeTestParseHelper>()));
            Assert::IsFalse(static_cast<JsonParseCoordinator*>(clone.get())->RemoveHelper(JsonIntTestParseHelper::TypeIdClass()));
            Assert::IsFalse(source.RemoveHelper(JsonIntTestParseHelper::TypeIdClass()));
            Assert::IsNull(static_cast<JsonParseCoordinator*>(clone.get())->GetHelper(JsonStringTestParseHelper::TypeIdClass()));
            Assert::IsNull(source.GetHelper(JsonStringTestParseHelper::TypeIdClass()));
        }

        TEST_METHOD(DeserializeNullFails) {
            auto wrapper = std::make_shared<NamedIntArrayTestParseWrapper>();
            JsonParseCoordinator coordinator{wrapper};
            coordinator.PushBackHelper(std::make_unique<JsonIntTestParseHelper>());

            Assert::ExpectException<std::invalid_argument>([&coordinator](){
                coordinator.DeserializeIntoWrapper(nullptr);
            });
        }

        TEST_METHOD(DeserializeMalformedJsonFails) {
            auto wrapper = std::make_shared<NamedIntArrayTestParseWrapper>();
            JsonParseCoordinator coordinator{wrapper};
            coordinator.PushBackHelper(std::make_unique<JsonIntTestParseHelper>());

            Assert::ExpectException<std::invalid_argument>([&coordinator](){
                coordinator.DeserializeIntoWrapperFromString(R"({)"s);
            });
        }

        TEST_METHOD(NoHandlerForArrayElement) {
            auto wrapper = std::make_shared<NamedFloatArrayTestParseWrapper>();
            JsonParseCoordinator coordinator{wrapper};
            coordinator.PushBackHelper(std::make_unique<JsonIntArrayTestParseHelper>());
            coordinator.PushBackHelper(std::make_unique<JsonIntTestParseHelper>());
            coordinator.PushBackHelper(std::make_unique<JsonFloatArrayTestParseHelper>());
            coordinator.PushBackHelper(std::make_unique<JsonFloatTestParseHelper>());
            coordinator.PushBackHelper(std::make_unique<JsonTypeTestParseHelper>());

            const JsonIntArrayTestParseHelper* iahelper = coordinator.GetHelper(JsonIntArrayTestParseHelper::TypeIdClass())
                ->As<JsonIntArrayTestParseHelper>();
            const JsonIntTestParseHelper* ichelper = coordinator.GetHelper(JsonIntTestParseHelper::TypeIdClass())
                ->As<JsonIntTestParseHelper>();
            const JsonFloatArrayTestParseHelper* fahelper = coordinator.GetHelper(JsonFloatArrayTestParseHelper::TypeIdClass())
                ->As<JsonFloatArrayTestParseHelper>();
            const JsonFloatTestParseHelper* fchelper = coordinator.GetHelper(JsonFloatTestParseHelper::TypeIdClass())
                ->As<JsonFloatTestParseHelper>();

            Assert::AreEqual(0, iahelper->EndParseCallsCount());
            Assert::AreEqual(0, ichelper->EndParseCallsCount());
            Assert::AreEqual(0, fahelper->EndParseCallsCount());
            Assert::AreEqual(0, fchelper->EndParseCallsCount());

            Assert::ExpectException<std::runtime_error>([&coordinator](){
                coordinator.DeserializeIntoWrapperFromString(R"({
  "Travel Plan": {
    "type": "float",
    "Destinations": [ "Jacksonville", "Los Angeles" ]
  }
})"s);
            });

            Assert::AreEqual(0, iahelper->EndParseCallsCount());
            Assert::AreEqual(0, ichelper->EndParseCallsCount());
            Assert::AreEqual(0, fahelper->EndParseCallsCount());
            Assert::AreEqual(0, fchelper->EndParseCallsCount());
        }

        TEST_METHOD(ParseNestedFloatArrayWithTypeFromFile) {
            auto wrapper = std::make_shared<NamedFloatArrayTestParseWrapper>();
            auto coordinator = JsonParseCoordinator(wrapper);

            coordinator.PushBackHelper(std::make_unique<JsonFloatArrayTestParseHelper>());
            const JsonFloatArrayTestParseHelper* flohelper = coordinator.GetHelper(JsonFloatArrayTestParseHelper::TypeIdClass())
                ->As<JsonFloatArrayTestParseHelper>();

            Assert::IsNotNull(flohelper);
            Assert::IsTrue(flohelper->IsInitialized());
            Assert::AreEqual(0, flohelper->TryParseCallsCount());
            Assert::AreEqual(0, flohelper->EndParseCallsCount());

            coordinator.PushBackHelper(std::make_unique<JsonTypeTestParseHelper>());
            const JsonTypeTestParseHelper* typehelper = coordinator.GetHelper(JsonTypeTestParseHelper::TypeIdClass())
                ->As<JsonTypeTestParseHelper>();

            Assert::IsNotNull(typehelper);
            Assert::IsTrue(typehelper->IsInitialized());
            Assert::AreEqual(0, typehelper->TryParseCallsCount());
            Assert::AreEqual(0, typehelper->EndParseCallsCount());

            coordinator.DeserializeIntoWrapperFromFile(R"(Files\TestParse.json)"s);

            Assert::AreEqual(8, flohelper->TryParseCallsCount());
            Assert::AreEqual(7, flohelper->EndParseCallsCount());
            Assert::AreEqual(1, typehelper->TryParseCallsCount());
            Assert::AreEqual(1, typehelper->EndParseCallsCount());
            Assert::AreEqual(depth_type(0), wrapper->Depth());
            Assert::AreEqual(depth_type(2), wrapper->DeepestDepth());
            Assert::AreEqual(2, wrapper->IncrementDepthCallsCount());
            Assert::AreEqual(2, wrapper->DecrementDepthCallsCount());
            Assert::AreEqual(size_type(7), wrapper->Floats.Size());
            Assert::IsTrue(size_type(1) < wrapper->Floats.Capacity());
            Assert::IsTrue(0.001f > std::abs(100.f - wrapper->Floats[size_type(0)]));
            Assert::IsTrue(0.001f > std::abs(95.5f - wrapper->Floats[size_type(1)]));
            Assert::IsTrue(0.001f > std::abs(12.34f - wrapper->Floats[size_type(2)]));
            Assert::IsTrue(0.001f > std::abs(23.45f - wrapper->Floats[size_type(3)]));
            Assert::IsTrue(0.001f > std::abs(60.f - wrapper->Floats[size_type(4)]));
            Assert::IsTrue(0.001f > std::abs(0.89f - wrapper->Floats[size_type(5)]));
            Assert::IsTrue(0.001f > std::abs(7.f - wrapper->Floats[size_type(6)]));
            Assert::AreEqual("Individuals"s, wrapper->Name);
        }

        TEST_METHOD(NoHandlerForObjectElement) {
            auto wrapper = std::make_shared<NamedFloatArrayTestParseWrapper>();
            JsonParseCoordinator coordinator{wrapper};
            coordinator.PushBackHelper(std::make_unique<JsonIntArrayTestParseHelper>());
            coordinator.PushBackHelper(std::make_unique<JsonIntTestParseHelper>());
            coordinator.PushBackHelper(std::make_unique<JsonFloatArrayTestParseHelper>());
            coordinator.PushBackHelper(std::make_unique<JsonFloatTestParseHelper>());
            coordinator.PushBackHelper(std::make_unique<JsonTypeTestParseHelper>());

            const JsonIntArrayTestParseHelper* iahelper = coordinator.GetHelper(JsonIntArrayTestParseHelper::TypeIdClass())
                ->As<JsonIntArrayTestParseHelper>();
            const JsonIntTestParseHelper* ichelper = coordinator.GetHelper(JsonIntTestParseHelper::TypeIdClass())
                ->As<JsonIntTestParseHelper>();
            const JsonFloatArrayTestParseHelper* fahelper = coordinator.GetHelper(JsonFloatArrayTestParseHelper::TypeIdClass())
                ->As<JsonFloatArrayTestParseHelper>();
            const JsonFloatTestParseHelper* fchelper = coordinator.GetHelper(JsonFloatTestParseHelper::TypeIdClass())
                ->As<JsonFloatTestParseHelper>();

            Assert::AreEqual(0, iahelper->EndParseCallsCount());
            Assert::AreEqual(0, ichelper->EndParseCallsCount());
            Assert::AreEqual(0, fahelper->EndParseCallsCount());
            Assert::AreEqual(0, fchelper->EndParseCallsCount());

            Assert::ExpectException<std::runtime_error>([&coordinator](){
                coordinator.DeserializeIntoWrapperFromString(R"({
  "Travel Plan": {
    "type": "float",
    "Destination": "somewhere"
  }
})"s);
                });

            Assert::AreEqual(0, iahelper->EndParseCallsCount());
            Assert::AreEqual(0, ichelper->EndParseCallsCount());
            Assert::AreEqual(0, fahelper->EndParseCallsCount());
            Assert::AreEqual(0, fchelper->EndParseCallsCount());
        }
    };
}