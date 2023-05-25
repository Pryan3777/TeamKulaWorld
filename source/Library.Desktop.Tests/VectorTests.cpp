#include "pch.h"
#include "CppUnitTest.h"
#include "Vector.h"
#include "Foo.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace LibraryDesktopTests {
    TEST_CLASS(VectorTests) {

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
            Vector<Foo> empty{};
            auto emptySize = empty.Size();
            auto emptyCapacity = empty.Capacity();

            Assert::IsTrue(empty.IsEmpty());
            Assert::AreEqual(empty.begin(), empty.end());
            Assert::AreEqual(empty.cbegin(), empty.cend());
            Assert::AreEqual(Vector<Foo>::size_type(0), emptySize);
            Assert::AreEqual(Vector<Foo>::size_type(0), emptyCapacity);
            Assert::ExpectException<std::out_of_range>([&empty](){ auto& tmp = empty.At(0); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::runtime_error>([&empty](){ auto& tmp = empty[0]; UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&empty](){ auto& tmp = empty.Front(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&empty](){ const auto& tmp = empty.CFront(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&empty](){ auto& tmp = empty.Back(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&empty](){ const auto& tmp = empty.CBack(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&empty](){ auto& tmp = *(empty.begin()); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&empty](){ const auto& tmp = *(empty.cbegin()); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&empty](){ auto& tmp = *(empty.end()); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&empty](){ const auto& tmp = *(empty.cend()); UNREFERENCED_LOCAL(tmp); });

            empty.PopBack();

            Assert::AreEqual(emptySize, empty.Size());
            Assert::AreEqual(emptySize, empty.Capacity());

            const Vector<Foo> cempty{};

            Assert::ExpectException<std::out_of_range>([&cempty](){ const auto& tmp = cempty.Front(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&cempty](){ const auto& tmp = cempty.Back(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&cempty](){ const auto& tmp = *(cempty.begin()); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&cempty](){ const auto& tmp = *(cempty.end()); UNREFERENCED_LOCAL(tmp); });
        }

        TEST_METHOD(VectorRandomAccess) {
            using size_type = Vector<Foo>::size_type;

            const Foo FIRST{1};
            const Foo SECOND{2};
            const Foo THIRD{3};

            Vector<Foo> vector{FIRST, SECOND, THIRD};
            Foo& first = vector[size_type(0)];
            Foo& firstAt = vector.At(size_type(0));
            Foo& second = vector[size_type(1)];
            Foo& secondAt = vector.At(size_type(1));
            Foo& third = vector[size_type(2)];
            Foo& thirdAt = vector.At(size_type(2));

            Assert::AreEqual(FIRST, first);
            Assert::AreEqual(FIRST, firstAt);
            Assert::AreEqual(first, firstAt);
            Assert::AreNotSame(FIRST, first);
            Assert::AreNotSame(FIRST, firstAt);
            Assert::AreSame(first, firstAt);
            Assert::AreEqual(SECOND, second);
            Assert::AreEqual(SECOND, secondAt);
            Assert::AreEqual(second, secondAt);
            Assert::AreNotSame(SECOND, second);
            Assert::AreNotSame(SECOND, secondAt);
            Assert::AreSame(second, secondAt);
            Assert::AreEqual(THIRD, third);
            Assert::AreEqual(THIRD, thirdAt);
            Assert::AreEqual(third, thirdAt);
            Assert::AreNotSame(THIRD, third);
            Assert::AreNotSame(THIRD, thirdAt);
            Assert::AreSame(third, thirdAt);
        }

        TEST_METHOD(CapacityManipulation) {
            using size_type = Vector<Foo>::size_type;

            const size_type ZERO = size_type(0);
            const size_type NONZERO_SMALL = size_type(8);
            const size_type NONZERO_MEDIUM = size_type(16);
            const size_type NONZERO_LARGE = size_type(32);
            const Foo FILLER{4};

            Vector<Foo> vector{};

            auto defaultSize = vector.Size();
            auto defaultCapacity = vector.Capacity();

            vector.Reserve(ZERO);

            Assert::AreEqual(defaultSize, vector.Size());
            Assert::AreEqual(defaultCapacity, vector.Capacity());

            vector.ShrinkToFit(NONZERO_SMALL);

            Assert::AreEqual(defaultCapacity, vector.Capacity());

            vector.Reserve(NONZERO_MEDIUM);
            auto mediumCapacity = vector.Capacity();

            Assert::AreNotEqual(vector.Capacity(), vector.Size());
            Assert::AreEqual(defaultSize, vector.Size());
            Assert::AreNotEqual(defaultCapacity, mediumCapacity);
            Assert::AreEqual(NONZERO_MEDIUM, mediumCapacity);

            vector.Reserve(NONZERO_SMALL);

            Assert::AreNotEqual(NONZERO_SMALL, vector.Capacity());
            Assert::AreEqual(mediumCapacity, vector.Capacity());

            vector.PushBack(FILLER);
            vector.PushBack(FILLER);
            vector.PushBack(FILLER);
            vector.PushBack(FILLER);
            auto filledSize = vector.Size();

            Assert::AreNotEqual(filledSize, vector.Capacity());
            Assert::AreEqual(mediumCapacity, vector.Capacity());

            vector.ShrinkToFit(NONZERO_SMALL);
            auto shrunkCapacity = vector.Capacity();

            Assert::AreNotEqual(mediumCapacity, shrunkCapacity);
            Assert::AreNotEqual(filledSize, shrunkCapacity);
            Assert::AreEqual(NONZERO_SMALL, shrunkCapacity);

            vector.ShrinkToFit();
            auto shrunkAgainCapacity = vector.Capacity();

            Assert::AreNotEqual(NONZERO_SMALL, shrunkAgainCapacity);
            Assert::AreNotEqual(shrunkCapacity, shrunkAgainCapacity);
            Assert::AreEqual(filledSize, shrunkAgainCapacity);

            vector.Reserve(NONZERO_LARGE);
            auto filledReservedCapacity = vector.Capacity();

            Assert::AreNotEqual(vector.Size(), filledReservedCapacity);
            Assert::AreNotEqual(filledSize, filledReservedCapacity);
            Assert::AreNotEqual(shrunkAgainCapacity, filledReservedCapacity);
            Assert::AreEqual(NONZERO_LARGE, filledReservedCapacity);

            vector.Clear();
            auto capacityAfterClear = vector.Capacity();

            Assert::AreNotEqual(ZERO, capacityAfterClear);
            Assert::AreEqual(filledReservedCapacity, capacityAfterClear);

            vector.ShrinkToFit();

            Assert::AreEqual(defaultCapacity, vector.Capacity());
        }

        TEST_METHOD(Destructor) {
            const Foo FILLER{5};

            Vector<Foo> vector{};
            auto defaultSize = vector.Size();
            auto defaultCapacity = vector.Capacity();

            vector.PushBack(FILLER);
            vector.PushBack(FILLER);
            vector.PushBack(FILLER);
            vector.PushBack(FILLER);
            vector.PushBack(FILLER);

            auto filledSize = vector.Size();
            auto filledCapacity = vector.Capacity();

            Assert::AreNotEqual(defaultSize, filledSize);
            Assert::AreNotEqual(defaultCapacity, filledCapacity);

            vector.~Vector();
            auto destructedSize = vector.Size();
            auto destructedCapacity = vector.Capacity();

            Assert::AreNotEqual(filledSize, destructedSize);
            Assert::AreNotEqual(filledCapacity, destructedCapacity);
            Assert::AreEqual(defaultSize, destructedSize);
            Assert::AreEqual(defaultCapacity, destructedCapacity);

            const Vector<Foo> cvector{FILLER, FILLER, FILLER};
            filledSize = cvector.Size();
            filledCapacity = cvector.Capacity();

            Assert::AreNotEqual(defaultSize, filledSize);
            Assert::AreNotEqual(defaultCapacity, filledCapacity);

            cvector.~Vector();
            destructedSize = cvector.Size();
            destructedCapacity = cvector.Capacity();

            Assert::AreNotEqual(filledSize, destructedSize);
            Assert::AreNotEqual(filledCapacity, destructedCapacity);
            Assert::AreEqual(defaultSize, destructedSize);
            Assert::AreEqual(defaultCapacity, destructedCapacity);
        }

        TEST_METHOD(Clear) {
            const Foo FILLER{6};

            Vector<Foo> vector{};
            auto defaultSize = vector.Size();
            auto defaultCapacity = vector.Capacity();

            vector.PushBack(FILLER);
            auto filledSize = vector.Size();
            auto filledCapacity = vector.Capacity();

            Assert::AreNotEqual(defaultSize, filledSize);
            Assert::AreNotEqual(defaultCapacity, filledCapacity);

            vector.Clear();
            auto clearedSize = vector.Size();
            auto clearedCapacity = vector.Capacity();

            Assert::AreNotEqual(filledSize, clearedSize);
            Assert::AreEqual(defaultSize, clearedSize);
            Assert::AreNotEqual(defaultCapacity, clearedCapacity);
            Assert::AreEqual(filledCapacity, clearedCapacity);
        }

        TEST_METHOD(PopBack) {
            const Foo FRONT{7};
            const Foo BACK{8};

            Vector<Foo> vector{FRONT, BACK};
            Foo& initialFront = vector.Front();
            Foo& initialBack = vector.Back();
            auto initialSize = vector.Size();
            auto initialCapacity = vector.Capacity();

            Assert::AreNotEqual(initialFront, initialBack);
            Assert::AreNotSame(initialFront, initialBack);
            Assert::AreEqual(FRONT, initialFront);
            Assert::AreEqual(BACK, initialBack);

            vector.PopBack();
            Foo& frontAfterOnePop = vector.Front();
            Foo& backAfterOnePop = vector.Back();
            auto sizeAfterOnePop = vector.Size();

            Assert::AreEqual(initialCapacity, vector.Capacity());
            Assert::AreNotEqual(initialSize, sizeAfterOnePop);
            Assert::AreEqual(frontAfterOnePop, backAfterOnePop);
            Assert::AreSame(frontAfterOnePop, backAfterOnePop);
            Assert::AreEqual(FRONT, frontAfterOnePop);
            Assert::AreNotEqual(BACK, backAfterOnePop);
            Assert::AreEqual(FRONT, backAfterOnePop);

            vector.PopBack();
            auto sizeAfterTwoPops = vector.Size();

            Assert::AreEqual(initialCapacity, vector.Capacity());
            Assert::AreNotEqual(sizeAfterOnePop, sizeAfterTwoPops);
            Assert::IsTrue(vector.IsEmpty());
            Assert::ExpectException<std::out_of_range>([&vector](){ auto& tmp = vector.Front(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&vector](){ auto& tmp = vector.Back(); UNREFERENCED_LOCAL(tmp); });

            vector.PopBack();
            auto sizeAfterThreePops = vector.Size();

            Assert::AreEqual(initialCapacity, vector.Capacity());
            Assert::AreEqual(sizeAfterTwoPops, sizeAfterThreePops);
            Assert::IsTrue(vector.IsEmpty());
        }

        TEST_METHOD(FrontBack) {
            const Foo FRONT{9};
            const Foo BACK{10};
            const Foo BACKATTACK{11};

            Vector<Foo> vector{FRONT, BACK};
            vector.Reserve(vector.Capacity() * Vector<Foo>::size_type(10));
            Foo& initialFront = vector.Front();
            const Foo& initialCFront = vector.CFront();
            const Foo& initialCcFront = const_cast<const Vector<Foo>*>(&vector)->Front();
            Foo& initialBack = vector.Back();
            const Foo& initialCBack = vector.CBack();
            const Foo& initialCcBack = const_cast<const Vector<Foo>*>(&vector)->Back();

            Assert::AreEqual(FRONT, initialFront);
            Assert::AreEqual(FRONT, initialCFront);
            Assert::AreEqual(FRONT, initialCcFront);
            Assert::AreNotSame(FRONT, initialFront);
            Assert::AreNotSame(FRONT, initialCFront);
            Assert::AreNotSame(FRONT, initialCcFront);
            Assert::AreSame(initialFront, initialCFront);
            Assert::AreSame(initialFront, initialCcFront);
            Assert::AreSame(initialCFront, initialCcFront);
            Assert::AreEqual(BACK, initialBack);
            Assert::AreEqual(BACK, initialCBack);
            Assert::AreEqual(BACK, initialCcBack);
            Assert::AreNotSame(BACK, initialBack);
            Assert::AreNotSame(BACK, initialCBack);
            Assert::AreNotSame(BACK, initialCcBack);
            Assert::AreSame(initialBack, initialCBack);
            Assert::AreSame(initialBack, initialCcBack);
            Assert::AreSame(initialCBack, initialCcBack);

            vector.PushBack(BACKATTACK);
            Foo& postpushFront = vector.Front();
            const Foo& postpushCFront = vector.CFront();
            const Foo& postpushCcFront = const_cast<const Vector<Foo>*>(&vector)->Front();
            Foo& postpushBack = vector.Back();
            const Foo& postpushCBack = vector.CBack();
            const Foo& postpushCcBack = const_cast<const Vector<Foo>*>(&vector)->Back();

            Assert::AreEqual(FRONT, postpushFront);
            Assert::AreEqual(FRONT, postpushCFront);
            Assert::AreEqual(FRONT, postpushCcFront);
            Assert::AreNotSame(FRONT, postpushFront);
            Assert::AreNotSame(FRONT, postpushCFront);
            Assert::AreNotSame(FRONT, postpushCcFront);
            Assert::AreSame(postpushFront, postpushCFront);
            Assert::AreSame(postpushFront, postpushCcFront);
            Assert::AreSame(postpushCFront, postpushCcFront);
            Assert::AreSame(initialFront, postpushFront);
            Assert::AreSame(initialCFront, postpushCFront);
            Assert::AreSame(initialCcFront, postpushCcFront);
            Assert::AreNotEqual(BACK, postpushBack);
            Assert::AreNotEqual(BACK, postpushCBack);
            Assert::AreNotEqual(BACK, postpushCcBack);
            Assert::AreNotEqual(initialBack, postpushBack);
            Assert::AreNotEqual(initialBack, postpushCBack);
            Assert::AreNotEqual(initialBack, postpushCcBack);
            Assert::AreNotSame(initialBack, postpushBack);
            Assert::AreNotSame(initialBack, postpushCBack);
            Assert::AreNotSame(initialBack, postpushCcBack);
            Assert::AreEqual(BACKATTACK, postpushBack);
            Assert::AreEqual(BACKATTACK, postpushCBack);
            Assert::AreEqual(BACKATTACK, postpushCcBack);
            Assert::AreNotSame(BACKATTACK, postpushBack);
            Assert::AreNotSame(BACKATTACK, postpushCBack);
            Assert::AreNotSame(BACKATTACK, postpushCcBack);
            Assert::AreSame(postpushBack, postpushCBack);
            Assert::AreSame(postpushBack, postpushCcBack);
            Assert::AreSame(postpushCBack, postpushCcBack);
        }

        TEST_METHOD(Copy) {
            const Foo FIRST{12};
            const Foo SECOND{13};
            const Foo THIRD{14};
            const Foo FOURTH{15};
            const Foo FIFTH{16};

            Vector<Foo> original{FIRST, SECOND, THIRD};
            Vector<Foo> copyConstructed{original};

            Assert::IsTrue(std::equal(original.cbegin(), original.cend(), copyConstructed.cbegin()));
            Assert::AreEqual(original.Size(), copyConstructed.Size());
            Assert::AreNotSame(original.CFront(), copyConstructed.CFront());
            Assert::AreNotSame(original.CBack(), copyConstructed.CBack());

            original.PushBack(FOURTH);

            Assert::IsFalse(std::equal(copyConstructed.cbegin(), copyConstructed.cend(), original.cbegin(), original.cend()));

            Vector<Foo> copyAssigned{FOURTH, FIFTH};
            Vector<Foo> preCopy{copyAssigned};
            copyAssigned = original;

            Assert::IsFalse(std::equal(preCopy.cbegin(), preCopy.cend(), copyAssigned.cbegin(), copyAssigned.cend()));
            Assert::IsTrue(std::equal(original.cbegin(), original.cend(), copyAssigned.cbegin()));
            Assert::AreEqual(original.Size(), copyAssigned.Size());
            Assert::AreNotSame(original.CFront(), copyAssigned.CFront());
            Assert::AreNotSame(original.CBack(), copyAssigned.CBack());

            original.PushBack(FIFTH);

            Assert::IsFalse(std::equal(original.cbegin(), original.cend(), copyAssigned.cbegin(), copyAssigned.cend()));
        }

        TEST_METHOD(Move) {
            const Foo FIRST{17};
            const Foo SECOND{18};
            const Foo THIRD{19};

            Vector<Foo> original{FIRST, SECOND, THIRD};
            Vector<Foo> copy{original};
            Vector<Foo> moveConstructed{std::move(copy)};

            Assert::IsTrue(std::equal(original.cbegin(), original.cend(), moveConstructed.cbegin()));
            Assert::AreEqual(original.Size(), moveConstructed.Size());
            Assert::AreEqual(original.Capacity(), moveConstructed.Capacity());
#pragma warning(push)
#pragma warning(disable: 26800)
            Assert::IsTrue(copy.IsEmpty());
#pragma warning(pop)

            copy = original;
            Vector<Foo> moveAssigned{THIRD, THIRD, SECOND, THIRD, FIRST, SECOND};
            Vector<Foo> preMove{moveAssigned};
            moveAssigned = std::move(copy);

            Assert::IsFalse(std::equal(preMove.cbegin(), preMove.cend(), moveAssigned.cbegin()));
            Assert::IsTrue(std::equal(original.cbegin(), original.cend(), moveAssigned.cbegin()));
            Assert::AreEqual(original.Size(), moveAssigned.Size());
            Assert::AreEqual(original.Capacity(), moveAssigned.Capacity());
#pragma warning(push)
#pragma warning(disable: 26800)
            Assert::IsTrue(copy.IsEmpty());
#pragma warning(pop)
        }

        TEST_METHOD(Swap) {
            const Foo FIRST_X{20};
            const Foo FIRST_Y{21};
            const Foo SECOND_Z{22};

            Vector<Foo> first{FIRST_X, FIRST_Y};
            Vector<Foo> second{SECOND_Z};
            Vector<Foo> firstCopy{first};
            Vector<Foo> secondCopy{second};
            first.swap(second);

            Assert::IsFalse(std::equal(firstCopy.cbegin(), firstCopy.cend(), first.cbegin()));
            Assert::IsTrue(std::equal(firstCopy.cbegin(), firstCopy.cend(), second.cbegin()));
            Assert::IsFalse(std::equal(secondCopy.cbegin(), secondCopy.cend(), second.cbegin()));
            Assert::IsTrue(std::equal(secondCopy.cbegin(), secondCopy.cend(), first.cbegin()));
            Assert::AreEqual(firstCopy.Size(), second.Size());
            Assert::AreEqual(firstCopy.Capacity(), second.Capacity());
            Assert::AreEqual(secondCopy.Size(), first.Size());
            Assert::AreEqual(secondCopy.Capacity(), first.Capacity());

            Vector<Foo> empty{};
            std::swap(second, empty);

            Assert::IsTrue(second.IsEmpty());
            Assert::IsTrue(std::equal(firstCopy.cbegin(), firstCopy.cend(), empty.cbegin()));
            Assert::AreEqual(firstCopy.Size(), empty.Size());
            Assert::AreEqual(firstCopy.Capacity(), empty.Capacity());
            Assert::AreEqual(secondCopy.Size(), first.Size());
            Assert::AreEqual(secondCopy.Capacity(), first.Capacity());
        }

        TEST_METHOD(Find) {
            const Foo FIRST{23};
            const Foo SECOND{24};
            const Foo THIRD{25};
            const Foo FOURTH{26};
            const Foo FIFTH{27};
            const Foo SIXTH{28};

            Vector<Foo> vector{FIRST, SECOND, THIRD, FOURTH};
            auto it = vector.begin();
            auto found = vector.Find(SECOND);

            Assert::AreSame(vector[1], *found);
            Assert::AreSame(it[1], *found);
            Assert::AreEqual(it, found);

            const decltype(vector)& cvector = vector;
            auto cit = cvector.begin();
            auto cfound = cvector.Find(FIRST);

            Assert::AreSame(cvector[0], *cfound);
            Assert::AreSame(cvector.Front(), *cfound);
            Assert::AreEqual(cit, cfound);

            cfound = cvector.Find(FOURTH);

            Assert::AreSame(cvector[3], *cfound);
            Assert::AreSame(cvector.Back(), *cfound);
            Assert::AreSame(cit[3], *cfound);
            Assert::AreEqual(cit, cfound);

            found = vector.Find(FIFTH);

            Assert::AreEqual(vector.end(), found);

            cfound = vector.CFind(SIXTH);

            Assert::AreEqual(vector.cend(), cfound);
        }

        TEST_METHOD(Remove) {
            const Foo FIRST{29};
            const Foo SECOND{30};
            const Foo THIRD{31};
            const Foo FOURTH{32};
            const Foo FIFTH{33};
            const Foo SIXTH{34};
            const Foo SEVENTH{35};

            Vector<Foo> vector{FIRST, SECOND, THIRD, FOURTH, FIFTH, SIXTH};

            Assert::ExpectException<std::invalid_argument>([&vector](){ vector.Remove(Vector<Foo>::iterator{}); });

            Vector<Foo> predicted{vector};

            vector.Remove(SEVENTH);

            Assert::IsTrue(std::equal(predicted.cbegin(), predicted.cend(), vector.cbegin()));
            Assert::AreEqual(vector.Size(), predicted.Size());

            vector.Remove(vector.end());

            Assert::IsTrue(std::equal(predicted.cbegin(), predicted.cend(), vector.cbegin()));
            Assert::AreEqual(vector.Size(), predicted.Size());

            predicted = Vector<Foo>{FIRST, SECOND, THIRD, FIFTH, SIXTH};
            vector.Remove(FOURTH);

            Assert::IsTrue(std::equal(predicted.cbegin(), predicted.cend(), vector.cbegin()));
            Assert::AreEqual(vector.Size(), predicted.Size());

            predicted = Vector<Foo>{FIRST, THIRD, FIFTH, SIXTH};
            vector.RemoveAt(Vector<Foo>::size_type(1));

            Assert::IsTrue(std::equal(predicted.cbegin(), predicted.cend(), vector.cbegin()));
            Assert::AreEqual(vector.Size(), predicted.Size());

            predicted = Vector<Foo>{FIRST, SIXTH};
            auto start = vector.begin();
            auto finish = vector.end();
            vector.Remove(++start, --finish);

            Assert::IsTrue(std::equal(predicted.cbegin(), predicted.cend(), vector.cbegin()));
            Assert::AreEqual(vector.Size(), predicted.Size());

            predicted = Vector<Foo>{FIRST};
            vector.Remove(vector.Back());

            Assert::IsTrue(std::equal(predicted.cbegin(), predicted.cend(), vector.cbegin()));
            Assert::AreEqual(vector.Size(), predicted.Size());

            predicted.Clear();
            vector.Remove(vector.begin());

            Assert::IsTrue(std::equal(predicted.cbegin(), predicted.cend(), vector.cbegin()));
            Assert::AreEqual(vector.Size(), predicted.Size());

            vector = Vector<Foo>{SIXTH, FIFTH, FOURTH, THIRD, SECOND, FIRST};
            predicted.~Vector();
            vector.Remove(vector.begin(), vector.end());

            Assert::IsTrue(std::equal(predicted.cbegin(), predicted.cend(), vector.cbegin()));
            Assert::AreEqual(vector.Size(), predicted.Size());
        }

        TEST_METHOD(PushEmplace) {
            const Foo FIRST{36};
            const Foo SECOND{37};
            const Foo THIRD{38};
            const Foo FOURTH{39};
            const Foo FIFTH{40};

            Vector<Foo> vector{};
            Vector<Foo> predicted{FIRST};

            vector.PushBack(FIRST);

            Assert::IsTrue(std::equal(predicted.cbegin(), predicted.cend(), vector.cbegin()));
            Assert::AreEqual(vector.Size(), predicted.Size());

            Foo second{SECOND};
            predicted = Vector<Foo>{FIRST, SECOND};
            vector.PushBack(std::move(second));

            Assert::IsTrue(std::equal(predicted.cbegin(), predicted.cend(), vector.cbegin()));
            Assert::AreEqual(vector.Size(), predicted.Size());
            Assert::AreEqual(FIRST, vector.CFront());
            Assert::AreEqual(SECOND, vector.CBack());
            Assert::AreNotSame(FIRST, vector.CFront());
            Assert::AreNotSame(SECOND, vector.CBack());

            Foo third{THIRD};
            predicted = Vector<Foo>{FIRST, SECOND, THIRD};
            vector.EmplaceBack(third);

            Assert::IsTrue(std::equal(predicted.cbegin(), predicted.cend(), vector.cbegin()));
            Assert::AreEqual(vector.Size(), predicted.Size());

            Foo fourth{FOURTH};
            predicted = Vector<Foo>{FIRST, SECOND, THIRD, FOURTH};
            vector.EmplaceBack(std::move(fourth));

            Assert::IsTrue(std::equal(predicted.cbegin(), predicted.cend(), vector.cbegin()));
            Assert::AreEqual(vector.Size(), predicted.Size());

            predicted = Vector<Foo>{FIRST, SECOND, THIRD, FOURTH, FIFTH};
            vector.EmplaceBack(FIFTH.Data());

            Assert::IsTrue(std::equal(predicted.cbegin(), predicted.cend(), vector.cbegin()));
            Assert::AreEqual(vector.Size(), predicted.Size());

            Foo secondSecond{SECOND};
            vector.Clear();
            vector.EmplaceBack(FIRST);
            vector.EmplaceBack(std::move(secondSecond));

            Assert::AreEqual(FIRST, vector.CFront());
            Assert::AreEqual(SECOND, vector.CBack());
            Assert::AreNotSame(FIRST, vector.CFront());
            Assert::AreNotSame(SECOND, vector.CBack());
        }

        TEST_METHOD(CustomGrowCapacityFunctor) {
            using size_type = Vector<Foo>::size_type;
            const Foo FILLER{41};

            Vector<Foo> vector{};
            auto capacityCheck = vector.Capacity();

            vector.SetGrowCapacityFunctor([](size_type, size_type) { return size_type(0); });

            vector.PushBack(FILLER);

            Assert::AreEqual(++capacityCheck, vector.Capacity());

            vector.SetGrowCapacityFunctor([&FILLER](size_type, size_type) { return size_type(FILLER.Data()); });

            vector.EmplaceBack(FILLER);

            Assert::AreEqual(size_type(FILLER.Data()), vector.Capacity());

            vector.SetGrowCapacityFunctor([](size_type size, size_type capacity) {
                return size + capacity;
            });

            vector.EmplaceBack(FILLER.Data());

            Assert::AreEqual(size_type(FILLER.Data()), vector.Capacity());
        }

        TEST_METHOD(DefaultGrowCapacityFunctor) {
            using size_type = Vector<Foo>::size_type;
            const Foo FILLER{42};

            Vector<Foo> vector{};

            Assert::AreEqual(size_type(0), vector.Capacity());

            vector.PushBack(FILLER);

            Assert::AreEqual(size_type(8), vector.Capacity());

            size_type i;
            for (i = size_type(0); i < size_type(8); ++i) {
                vector.PushBack(FILLER);
            }

            Assert::AreEqual(size_type(16), vector.Capacity());

            for (i = size_type(0); i < size_type(3000); ++i) {
                vector.PushBack(FILLER);
            }

            Assert::AreEqual(size_type(1024 * 3), vector.Capacity());

            DefaultGrowCapacity functor{};

            Assert::AreEqual(size_type(FILLER.Data()), functor(size_type(0), size_type(FILLER.Data())));
            Assert::ExpectException<std::overflow_error>([&functor](){ auto tmp = functor(size_type(-1), size_type(-1)); UNREFERENCED_LOCAL(tmp); });
        }

        TEST_METHOD(IteratorComparison) {
            const Foo FIRST{43};
            const Foo SECOND{44};
            const Foo THIRD{45};

            Vector<Foo> vector{FIRST, SECOND, THIRD};

            auto middle = ++(vector.begin());
            auto decFront = --middle;
            auto incFront = middle++;
            auto incBack = ++middle;
            auto decBack = middle--;

            Assert::AreEqual(incFront, decFront);
            Assert::AreEqual(incBack, decBack);
            Assert::AreSame(*incFront, *decFront);
            Assert::AreSame(*incBack, *decBack);
            Assert::AreNotEqual(incFront, middle);
            Assert::AreNotEqual(incBack, middle);
            Assert::AreNotEqual(decFront, middle);
            Assert::AreNotEqual(decBack, middle);
            Assert::IsTrue(incFront < middle);
            Assert::IsTrue(middle < incBack);
            Assert::IsFalse(incFront > incBack);
            Assert::IsTrue(decBack > middle);
            Assert::IsTrue(middle > decFront);
            Assert::IsFalse(decBack < decFront);
            Assert::IsTrue(incFront <= middle);
            Assert::IsTrue(middle <= incBack);
            Assert::IsFalse(incFront >= incBack);
            Assert::IsTrue(decBack >= middle);
            Assert::IsTrue(middle >= decFront);
            Assert::IsFalse(decBack <= decFront);
        }

        TEST_METHOD(ConstIteratorComparison) {
            const Foo FIRST{46};
            const Foo SECOND{47};
            const Foo THIRD{48};

            Vector<Foo> vector{FIRST, SECOND, THIRD};

            auto middle = ++(vector.cbegin());
            auto decFront = --middle;
            auto incFront = middle++;
            auto incBack = ++middle;
            auto decBack = middle--;

            Assert::AreEqual(incFront, decFront);
            Assert::AreEqual(incBack, decBack);
            Assert::AreSame(*incFront, *decFront);
            Assert::AreSame(*incBack, *decBack);
            Assert::AreNotEqual(incFront, middle);
            Assert::AreNotEqual(incBack, middle);
            Assert::AreNotEqual(decFront, middle);
            Assert::AreNotEqual(decBack, middle);
            Assert::IsTrue(incFront < middle);
            Assert::IsTrue(middle < incBack);
            Assert::IsFalse(incFront > incBack);
            Assert::IsTrue(decBack > middle);
            Assert::IsTrue(middle > decFront);
            Assert::IsFalse(decBack < decFront);
            Assert::IsTrue(incFront <= middle);
            Assert::IsTrue(middle <= incBack);
            Assert::IsFalse(incFront >= incBack);
            Assert::IsTrue(decBack >= middle);
            Assert::IsTrue(middle >= decFront);
            Assert::IsFalse(decBack <= decFront);
        }

        TEST_METHOD(IteratorArithmetic) {
            using size_type = Vector<Foo>::size_type;
            using diff_type = Vector<Foo>::difference_type;

            const Foo FIRST{49};
            const Foo SECOND{50};
            const Foo THIRD{51};
            const Foo FOURTH{52};

            Vector<Foo> vector{FIRST, SECOND, THIRD, FOURTH};

            Assert::AreEqual(vector.end() - vector.begin(), static_cast<diff_type>(vector.Size()));

            auto end = vector.begin();
            end += size_type(4);
            auto middle = end;
            middle -= size_type(2);

            Assert::AreEqual(vector.begin() + size_type(2), middle);
            Assert::AreEqual(vector.end() - size_type(2), middle);
            Assert::AreEqual(vector.end(), end);
        }

        TEST_METHOD(ConstIteratorArithmetic) {
            using size_type = Vector<Foo>::size_type;
            using diff_type = Vector<Foo>::difference_type;

            const Foo FIRST{49};
            const Foo SECOND{50};
            const Foo THIRD{51};
            const Foo FOURTH{52};

            Vector<Foo> vector{FIRST, SECOND, THIRD, FOURTH};

            Assert::AreEqual(vector.cend() - vector.cbegin(), static_cast<diff_type>(vector.Size()));

            auto end = vector.cbegin();
            end += size_type(4);
            auto middle = end;
            middle -= size_type(2);

            Assert::AreEqual(vector.cbegin() + size_type(2), middle);
            Assert::AreEqual(vector.cend() - size_type(2), middle);
            Assert::AreEqual(vector.cend(), end);
        }

        TEST_METHOD(DefaultIterator) {
            Vector<Foo>::iterator it{};
            auto copy = it++;
            ++it;

            Assert::ExpectException<std::runtime_error>([&it](){ auto& tmp = *it; UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::runtime_error>([&copy](){ auto& tmp = copy[Vector<Foo>::size_type(1)]; UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::runtime_error>([&it, &copy](){ bool less = copy < it; UNREFERENCED_LOCAL(less); });
        }

        TEST_METHOD(DefaultConstIterator) {
            Vector<Foo>::const_iterator it{};
            auto copy = it++;
            ++it;

            Assert::ExpectException<std::runtime_error>([&it](){ auto& tmp = *it; UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::runtime_error>([&copy](){ auto& tmp = copy[Vector<Foo>::size_type(1)]; UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::runtime_error>([&it, &copy](){ bool less = copy < it; UNREFERENCED_LOCAL(less); });
        }

        TEST_METHOD(CopySelfAssignment) {
            const Foo FILLER{55};

            Vector<Foo> vector{FILLER, FILLER, FILLER};
            Vector<Foo> copy{vector};
            auto capacity = vector.Capacity();
            vector = vector;

            Assert::IsTrue(std::equal(copy.cbegin(), copy.cend(), vector.cbegin(), vector.cend()));
            Assert::AreEqual(capacity, vector.Capacity());
        }

        TEST_METHOD(MoveSelfAssignment) {
            const Foo FILLER{56};

            Vector<Foo> vector{FILLER, FILLER, FILLER};
            Vector<Foo> copy{vector};
            auto capacity = vector.Capacity();
            vector = std::move(vector);

#pragma warning(push)
#pragma warning(disable: 26800)
            Assert::IsTrue(std::equal(copy.cbegin(), copy.cend(), vector.cbegin(), vector.cend()));
            Assert::AreEqual(capacity, vector.Capacity());
#pragma warning(pop)
        }

        TEST_METHOD(ShrinkToFitDoesntIncreaseCapacity) {
            Vector<Foo> empty{};
            auto capacity = empty.Capacity();
            empty.ShrinkToFit(Vector<Foo>::size_type(57));

            Assert::AreEqual(capacity, empty.Capacity());
        }

        TEST_METHOD(PushEmplaceBackReturnsIterator) {
            const Foo FIRST{58};
            const Foo SECOND{59};
            const Foo THIRD{60};

            Vector<Foo> vector{};

            auto it = vector.PushBack(FIRST);

            Assert::AreEqual(FIRST, *it);

            Foo second{SECOND};
            it = vector.PushBack(std::move(second));

            Assert::AreEqual(SECOND, *it);

            it = vector.EmplaceBack(THIRD.Data());

            Assert::AreEqual(THIRD, *it);
        }
    };
}
