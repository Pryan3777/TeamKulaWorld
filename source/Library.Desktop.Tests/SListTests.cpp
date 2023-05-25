#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "SList.h"
#include "Stack.h"
#include "ToStringSpecializations.h"
#include <cstdint>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace LibraryDesktopTests {
    TEST_CLASS(SListTests) {

    private:
        inline static _CrtMemState _startMemState;

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

        TEST_METHOD(DefaultConstructor) {
            SList<Foo> list{};
            auto emptySize = list.Size();

            Assert::AreEqual(SList<Foo>::size_type(0), emptySize);
            Assert::IsTrue(list.IsEmpty());
            Assert::ExpectException<std::out_of_range>([&list](){ auto& _ = list.Front(); UNREFERENCED_LOCAL(_); });
            Assert::ExpectException<std::out_of_range>([&list](){ auto& _ = list.Back(); UNREFERENCED_LOCAL(_); });

            list.PopBack();
            list.PopFront();

            Assert::AreEqual(emptySize, list.Size());
        }

        TEST_METHOD(CopyConstructorAndAssignment) {
            const Foo ONE{1};
            const Foo TWO{2};
            const Foo THREE{3};
            const Foo FOUR{4};

            SList<Foo> source{ONE, TWO, THREE, FOUR};
            SList<Foo> destination{source};

            Assert::AreEqual(source.Size(), destination.Size());
            Assert::IsFalse(destination.IsEmpty());

            auto popped = destination.Front();
            destination.PopFront();

            Assert::AreEqual(ONE, popped);

            popped = destination.Front();
            destination.PopFront();

            Assert::AreEqual(TWO, popped);

            popped = destination.Front();
            destination.PopFront();

            Assert::AreEqual(THREE, popped);

            popped = destination.Front();
            destination.PopFront();

            Assert::AreEqual(FOUR, popped);

            destination = source;

            Assert::AreEqual(source.Size(), destination.Size());
            Assert::IsFalse(destination.IsEmpty());

            popped = destination.Back();
            destination.PopBack();

            Assert::AreEqual(FOUR, popped);

            popped = destination.Back();
            destination.PopBack();

            Assert::AreEqual(THREE, popped);

            popped = destination.Back();
            destination.PopBack();

            Assert::AreEqual(TWO, popped);

            popped = destination.Back();
            destination.PopBack();

            Assert::AreEqual(ONE, popped);

            SList<Foo> empty{};
            SList<Foo> emptyCopy{empty};

            Assert::AreEqual(SList<Foo>::size_type(0), emptyCopy.Size());
            Assert::IsTrue(emptyCopy.IsEmpty());
            Assert::ExpectException<std::out_of_range>([&emptyCopy](){ auto& _ = emptyCopy.Front(); UNREFERENCED_LOCAL(_); });
            Assert::ExpectException<std::out_of_range>([&emptyCopy](){ auto& _ = emptyCopy.Back(); UNREFERENCED_LOCAL(_); });

            destination = empty;

            Assert::AreEqual(SList<Foo>::size_type(0), destination.Size());
            Assert::IsTrue(destination.IsEmpty());
            Assert::ExpectException<std::out_of_range>([&destination](){ auto& _ = destination.Front(); UNREFERENCED_LOCAL(_); });
            Assert::ExpectException<std::out_of_range>([&destination](){ auto& _ = destination.Back(); UNREFERENCED_LOCAL(_); });

            SList<Foo> single{ONE};
            SList<Foo> singleCopy{ONE};
            destination = single;

            Assert::AreEqual(single.Front(), singleCopy.Front());
            Assert::AreEqual(single.Back(), singleCopy.Back());
            Assert::AreNotSame(single.Front(), singleCopy.Front());
            Assert::AreNotSame(single.Back(), singleCopy.Back());
            Assert::AreEqual(single.Front(), destination.Front());
            Assert::AreEqual(single.Back(), destination.Back());
            Assert::AreNotSame(single.Front(), destination.Front());
            Assert::AreNotSame(single.Back(), destination.Back());
        }

        TEST_METHOD(PushFront) {
            const Foo DATA{5};

            SList<Foo> list{};
            list.PushFront(DATA);

            Assert::AreEqual(SList<Foo>::size_type(1), list.Size());
            Assert::IsFalse(list.IsEmpty());
            Assert::AreEqual(DATA, list.Front());
            Assert::AreEqual(DATA, list.Back());
        }

        TEST_METHOD(PushBack) {
            const Foo DATA{6};

            SList<Foo> list{};
            list.PushBack(DATA);

            Assert::AreEqual(SList<Foo>::size_type(1), list.Size());
            Assert::IsFalse(list.IsEmpty());
            Assert::AreEqual(DATA, list.Front());
            Assert::AreEqual(DATA, list.Back());
        }

        TEST_METHOD(PushBackThenFront) {
            const Foo BACK{7};
            const Foo FRONT{8};

            SList<Foo> list{};
            list.PushBack(BACK);
            list.PushFront(FRONT);

            Assert::AreEqual(SList<Foo>::size_type(2), list.Size());
            Assert::IsFalse(list.IsEmpty());
            Assert::AreEqual(BACK, list.Back());
            Assert::AreNotEqual(FRONT, list.Back());
            Assert::AreEqual(FRONT, list.Front());
            Assert::AreNotEqual(BACK, list.Front());
        }

        TEST_METHOD(PushBackThenPop) {
            const Foo FIRST{9};
            const Foo SECOND{10};

            SList<Foo> list{};
            list.PushBack(FIRST);
            list.PushBack(SECOND);

            Assert::AreEqual(SList<Foo>::size_type(2), list.Size());
            Assert::IsFalse(list.IsEmpty());
            Assert::AreEqual(SECOND, list.Back());
            Assert::AreNotEqual(FIRST, list.Back());

            auto popped = list.Back();
            list.PopBack();

            Assert::AreEqual(SList<Foo>::size_type(1), list.Size());
            Assert::IsFalse(list.IsEmpty());
            Assert::AreEqual(SECOND, popped);
            Assert::AreNotEqual(FIRST, popped);
            Assert::AreEqual(FIRST, list.Back());
            Assert::AreNotEqual(SECOND, list.Back());

            popped = list.Back();
            list.PopBack();

            Assert::AreEqual(SList<Foo>::size_type(0), list.Size());
            Assert::IsTrue(list.IsEmpty());
            Assert::AreEqual(FIRST, popped);
            Assert::AreNotEqual(SECOND, popped);
        }

        TEST_METHOD(PushFrontThenPop) {
            const Foo FIRST{11};
            const Foo SECOND{12};

            Foo first = FIRST;
            Foo second = SECOND;

            SList<Foo> list{};
            list.PushFront(std::move(first));
            list.PushFront(std::move(second));

            Assert::AreEqual(SList<Foo>::size_type(2), list.Size());
            Assert::IsFalse(list.IsEmpty());
            Assert::AreEqual(SECOND, list.Front());
            Assert::AreNotEqual(FIRST, list.Front());

            auto popped = list.Front();
            list.PopFront();

            Assert::AreEqual(SList<Foo>::size_type(1), list.Size());
            Assert::IsFalse(list.IsEmpty());
            Assert::AreEqual(SECOND, popped);
            Assert::AreNotEqual(FIRST, popped);
            Assert::AreEqual(FIRST, list.Front());
            Assert::AreNotEqual(SECOND, list.Front());

            popped = list.Front();
            list.PopFront();

            Assert::AreEqual(SList<Foo>::size_type(0), list.Size());
            Assert::IsTrue(list.IsEmpty());
            Assert::AreEqual(FIRST, popped);
            Assert::AreNotEqual(SECOND, popped);
        }

        TEST_METHOD(Clear) {
            const Foo FIRST{16};
            const Foo SECOND{17};
            const Foo THIRD{18};

            Stack<Foo> list{FIRST, SECOND, THIRD};

            auto sizePreClear = list.Size();
            auto isEmptyPreClear = list.IsEmpty();

            list.Clear();

            auto sizePostClear = list.Size();
            auto isEmptyPostClear = list.IsEmpty();

            list.Clear();

            Assert::AreNotEqual(sizePreClear, list.Size());
            Assert::AreNotEqual(isEmptyPreClear, list.IsEmpty());
            Assert::AreEqual(sizePostClear, list.Size());
            Assert::AreEqual(isEmptyPostClear, list.IsEmpty());
            Assert::AreEqual(SList<Foo>::size_type(0), list.Size());
            Assert::IsTrue(list.IsEmpty());
        }

        TEST_METHOD(PushMakesCopies) {
            const Foo COPIED{19};

            SList<Foo> list{COPIED};
            list.PushBack(COPIED);
            list.PushBack(COPIED);

            Assert::AreNotEqual(SList<Foo>::size_type(0), list.Size());
            Assert::AreNotEqual(SList<Foo>::size_type(1), list.Size());

            auto sizePrePop = list.Size();
            auto firstPop = list.Back();
            list.PopBack();

            Assert::AreNotEqual(sizePrePop, list.Size());
            Assert::AreEqual(sizePrePop - 1, list.Size());
            Assert::AreEqual(COPIED, firstPop);
            Assert::AreNotSame(COPIED, firstPop);

            auto secondPop = list.Back();
            list.PopBack();

            Assert::AreNotEqual(SList<Foo>::size_type(0), list.Size());
            Assert::IsFalse(list.IsEmpty());
            Assert::AreNotEqual(sizePrePop, list.Size());
            Assert::AreEqual(sizePrePop - 2, list.Size());
            Assert::AreEqual(COPIED, secondPop);
            Assert::AreNotSame(COPIED, secondPop);
            Assert::AreNotSame(COPIED, list.Back());
        }

        TEST_METHOD(Const) {
            const Foo FIRST{20};
            const Foo SECOND{21};
            const Foo THIRD{22};

            SList<Foo> mutlist{FIRST, SECOND, THIRD};
            const SList<Foo>& constlist = mutlist;

            Assert::AreEqual(mutlist.Size(), constlist.Size());
            Assert::AreEqual(mutlist.IsEmpty(), constlist.IsEmpty());
            Assert::AreSame(mutlist.Front(), constlist.Front());
            Assert::AreSame(mutlist.Back(), constlist.Back());
        }

        TEST_METHOD(Swap) {
            const Foo SINGLE{23};
            const Foo DOUBLE_ONE{24};
            const Foo DOUBLE_TWO{25};

            SList<Foo> first{SINGLE};
            SList<Foo> second{DOUBLE_ONE, DOUBLE_TWO};

            Foo* singlePtr = &(first.Front());
            Foo* doubleOnePtr = &(second.Front());
            Foo* doubleTwoPtr = &(second.Back());
            auto singleSize = first.Size();
            auto doubleSize = second.Size();

            Assert::AreEqual(SINGLE, *singlePtr);
            Assert::AreEqual(singlePtr, &(first.Back()));
            Assert::AreEqual(DOUBLE_ONE, *doubleOnePtr);
            Assert::AreEqual(DOUBLE_TWO, *doubleTwoPtr);

            first.swap(second);

            Assert::AreEqual(doubleOnePtr, &(first.Front()));
            Assert::AreEqual(doubleTwoPtr, &(first.Back()));
            Assert::AreEqual(singlePtr, &(second.Front()));
            Assert::AreEqual(singlePtr, &(second.Back()));
            Assert::AreEqual(doubleSize, first.Size());
            Assert::AreEqual(singleSize, second.Size());

            swap(first, second);

            Assert::AreEqual(singlePtr, &(first.Front()));
            Assert::AreEqual(singlePtr, &(first.Back()));
            Assert::AreEqual(doubleOnePtr, &(second.Front()));
            Assert::AreEqual(doubleTwoPtr, &(second.Back()));
            Assert::AreEqual(singleSize, first.Size());
            Assert::AreEqual(doubleSize, second.Size());

            second.swap(first);

            Assert::AreEqual(doubleOnePtr, &(first.Front()));
            Assert::AreEqual(doubleTwoPtr, &(first.Back()));
            Assert::AreEqual(singlePtr, &(second.Front()));
            Assert::AreEqual(singlePtr, &(second.Back()));
            Assert::AreEqual(doubleSize, first.Size());
            Assert::AreEqual(singleSize, second.Size());

            swap(second, first);

            Assert::AreEqual(singlePtr, &(first.Front()));
            Assert::AreEqual(singlePtr, &(first.Back()));
            Assert::AreEqual(doubleOnePtr, &(second.Front()));
            Assert::AreEqual(doubleTwoPtr, &(second.Back()));
            Assert::AreEqual(singleSize, first.Size());
            Assert::AreEqual(doubleSize, second.Size());
        }

        TEST_METHOD(RemoveElement) {
            const Foo FIRST{26};
            const Foo SECOND{27};
            const Foo THIRD{28};
            const Foo FOURTH{29};

            SList<Foo> list{FIRST, SECOND, THIRD, FOURTH};
            list.Remove(THIRD);

            Assert::AreEqual(SList<Foo>::size_type(3), list.Size());

            SList<Foo> check{FIRST, SECOND, FOURTH};

            Assert::IsTrue(std::equal(check.begin(), check.end(), list.begin()));
        }

        TEST_METHOD(IteratorTraversal) {
            const Foo BEGIN{30};
            const Foo END{31};

            SList<Foo> list{BEGIN, END};
            auto it = list.begin();

            Assert::AreEqual(list.begin(), it);
            Assert::AreEqual(BEGIN, *it);
            Assert::AreSame(list.Front(), *it);

            ++it;

            Assert::AreEqual(END, *it);
            Assert::AreSame(list.Back(), *it);

            it++;

            Assert::AreEqual(list.end(), it);
            Assert::ExpectException<std::runtime_error>([&it](){ UNREFERENCED_LOCAL(*it); });
        }

        TEST_METHOD(ConstIteraterTraversal) {
            const Foo BEGIN{32};
            const Foo END{33};

            const SList<Foo> list{BEGIN, END};
            auto it = list.begin();
            auto cit = list.cbegin();

            Assert::AreEqual(it, cit);
            Assert::AreEqual(list.begin(), it);
            Assert::AreEqual(list.cbegin(), it);
            Assert::AreEqual(list.begin(), cit);
            Assert::AreEqual(list.cbegin(), cit);
            Assert::AreEqual(BEGIN, *it);
            Assert::AreSame(list.Front(), *it);
            Assert::AreEqual(BEGIN, *cit);
            Assert::AreSame(list.Front(), *cit);

            ++it;
            ++cit;

            Assert::AreEqual(it, cit);
            Assert::AreEqual(END, *it);
            Assert::AreEqual(END, *cit);
            Assert::AreSame(list.Back(), *it);
            Assert::AreSame(list.Back(), *cit);

            it++;
            cit++;

            Assert::AreEqual(it, cit);
            Assert::AreEqual(list.end(), it);
            Assert::AreEqual(list.cend(), it);
            Assert::AreEqual(list.end(), cit);
            Assert::AreEqual(list.cend(), cit);
            Assert::ExpectException<std::runtime_error>([&it](){ UNREFERENCED_LOCAL(*it); });
            Assert::ExpectException<std::runtime_error>([&cit](){ UNREFERENCED_LOCAL(*cit); });
        }

        TEST_METHOD(Find) {
            const Foo READY{34};
            const Foo SET{35};
            const Foo GO{36};
            const Foo END{37};

            SList<Foo> list{READY, SET, GO};

            auto found = list.Find(READY);

            Assert::AreEqual(list.begin(), found);
            Assert::AreEqual(READY, *found);
            Assert::AreSame(list.Front(), *found);

            found = list.Find(GO);

            Assert::AreEqual(GO, *found);
            Assert::AreSame(list.Back(), *found);

            ++found;

            Assert::AreEqual(list.end(), found);

            found = list.Find(END);

            Assert::AreEqual(list.end(), found);

            found = list.Find(SET);

            Assert::AreEqual(SET, *found);

            ++found;

            Assert::AreEqual(GO, *found);
            Assert::AreSame(list.Back(), *found);
        }

        TEST_METHOD(ConstFind) {
            const Foo READY{38};
            const Foo SET{39};
            const Foo GO{40};
            const Foo END{41};

            const SList<Foo> list{READY, SET, GO};

            auto found = list.Find(READY);
            auto cfound = list.CFind(READY);

            Assert::AreEqual(found, cfound);
            Assert::AreEqual(list.begin(), found);
            Assert::AreEqual(list.cbegin(), found);
            Assert::AreEqual(list.begin(), cfound);
            Assert::AreEqual(list.cbegin(), cfound);
            Assert::AreEqual(READY, *found);
            Assert::AreEqual(READY, *cfound);
            Assert::AreSame(list.Front(), *found);
            Assert::AreSame(list.Front(), *cfound);

            found = list.Find(GO);
            cfound = list.CFind(GO);

            Assert::AreEqual(found, cfound);
            Assert::AreEqual(GO, *found);
            Assert::AreEqual(GO, *cfound);
            Assert::AreSame(list.Back(), *found);
            Assert::AreSame(list.Back(), *cfound);

            ++found;
            ++cfound;

            Assert::AreEqual(found, cfound);
            Assert::AreEqual(list.end(), found);
            Assert::AreEqual(list.cend(), found);
            Assert::AreEqual(list.end(), cfound);
            Assert::AreEqual(list.cend(), cfound);

            found = list.Find(END);
            cfound = list.CFind(END);

            Assert::AreEqual(found, cfound);
            Assert::AreEqual(list.end(), found);
            Assert::AreEqual(list.cend(), found);
            Assert::AreEqual(list.end(), cfound);
            Assert::AreEqual(list.cend(), cfound);

            found = list.Find(SET);
            cfound = list.CFind(SET);

            Assert::AreEqual(found, cfound);
            Assert::AreEqual(SET, *found);
            Assert::AreEqual(SET, *cfound);

            ++found;
            ++cfound;

            Assert::AreEqual(found, cfound);
            Assert::AreEqual(GO, *found);
            Assert::AreEqual(GO, *cfound);
            Assert::AreSame(list.Back(), *found);
            Assert::AreSame(list.Back(), *cfound);
        }

        TEST_METHOD(InsertAfter) {
            const Foo FIRST{42};
            const Foo SECOND{43};
            const Foo THIRD{44};
            const Foo FOURTH{45};
            const Foo FIFTH{46};

            SList<Foo> list{};

            list.InsertAfter(list.begin(), FIRST);

            Assert::AreEqual(SList<Foo>::size_type(1), list.Size());
            Assert::AreEqual(FIRST, list.Front());
            Assert::AreEqual(FIRST, list.Back());
            Assert::AreSame(list.Front(), list.Back());

            list.InsertAfter(list.begin(), FIFTH);

            Assert::AreEqual(SList<Foo>::size_type(2), list.Size());
            Assert::AreNotSame(list.Front(), list.Back());
            Assert::AreNotEqual(list.Front(), list.Back());
            Assert::AreEqual(FIRST, list.Front());
            Assert::AreEqual(FIFTH, list.Back());

            auto it = list.InsertAfter(list.begin(), SECOND);

            Assert::AreEqual(SList<Foo>::size_type(3), list.Size());
            Assert::AreEqual(FIFTH, list.Back());

            list.InsertAfter(it, FOURTH);
            list.InsertAfter(it, THIRD);

            Assert::AreEqual(SList<Foo>::size_type(5), list.Size());

            SList<Foo> check{FIRST, SECOND, THIRD, FOURTH, FIFTH};

            Assert::IsTrue(std::equal(check.begin(), check.end(), list.begin()));

        }

        TEST_METHOD(RemoveHitsEdgeCases) {
            const Foo INFIRST{47};
            const Foo INLAST{48};
            const Foo OUT{49};

            SList<Foo> list{INFIRST, INLAST};
            SList<Foo>::size_type initialSize = list.Size();

            list.Remove(SList<Foo>::iterator{});

            Assert::AreEqual(initialSize, list.Size());

            list.Remove(list.end());

            Assert::AreEqual(initialSize, list.Size());

            list.Remove(OUT);

            Assert::AreEqual(initialSize, list.Size());

            list.Remove(INLAST);

            Assert::AreEqual(initialSize - 1, list.Size());

            list.Remove(INFIRST);

            Assert::AreEqual(initialSize - 2, list.Size());
            Assert::IsTrue(list.IsEmpty());

            list.Remove(INLAST);

            Assert::AreEqual(initialSize - 2, list.Size());

            list.Remove(INFIRST);

            Assert::AreEqual(initialSize - 2, list.Size());
        }

        TEST_METHOD(InvalidIteratorEdgeCases) {
            const Foo SAMPLE{50};

            SList<Foo> one{};
            SList<Foo> two{};

            Assert::ExpectException<std::runtime_error>([&one, &two, &SAMPLE](){ one.InsertAfter(SList<Foo>::iterator{}, SAMPLE); });
            Assert::ExpectException<std::runtime_error>([&one, &two, &SAMPLE](){ one.InsertAfter(two.begin(), SAMPLE); });

            SList<Foo>::iterator invalid{};

            Assert::ExpectException<std::runtime_error>([&invalid](){ ++invalid; });
            Assert::ExpectException<std::runtime_error>([&invalid](){ invalid++; });

            SList<Foo>::iterator cinvalid{};

            Assert::ExpectException<std::runtime_error>([&cinvalid](){ ++cinvalid; });
            Assert::ExpectException<std::runtime_error>([&cinvalid](){ cinvalid++; });
        }
    };
}
