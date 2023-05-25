#include "pch.h"
#include "CppUnitTest.h"
#include "EventQueue.h"
#include "IntEventArgs.h"
#include "IntFloatEventArgs.h"
#include "HeapedIntEventArgs.h"
#include "OtherIntEventArgs.h"
#include "PointerIntEventArgs.h"
#include "SharedIntEventArgs.h"
#include "UniqueIntEventArgs.h"
#include "FooEventArgs.h"
#include "GameClock.h"
#include "GameTime.h"
#include "ToStringSpecializations.h"
#include <thread>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std::literals::string_literals;
using namespace std::chrono;
using namespace std::this_thread;

namespace LibraryDesktopTests {
    TEST_CLASS(EventQueueTests) {

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

        TEST_METHOD(InstantPublish) {
            int counter = 0;
            std::shared_ptr<EventSubscriber> subscriber = std::make_shared<EventSubscriber>([&counter](const IEventArgs& args) {
                const FooEventArgs* converted = args.As<FooEventArgs>();
                if (converted != nullptr) {
                    counter += (converted->_foo.Data() >= 0) ? 1 : -1;
                }
            });
            Event::Subscribe<FooEventArgs>(subscriber);
            EventQueue queue{};

            Assert::AreEqual(0, counter);

            GameClock clock{};
            queue.Enqueue(std::make_unique<FooEventArgs>(0), 0L, clock.StartTime());

            Assert::AreEqual(1, counter);

            queue.Enqueue(std::make_unique<OtherIntEventArgs>(0), 0L, clock.StartTime());

            Assert::AreEqual(1, counter);

            queue.Enqueue(std::make_unique<FooEventArgs>(-1), 0L, clock.StartTime());

            Assert::AreEqual(0, counter);
        }

        TEST_METHOD(Enqueue) {
            int totalCounter = 0;
            int intCounter = 0;
            int floatCounter = 0;
            int sharedCounter = 0;
            int uniqueCounter = 0;
            int heapedCounter = 0;

            std::shared_ptr<EventSubscriber> genericSubscriber = std::make_shared<EventSubscriber>([&totalCounter](const IEventArgs&){ ++totalCounter; });

            std::shared_ptr<EventSubscriber> intSubscriber = std::make_shared<EventSubscriber>([&intCounter](const IEventArgs& args){
                const IntEventArgs* converted = args.As<IntEventArgs>();
                if (converted != nullptr) {
                    ++intCounter;
                }
            });

            std::shared_ptr<EventSubscriber> floatSubscriber = std::make_shared<EventSubscriber>([&floatCounter](const IEventArgs& args){
                const IntFloatEventArgs* converted = args.As<IntFloatEventArgs>();
                if (converted != nullptr) {
                    ++floatCounter;
                }
            });

            std::shared_ptr<EventSubscriber> sharedSubscriber = std::make_shared<EventSubscriber>([&sharedCounter](const IEventArgs& args){
                const SharedIntEventArgs* converted = args.As<SharedIntEventArgs>();
                if (converted != nullptr) {
                    ++sharedCounter;
                }
            });

            std::shared_ptr<EventSubscriber> uniqueSubscriber = std::make_shared<EventSubscriber>([&uniqueCounter](const IEventArgs& args){
                const UniqueIntEventArgs* converted = args.As<UniqueIntEventArgs>();
                if (converted != nullptr) {
                    ++uniqueCounter;
                }
            });

            std::shared_ptr<EventSubscriber> heapedSubscriber = std::make_shared<EventSubscriber>([&heapedCounter](const IEventArgs& args){
                const HeapedIntEventArgs* converted = args.As<HeapedIntEventArgs>();
                if (converted != nullptr) {
                    ++heapedCounter;
                }
            });

            Event::Subscribe<IntEventArgs>(genericSubscriber);
            Event::Subscribe<OtherIntEventArgs>(genericSubscriber);
            Event::Subscribe<IntFloatEventArgs>(genericSubscriber);
            Event::Subscribe<SharedIntEventArgs>(genericSubscriber);
            Event::Subscribe<UniqueIntEventArgs>(genericSubscriber);
            Event::Subscribe<HeapedIntEventArgs>(genericSubscriber);
            Event::Subscribe<IntEventArgs>(intSubscriber);
            Event::Subscribe<IntFloatEventArgs>(floatSubscriber);
            Event::Subscribe<SharedIntEventArgs>(sharedSubscriber);
            Event::Subscribe<UniqueIntEventArgs>(uniqueSubscriber);
            Event::Subscribe<HeapedIntEventArgs>(heapedSubscriber);

            EventQueue queue{};
            GameClock clock{};
            queue.Enqueue<IntEventArgs>(10L, clock.StartTime(), 1);
            queue.Enqueue<OtherIntEventArgs>(50L, clock.StartTime(), 2);
            queue.Enqueue<IntFloatEventArgs>(20L, clock.StartTime(), 3.f, 3);
            queue.Enqueue<SharedIntEventArgs>(60L, clock.StartTime(), 4);
            queue.Enqueue<UniqueIntEventArgs>(40L, clock.StartTime(), 5);
            queue.Enqueue<HeapedIntEventArgs>(30L, clock.StartTime(), 6);

            Assert::IsFalse(queue.IsEmpty());

            EventQueue tmp = std::move(queue);
            EventQueue destination{};
            destination = std::move(tmp);

            auto size = destination.Size();

            Assert::AreEqual(decltype(size)(6), size);

            GameTime time{};
            clock.UpdateGameTime(time);

            while (!(destination.IsEmpty())) {
                time.SetElapsedGameTime(milliseconds(10L));
                time.SetTotalGameTime(time.TotalGameTime() + time.ElapsedGameTime());
                time.SetCurrentTime(time.CurrentTime() + time.ElapsedGameTime());
                destination.Update(time);
                if (size != destination.Size()) {
                    size = destination.Size();
                    switch (size) {

                    case 5:
                        Assert::AreEqual(1, totalCounter);
                        Assert::AreEqual(1, intCounter);
                        Assert::AreEqual(0, floatCounter);
                        Assert::AreEqual(0, sharedCounter);
                        Assert::AreEqual(0, uniqueCounter);
                        Assert::AreEqual(0, heapedCounter);
                        break;

                    case 4:
                        Assert::AreEqual(2, totalCounter);
                        Assert::AreEqual(1, intCounter);
                        Assert::AreEqual(1, floatCounter);
                        Assert::AreEqual(0, sharedCounter);
                        Assert::AreEqual(0, uniqueCounter);
                        Assert::AreEqual(0, heapedCounter);
                        break;

                    case 3:
                        Assert::AreEqual(3, totalCounter);
                        Assert::AreEqual(1, intCounter);
                        Assert::AreEqual(1, floatCounter);
                        Assert::AreEqual(0, sharedCounter);
                        Assert::AreEqual(0, uniqueCounter);
                        Assert::AreEqual(1, heapedCounter);
                        break;

                    case 2:
                        Assert::AreEqual(4, totalCounter);
                        Assert::AreEqual(1, intCounter);
                        Assert::AreEqual(1, floatCounter);
                        Assert::AreEqual(0, sharedCounter);
                        Assert::AreEqual(1, uniqueCounter);
                        Assert::AreEqual(1, heapedCounter);
                        break;

                    case 1:
                        Assert::AreEqual(5, totalCounter);
                        Assert::AreEqual(1, intCounter);
                        Assert::AreEqual(1, floatCounter);
                        Assert::AreEqual(0, sharedCounter);
                        Assert::AreEqual(1, uniqueCounter);
                        Assert::AreEqual(1, heapedCounter);
                        break;

                    default:
                        Assert::AreEqual(decltype(size)(0), size);

                    }
                }
            }

            time.SetElapsedGameTime(milliseconds(10L));
            time.SetTotalGameTime(time.TotalGameTime() + time.ElapsedGameTime());
            time.SetCurrentTime(time.CurrentTime() + time.ElapsedGameTime());
            destination.Update(time);
            Assert::AreEqual(6, totalCounter);
            Assert::AreEqual(1, intCounter);
            Assert::AreEqual(1, floatCounter);
            Assert::AreEqual(1, sharedCounter);
            Assert::AreEqual(1, uniqueCounter);
            Assert::AreEqual(1, heapedCounter);
        }

        TEST_METHOD(PendingOperations) {
            EventQueue queue{};
            int intCounter = 0;
            int floatCounter = 0;
            int otherCounter = 0;
            bool isCompleted = false;

            auto subscriber = std::make_shared<EventSubscriber>([&queue, &intCounter, &floatCounter, &isCompleted](const IEventArgs& args){
                if (args.Is(IntEventArgs::TypeIdClass())) {
                    ++intCounter;
                }

                const IntFloatEventArgs* converted = args.As<IntFloatEventArgs>();
                if (converted != nullptr) {
                    ++floatCounter;
                    auto now = high_resolution_clock::now();
                    if ((converted->_number) > 0.f) {
                        queue.Clear();
                        queue.Enqueue<IntFloatEventArgs>(50L, now, -10.f, 0);
                    } else if ((converted->_data) == 0) {
                        queue.Enqueue<IntEventArgs>(50L, now, 100);
                        queue.Enqueue<IntFloatEventArgs>(100L, now, -20.f, -30);
                        queue.Enqueue<OtherIntEventArgs>(150L, now, 100);
                        queue.Enqueue<IntEventArgs>(200L, now, 0);
                        queue.Enqueue<IntEventArgs>(250L, now, 200);
                        queue.Enqueue<IntEventArgs>(300L, now, 0);
                    } else if ((converted->_data) < 0) {
                        queue.RemoveEach([](const IEventArgs& args, const EventQueue::timepoint_type&){
                            const IntEventArgs* intargs = args.As<IntEventArgs>();
                            return (intargs == nullptr) || ((intargs->_data) < 1);
                        });
                        queue.Enqueue<IntFloatEventArgs>(50L, now, -30.f, 80);
                    } else {
                        isCompleted = true;
                    }

                    if (queue.IsUpdating()) {
                        queue.Update(GameTime{});
                    }
                }

            });

            auto otherSubscriber = std::make_shared<EventSubscriber>([&otherCounter](const IEventArgs& args){ if (args.Is(OtherIntEventArgs::TypeIdClass())) { ++otherCounter; } });

            Event::Subscribe<IntEventArgs>(subscriber);
            Event::Subscribe<IntFloatEventArgs>(subscriber);
            Event::Subscribe(OtherIntEventArgs::TypeIdClass(), otherSubscriber);

            GameClock clock{};

            queue.Enqueue<IntEventArgs>(10L, clock.StartTime(), 123);
            queue.Enqueue<IntFloatEventArgs>(20L, clock.StartTime(), 2.34f, 345);
            queue.Enqueue<IntFloatEventArgs>(30L, clock.StartTime(), 4.56f, 567);
            queue.Enqueue<OtherIntEventArgs>(35L, clock.StartTime(), 678);
            queue.Enqueue<IntEventArgs>(40L, clock.StartTime(), 789);
            queue.Enqueue<OtherIntEventArgs>(45L, clock.StartTime(), 890);

            GameTime time{};
            clock.UpdateGameTime(time);

            while (!(queue.IsEmpty())) {
                time.SetElapsedGameTime(milliseconds(10L));
                time.SetTotalGameTime(time.TotalGameTime() + time.ElapsedGameTime());
                time.SetCurrentTime(time.CurrentTime() + time.ElapsedGameTime());
                queue.Update(time);
            }

            Assert::AreEqual(0, otherCounter);
            Assert::AreEqual(7, intCounter);
            Assert::AreEqual(4, floatCounter);
            Assert::IsTrue(isCompleted);
        }
    };
}