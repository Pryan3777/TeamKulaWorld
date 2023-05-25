#include "pch.h"
#include "CppUnitTest.h"
#include "EventQueue.h"
#include "IntEventArgs.h"
#include "PointerIntEventArgs.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std::literals::string_literals;

namespace LibraryDesktopTests {
    TEST_CLASS(EventSubscriptionTests) {

    private:
        inline static _CrtMemState _startMemState;

    public:
        TEST_METHOD_INITIALIZE(Initialize) {
            Event::CreateSubscriptionsSingleton();
    #if defined(DEBUG) || defined(_DEBUG)
            _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
            _CrtMemCheckpoint(&_startMemState);
    #endif
        }

        TEST_METHOD_CLEANUP(Cleanup) {
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
        }

        TEST_METHOD(SubscriptionEdgeCases) {
            Assert::IsFalse(Event::CreateSubscriptionsSingleton());

            auto subscriber = std::make_shared<EventSubscriber>();
            Event::DestroySubscriptionsSingleton();

            Assert::ExpectException<std::logic_error>([&subscriber](){ Event::Subscribe<IntEventArgs>(subscriber); });
            Assert::ExpectException<std::logic_error>([&subscriber](){ Event::Unsubscribe<IntEventArgs>(subscriber); });
            Assert::ExpectException<std::logic_error>([&subscriber](){ Event::ClearSubscriptions<IntEventArgs>(); });
            Assert::ExpectException<std::logic_error>([&subscriber](){ Event::Subscribe(IntEventArgs::TypeIdClass(), subscriber); });
            Assert::ExpectException<std::logic_error>([&subscriber](){ Event::Unsubscribe(IntEventArgs::TypeIdClass(), subscriber); });
            Assert::ExpectException<std::logic_error>([&subscriber](){ Event::ClearSubscriptions(IntEventArgs::TypeIdClass()); });

            Assert::IsTrue(Event::CreateSubscriptionsSingleton());

            Assert::IsFalse(Event::Subscribe<IntEventArgs>(std::shared_ptr<EventSubscriber>{}));
            Assert::IsFalse(Event::Unsubscribe<IntEventArgs>(std::shared_ptr<EventSubscriber>{}));

            Event::ClearSubscriptions<PointerIntEventArgs>();

            Assert::IsFalse(Event::Unsubscribe<PointerIntEventArgs>(subscriber));

            Event ev{std::make_unique<IntEventArgs>(5)};

            Event::DestroySubscriptionsSingleton();

            Assert::ExpectException<std::logic_error>([&ev](){ ev.Publish(); });
        }

        TEST_METHOD(UnsubscribeAndClear) {
            int counter = 0;
            auto subscriber = std::make_shared<EventSubscriber>([&counter](const IEventArgs&){ ++counter; });
            Event::Subscribe<IntEventArgs>(subscriber);
            Event ev{std::make_unique<IntEventArgs>(5)};

            Assert::AreEqual(0, counter);

            ev.Publish();

            Assert::AreEqual(1, counter);

            Event::Unsubscribe<IntEventArgs>(subscriber);

            counter = 0;
            Event copy = ev;
            copy.Publish();

            Assert::AreEqual(0, counter);

            auto secondSubscriber = std::make_shared<EventSubscriber>([&counter](const IEventArgs&){ ++counter; });
            Event::Subscribe<IntEventArgs>(subscriber);
            Event::Subscribe<IntEventArgs>(secondSubscriber);

            Assert::AreEqual(0, counter);

            Event other{std::make_unique<PointerIntEventArgs>(&counter)};
            other = copy;
            other.Publish();

            Assert::AreEqual(2, counter);

            secondSubscriber.reset();
            counter = 0;
            copy.Publish();

            Assert::AreEqual(1, counter);

            Event::ClearSubscriptions<IntEventArgs>();

            counter = 0;
            ev.Publish();

            Assert::AreEqual(0, counter);
        }
    };
}