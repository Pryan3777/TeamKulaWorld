#include "pch.h"
#include "CppUnitTest.h"
#include "HashMap.h"
#include "Foo.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std::literals::string_literals;
using std::string;

template<> std::size_t FieaGameEngine::DefaultHash<LibraryDesktopTests::Foo>::operator()(const LibraryDesktopTests::Foo& key) {
    return static_cast<std::size_t>(key.Data());
}

namespace LibraryDesktopTests {
    TEST_CLASS(HashMapTests) {

    private:
        inline static _CrtMemState _startMemState;

        inline static constexpr std::size_t SMALL_MAX_HASH_VAL = 31;

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
            using size_type = HashMap<string, Foo>::size_type;

            const string TEST = "TEST"s;

            HashMap<string, Foo> empty{};
            auto emptySize = empty.Size();
            auto emptyMaxHashValue = empty.MaxHashValue();
            auto emptyUsedHashValueCount = empty.UsedHashValueCount();

            Assert::IsTrue(empty.IsEmpty());
            Assert::AreEqual(empty.begin(), empty.end());
            Assert::AreEqual(empty.cbegin(), empty.cend());
            Assert::AreEqual(size_type(0), emptySize);
            Assert::AreNotEqual(size_type(0), emptyMaxHashValue);
            Assert::AreEqual(size_type(0), emptyUsedHashValueCount);
            Assert::ExpectException<std::out_of_range>([&empty, &TEST](){ auto& tmp = empty.At(TEST); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&empty, &TEST](){ const auto& tmp = empty.CAt(TEST); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::runtime_error>([&empty](){ auto& tmp = *(empty.begin()); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::runtime_error>([&empty](){ const auto& tmp = *(empty.cbegin()); UNREFERENCED_LOCAL(tmp); });

            empty.Remove(TEST);

            Assert::IsFalse(empty.RehashIfNeeded());
            Assert::AreEqual(emptySize, empty.Size());
            Assert::AreEqual(emptyMaxHashValue, empty.MaxHashValue());
            Assert::AreEqual(emptyUsedHashValueCount, empty.UsedHashValueCount());

            const HashMap<string, Foo> cempty{};

            Assert::AreEqual(cempty.begin(), cempty.end());
            Assert::ExpectException<std::out_of_range>([&cempty, &TEST](){ const auto& tmp = cempty.At(TEST); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::runtime_error>([&cempty](){ const auto& tmp = *(cempty.begin()); UNREFERENCED_LOCAL(tmp); });
        }

        TEST_METHOD(InvalidArgumentConstructor) {
            using size_type = HashMap<string, Foo>::size_type;

            Assert::ExpectException<std::invalid_argument>([](){ HashMap<string, Foo> bad{size_type(0)}; UNREFERENCED_LOCAL(bad); });
            Assert::ExpectException<std::invalid_argument>([](){ HashMap<string, Foo> bad{size_type(1)}; UNREFERENCED_LOCAL(bad); });

            HashMap<string, Foo> ok{size_type(2)};

            Assert::AreEqual(size_type(2), ok.MaxHashValue());
        }

        TEST_METHOD(CustomHashFunctor) {
            using size_type = HashMap<Foo, string>::size_type;

            const Foo ONE{1};
            const Foo TWO{2};
            const Foo THREE{3};
            const string TEST = "TEST"s;
            size_type* uses = new size_type(0);

            HashMap<Foo, string> customHash{SMALL_MAX_HASH_VAL, [&uses](const Foo& key) { ++(*uses); UNREFERENCED_LOCAL(key); return size_type(0); }};

            Assert::AreEqual(size_type(0), *uses);

            customHash.InsertOrAssign(std::make_pair(ONE, TEST));
            customHash.InsertOrAssign(std::make_pair(TWO, TEST));
            customHash.InsertOrAssign(std::make_pair(THREE, TEST));

            Assert::AreEqual(size_type(3), *uses);
            Assert::AreEqual(size_type(3), customHash.Size());
            Assert::AreEqual(size_type(1), customHash.UsedHashValueCount());
            Assert::AreEqual(size_type(0), customHash.GetHashFunctor()(ONE));
            Assert::AreEqual(size_type(4), *uses);

            auto it = customHash.cbegin();

            Assert::AreEqual(ONE, (*(it++)).first);
            Assert::AreEqual(TWO, (*(it++)).first);
            Assert::AreEqual(THREE, (*(it++)).first);
            Assert::AreEqual(it, customHash.cend());

            delete uses;
        }

        TEST_METHOD(CustomKeyCompareFunctor) {
            using size_type = HashMap<string, Foo>::size_type;

            const string FIRST = "FIRST"s;
            const string SECOND = "SECOND"s;
            const string THIRD = "THIRD"s;
            const Foo FILLER{4};
            size_type* uses = new size_type(0);

            HashMap<string, Foo> customKeyCompare{SMALL_MAX_HASH_VAL, [uses](const string& lhs, const string& rhs) {
                ++(*uses);
                return strcmp(lhs.c_str(), rhs.c_str()) == 0;
            }};

            Assert::AreEqual(size_type(0), *uses);

            customKeyCompare.InsertOrAssign(std::make_pair(FIRST, FILLER));
            customKeyCompare.InsertOrAssign(std::make_pair(SECOND, FILLER));
            customKeyCompare.InsertOrAssign(std::make_pair(THIRD, FILLER));

            Assert::AreEqual(size_type(0), *uses);
            Assert::IsTrue(customKeyCompare.IsContainingKey(THIRD));
            Assert::AreEqual(size_type(1), *uses);
            Assert::IsTrue(customKeyCompare.GetKeyCompareFunctor()(FIRST, FIRST));
            Assert::IsFalse(customKeyCompare.GetKeyCompareFunctor()(SECOND, THIRD));
            Assert::AreEqual(size_type(3), *uses);

            delete uses;
        }

        TEST_METHOD(CustomEmplaceDefaultFunctor) {
            using size_type = HashMap<string, Foo>::size_type;

            const string FIRST = "FIRST"s;
            const string SECOND = "SECOND"s;
            const string THIRD = "THIRD"s;
            const Foo FILLER{5};
            const Foo THRILLER{6};
            size_type* uses = new size_type(0);

            HashMap<string, Foo> customEmplaceDefault{SMALL_MAX_HASH_VAL, [uses, FILLER](){ ++(*uses); return FILLER; }};

            Assert::AreEqual(size_type(0), *uses);

            auto& first = customEmplaceDefault[FIRST];

            Assert::AreEqual(FILLER, first);
            Assert::AreSame(first, customEmplaceDefault[FIRST]);
            Assert::AreEqual(size_type(1), *uses);
            Assert::AreEqual(size_type(1), customEmplaceDefault.Size());
            Assert::AreEqual(size_type(1), customEmplaceDefault.UsedHashValueCount());
            Assert::AreEqual(FILLER, customEmplaceDefault[SECOND]);
            Assert::AreEqual(FILLER, customEmplaceDefault[THIRD]);
            Assert::AreEqual(size_type(3), *uses);
            Assert::AreEqual(size_type(3), customEmplaceDefault.Size());
            Assert::AreEqual(size_type(3), customEmplaceDefault.UsedHashValueCount());

            customEmplaceDefault[SECOND] = THRILLER;

            Assert::AreEqual(size_type(3), *uses);
            Assert::AreEqual(size_type(3), customEmplaceDefault.Size());
            Assert::AreEqual(size_type(3), customEmplaceDefault.UsedHashValueCount());
            Assert::AreEqual(THRILLER, customEmplaceDefault[SECOND]);
            Assert::AreEqual(FILLER, customEmplaceDefault.GetEmplaceDefaultFunctor()());
            Assert::AreEqual(size_type(4), *uses);

            customEmplaceDefault.SetEmplaceDefaultFunctor([uses, THRILLER](){ ++(*uses); return THRILLER; });

            Assert::AreEqual(THRILLER, customEmplaceDefault.GetEmplaceDefaultFunctor()());
            Assert::AreEqual(size_type(5), *uses);

            delete uses;
        }

        TEST_METHOD(DefaultFunctors) {
            const string FILLER = "FILLER"s;
            const Foo THRILLER{7};

            HashMap<string, Foo> usesDefaults{};

            auto& filler = usesDefaults[FILLER];

            Assert::AreSame(filler, usesDefaults[FILLER]);

            HashMap<Foo, string> alsoUsesDefaults{};

            auto& thriller = alsoUsesDefaults[THRILLER];

            Assert::AreSame(thriller, alsoUsesDefaults[THRILLER]);
        }

        TEST_METHOD(InsertVsInsertOrAssign) {
            using map_type = HashMap<string, Foo>;
            using size_type = map_type::size_type;

            const string KEY = "KEY"s;
            const string KEYKEY = "KEY KEY"s;
            const Foo BEFORE{8};
            const Foo AFTER{9};

            map_type map{};

            map_type::value_type pairBefore{std::make_pair(KEY, BEFORE)};
            auto insertedBeforePosition = map.Insert(pairBefore);
            Foo insertedBefore = insertedBeforePosition->second;

            map_type::value_type pairAfter{std::make_pair(KEY, AFTER)};
            auto insertedAfterPosition = map.Insert(pairAfter);
            Foo insertedAfter = insertedAfterPosition->second;

            Assert::AreEqual(size_type(1), map.Size());
            Assert::AreEqual(size_type(1), map.UsedHashValueCount());
            Assert::AreEqual(insertedBeforePosition, insertedAfterPosition);
            Assert::AreNotSame(insertedBeforePosition, insertedAfterPosition);
            Assert::AreEqual(insertedBefore, insertedAfter);
            Assert::AreNotSame(insertedBefore, insertedAfter);
            Assert::AreSame(*insertedBeforePosition, *insertedAfterPosition);
            Assert::AreSame(insertedBeforePosition->second, insertedAfterPosition->second);

            auto insertedOrAssignedAfterPosition = map.InsertOrAssign(pairAfter);
            Foo insertedOrAssignedAfter = insertedOrAssignedAfterPosition->second;

            Assert::AreEqual(size_type(1), map.Size());
            Assert::AreEqual(size_type(1), map.UsedHashValueCount());
            Assert::AreEqual(insertedBeforePosition, insertedAfterPosition);
            Assert::AreEqual(insertedBeforePosition, insertedOrAssignedAfterPosition);
            Assert::AreEqual(insertedAfterPosition, insertedOrAssignedAfterPosition);
            Assert::AreEqual(insertedBefore, insertedAfter);
            Assert::AreNotEqual(insertedBefore, insertedOrAssignedAfter);
            Assert::AreNotEqual(insertedAfter, insertedOrAssignedAfter);
            Assert::AreSame(*insertedBeforePosition, *insertedAfterPosition);
            Assert::AreSame(insertedBeforePosition->second, insertedAfterPosition->second);
            Assert::AreSame(*insertedBeforePosition, *insertedOrAssignedAfterPosition);
            Assert::AreSame(insertedBeforePosition->second, insertedOrAssignedAfterPosition->second);
            Assert::AreSame(*insertedAfterPosition, *insertedOrAssignedAfterPosition);
            Assert::AreSame(insertedAfterPosition->second, insertedOrAssignedAfterPosition->second);

            map_type::value_type pairKeykey{std::make_pair(KEYKEY, BEFORE)};
            map.InsertOrAssign(pairKeykey);

            Assert::AreEqual(size_type(2), map.Size());
            Assert::AreEqual(size_type(2), map.UsedHashValueCount());
        }

        TEST_METHOD(Remove) {
            using key_type = std::size_t;
            using size_type = HashMap<key_type, Foo>::size_type;

            const key_type OFFSET = key_type(17);
            const Foo FIRST{10};
            const Foo SECOND{11};

            HashMap<key_type, Foo> map{SMALL_MAX_HASH_VAL};

            map.InsertOrAssign(std::make_pair(OFFSET, FIRST));
            map.InsertOrAssign(std::make_pair(OFFSET + SMALL_MAX_HASH_VAL, SECOND));

            Assert::AreEqual(size_type(2), map.Size());
            Assert::AreEqual(size_type(1), map.UsedHashValueCount());
            Assert::AreEqual(FIRST, map.At(OFFSET));
            Assert::AreEqual(SECOND, const_cast<const HashMap<key_type, Foo>&>(map).At(OFFSET + SMALL_MAX_HASH_VAL));

            Assert::IsTrue(map.Remove(OFFSET));

            Assert::ExpectException<std::out_of_range>([&map, &OFFSET](){
                const auto& tmp = const_cast<const HashMap<key_type, Foo>&>(map).At(OFFSET);
                UNREFERENCED_LOCAL(tmp);
            });
            Assert::AreEqual(SECOND, map.CAt(OFFSET + SMALL_MAX_HASH_VAL));
            Assert::AreEqual(size_type(1), map.Size());
            Assert::AreEqual(size_type(1), map.UsedHashValueCount());

            Assert::IsFalse(map.Remove(OFFSET));
            Assert::IsTrue(map.Remove(OFFSET + SMALL_MAX_HASH_VAL));

            Assert::ExpectException<std::out_of_range>([&map, &OFFSET](){ const auto& tmp = map.CAt(OFFSET); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&map, &OFFSET](){ auto& tmp = map.At(OFFSET + SMALL_MAX_HASH_VAL); UNREFERENCED_LOCAL(tmp); });
            Assert::AreEqual(size_type(0), map.Size());
            Assert::AreEqual(size_type(0), map.UsedHashValueCount());

            Assert::IsFalse(map.Remove(OFFSET));
            Assert::IsFalse(map.Remove(OFFSET + SMALL_MAX_HASH_VAL));
        }

        TEST_METHOD(Clear) {
            using size_type = HashMap<string, Foo>::size_type;

            const string FIRST = "FIRST"s;
            const string SECOND = "SECOND"s;
            const string THIRD = "THIRD"s;
            const Foo FILLER{12};

            HashMap<string, Foo> map{};

            map.Insert(std::make_pair(FIRST, FILLER));
            map.Insert(std::make_pair(SECOND, FILLER));
            map.Insert(std::make_pair(THIRD, FILLER));

            auto filledSize = map.Size();
            auto filledMaxHashValue = map.MaxHashValue();
            auto filledUsedHashValueCount = map.UsedHashValueCount();

            Assert::AreNotEqual(size_type(0), filledSize);
            Assert::AreNotEqual(size_type(0), filledUsedHashValueCount);

            map.Clear();

            Assert::AreNotEqual(filledSize, map.Size());
            Assert::AreNotEqual(filledUsedHashValueCount, map.UsedHashValueCount());
            Assert::AreEqual(filledMaxHashValue, map.MaxHashValue());
        }

        TEST_METHOD(Copy) {
            using size_type = HashMap<string, Foo>::size_type;

            const string FIRST_KEY = "FIRST"s;
            const string SECOND_KEY = "SECOND"s;
            const string THIRD_KEY = "THIRD"s;
            const string FOURTH_KEY = "FOURTH"s;
            const string FIFTH_KEY = "FIFTH"s;
            const Foo FIRST_DATA{13};
            const Foo SECOND_DATA{14};
            const Foo THIRD_DATA{15};
            const Foo FOURTH_DATA{16};
            const Foo FIFTH_DATA{17};

            HashMap<string, Foo> original{};

            original.Insert(std::make_pair(FIRST_KEY, FIRST_DATA));
            original.Insert(std::make_pair(SECOND_KEY, SECOND_DATA));
            original.Insert(std::make_pair(THIRD_KEY, THIRD_DATA));
            original.Insert(std::make_pair(FOURTH_KEY, FOURTH_DATA));
            original.Insert(std::make_pair(FIFTH_KEY, FIFTH_DATA));

            HashMap<string, Foo> constructed{original};

            Assert::AreEqual(size_type(5), original.Size());
            Assert::AreEqual(original.Size(), constructed.Size());
            Assert::AreEqual(original.MaxHashValue(), constructed.MaxHashValue());
            Assert::AreEqual(original.UsedHashValueCount(), constructed.UsedHashValueCount());
            Assert::AreEqual(original.At(FIRST_KEY), constructed.At(FIRST_KEY));
            Assert::AreNotSame(original.At(FIRST_KEY), constructed.At(FIRST_KEY));
            Assert::AreEqual(original.CAt(SECOND_KEY), constructed.At(SECOND_KEY));
            Assert::AreNotSame(original.CAt(SECOND_KEY), constructed.At(SECOND_KEY));
            Assert::AreEqual(original.At(THIRD_KEY), constructed.CAt(THIRD_KEY));
            Assert::AreNotSame(original.At(THIRD_KEY), constructed.CAt(THIRD_KEY));
            Assert::AreEqual(original.CAt(FOURTH_KEY), constructed.CAt(FOURTH_KEY));
            Assert::AreNotSame(original.CAt(FOURTH_KEY), constructed.CAt(FOURTH_KEY));
            Assert::AreEqual(original.At(FIFTH_KEY), constructed.At(FIFTH_KEY));
            Assert::AreNotSame(original.At(FIFTH_KEY), constructed.At(FIFTH_KEY));

            HashMap<string, Foo> assigned{};
            assigned = original;

            Assert::AreEqual(size_type(5), original.Size());
            Assert::AreEqual(original.Size(), assigned.Size());
            Assert::AreEqual(constructed.Size(), assigned.Size());
            Assert::AreEqual(original.MaxHashValue(), assigned.MaxHashValue());
            Assert::AreEqual(constructed.MaxHashValue(), assigned.MaxHashValue());
            Assert::AreEqual(original.UsedHashValueCount(), assigned.UsedHashValueCount());
            Assert::AreEqual(constructed.UsedHashValueCount(), assigned.UsedHashValueCount());
            Assert::AreEqual(original.At(FIRST_KEY), assigned.At(FIRST_KEY));
            Assert::AreNotSame(original.At(FIRST_KEY), assigned.At(FIRST_KEY));
            Assert::AreEqual(constructed.At(FIRST_KEY), assigned.At(FIRST_KEY));
            Assert::AreNotSame(constructed.At(FIRST_KEY), assigned.At(FIRST_KEY));
            Assert::AreEqual(original.CAt(SECOND_KEY), assigned.At(SECOND_KEY));
            Assert::AreNotSame(original.CAt(SECOND_KEY), assigned.At(SECOND_KEY));
            Assert::AreEqual(constructed.CAt(SECOND_KEY), assigned.At(SECOND_KEY));
            Assert::AreNotSame(constructed.CAt(SECOND_KEY), assigned.At(SECOND_KEY));
            Assert::AreEqual(original.At(THIRD_KEY), assigned.CAt(THIRD_KEY));
            Assert::AreNotSame(original.At(THIRD_KEY), assigned.CAt(THIRD_KEY));
            Assert::AreEqual(constructed.At(THIRD_KEY), assigned.CAt(THIRD_KEY));
            Assert::AreNotSame(constructed.At(THIRD_KEY), assigned.CAt(THIRD_KEY));
            Assert::AreEqual(original.CAt(FOURTH_KEY), assigned.CAt(FOURTH_KEY));
            Assert::AreNotSame(original.CAt(FOURTH_KEY), assigned.CAt(FOURTH_KEY));
            Assert::AreEqual(constructed.CAt(FOURTH_KEY), assigned.CAt(FOURTH_KEY));
            Assert::AreNotSame(constructed.CAt(FOURTH_KEY), assigned.CAt(FOURTH_KEY));
            Assert::AreEqual(original.At(FIFTH_KEY), assigned.At(FIFTH_KEY));
            Assert::AreNotSame(original.At(FIFTH_KEY), assigned.At(FIFTH_KEY));
            Assert::AreEqual(constructed.At(FIFTH_KEY), assigned.At(FIFTH_KEY));
            Assert::AreNotSame(constructed.At(FIFTH_KEY), assigned.At(FIFTH_KEY));
        }

        TEST_METHOD(Move) {
            using size_type = HashMap<string, Foo>::size_type;

            const string FIRST_KEY = "FIRST"s;
            const string SECOND_KEY = "SECOND"s;
            const string THIRD_KEY = "THIRD"s;
            const string FOURTH_KEY = "FOURTH"s;
            const string FIFTH_KEY = "FIFTH"s;
            const Foo FIRST_DATA{18};
            const Foo SECOND_DATA{19};
            const Foo THIRD_DATA{20};
            const Foo FOURTH_DATA{21};
            const Foo FIFTH_DATA{22};

            HashMap<string, Foo> original{};

            original.Insert(std::make_pair(FIRST_KEY, FIRST_DATA));
            original.Insert(std::make_pair(SECOND_KEY, SECOND_DATA));
            original.Insert(std::make_pair(THIRD_KEY, THIRD_DATA));
            original.Insert(std::make_pair(FOURTH_KEY, FOURTH_DATA));
            original.Insert(std::make_pair(FIFTH_KEY, FIFTH_DATA));

            auto originalSize = original.Size();
            auto originalMaxHashValue = original.MaxHashValue();
            auto originalUsedHashValueCount = original.UsedHashValueCount();

            auto& firstData = original.At(FIRST_KEY);
            auto& secondData = original.At(SECOND_KEY);
            auto& thirdData = original.At(THIRD_KEY);
            auto& fourthData = original.At(FOURTH_KEY);
            auto& fifthData = original.At(FIFTH_KEY);

            auto firstPosition = original.Find(FIRST_KEY);
            auto secondPosition = original.Find(SECOND_KEY);
            auto thirdPosition = original.Find(THIRD_KEY);
            auto fourthPosition = original.Find(FOURTH_KEY);
            auto fifthPosition = original.Find(FIFTH_KEY);

            HashMap<string, Foo> constructed{std::move(original)};

            Assert::AreEqual(originalSize, constructed.Size());
            Assert::AreEqual(originalMaxHashValue, constructed.MaxHashValue());
            Assert::AreEqual(originalUsedHashValueCount, constructed.UsedHashValueCount());
            Assert::AreSame(firstData, constructed.At(FIRST_KEY));
            Assert::AreSame(secondData, constructed.At(SECOND_KEY));
            Assert::AreSame(thirdData, constructed.At(THIRD_KEY));
            Assert::AreSame(fourthData, constructed.At(FOURTH_KEY));
            Assert::AreSame(fifthData, constructed.At(FIFTH_KEY));
            Assert::AreSame(*firstPosition, *(constructed.Find(FIRST_KEY)));
            Assert::AreSame(*secondPosition, *(constructed.Find(SECOND_KEY)));
            Assert::AreSame(*thirdPosition, *(constructed.Find(THIRD_KEY)));
            Assert::AreSame(*fourthPosition, *(constructed.Find(FOURTH_KEY)));
            Assert::AreSame(*fifthPosition, *(constructed.Find(FIFTH_KEY)));

            HashMap<string, Foo> assigned{};
            assigned = std::move(constructed);

            Assert::AreEqual(originalSize, assigned.Size());
            Assert::AreEqual(originalMaxHashValue, assigned.MaxHashValue());
            Assert::AreEqual(originalUsedHashValueCount, assigned.UsedHashValueCount());
            Assert::AreSame(firstData, assigned.At(FIRST_KEY));
            Assert::AreSame(secondData, assigned.At(SECOND_KEY));
            Assert::AreSame(thirdData, assigned.At(THIRD_KEY));
            Assert::AreSame(fourthData, assigned.At(FOURTH_KEY));
            Assert::AreSame(fifthData, assigned.At(FIFTH_KEY));
            Assert::AreSame(*firstPosition, *(assigned.Find(FIRST_KEY)));
            Assert::AreSame(*secondPosition, *(assigned.Find(SECOND_KEY)));
            Assert::AreSame(*thirdPosition, *(assigned.Find(THIRD_KEY)));
            Assert::AreSame(*fourthPosition, *(assigned.Find(FOURTH_KEY)));
            Assert::AreSame(*fifthPosition, *(assigned.Find(FIFTH_KEY)));
        }

        TEST_METHOD(RehashIfNeededDefaults) {
            using size_type = HashMap<std::size_t, string>::size_type;

            const string FILLER = "FILLER"s;

            HashMap<std::size_t, string> map{size_type(3)};

            map.Insert(std::make_pair(std::size_t(6), FILLER));

            Assert::IsFalse(map.RehashIfNeeded());

            map.Insert(std::make_pair(std::size_t(14), FILLER));
            auto firstSize = map.Size();

            Assert::IsTrue(map.RehashIfNeeded());
            Assert::AreEqual(firstSize, map.Size());
            Assert::AreEqual(size_type(7), map.MaxHashValue());

            map.Insert(std::make_pair(std::size_t(21), FILLER));
            map.Insert(std::make_pair(std::size_t(76), FILLER));
            map.Insert(std::make_pair(std::size_t(2), FILLER));
            map.Insert(std::make_pair(std::size_t(8), FILLER));
            map.Insert(std::make_pair(std::size_t(141), FILLER));
            map.Insert(std::make_pair(std::size_t(249), FILLER));
            map.Insert(std::make_pair(std::size_t(10), FILLER));
            map.Insert(std::make_pair(std::size_t(17), FILLER));
            map.Insert(std::make_pair(std::size_t(24), FILLER));
            map.Insert(std::make_pair(std::size_t(250), FILLER));
            map.Insert(std::make_pair(std::size_t(35), FILLER));
            map.Insert(std::make_pair(std::size_t(5), FILLER));
            map.Insert(std::make_pair(std::size_t(52), FILLER));
            map.Insert(std::make_pair(std::size_t(12), FILLER));
            auto secondSize = map.Size();

            Assert::IsTrue(map.RehashIfNeeded());
            Assert::AreEqual(secondSize, map.Size());
            Assert::AreNotEqual(size_type(13), map.MaxHashValue());
            Assert::AreEqual(size_type(23), map.MaxHashValue());
        }

        TEST_METHOD(RehashIfNeededCustomRehash) {
            using size_type = HashMap<std::size_t, string>::size_type;

            struct CustomRehash {
                size_type operator()(size_type current) const { return current + current; }
            } customRehash{};

            const string FILLER = "FILLER"s;

            HashMap<std::size_t, string> map{SMALL_MAX_HASH_VAL};

            for (size_type i = size_type(0); i < SMALL_MAX_HASH_VAL; ++i) {
                map.Insert(std::make_pair(i + (SMALL_MAX_HASH_VAL * i), FILLER));
            }

            Assert::IsTrue(map.RehashIfNeeded(customRehash));
            Assert::AreEqual(SMALL_MAX_HASH_VAL + SMALL_MAX_HASH_VAL, map.MaxHashValue());
        }

        TEST_METHOD(RehashIfNeededCustomCondition) {
            using map_type = HashMap<std::size_t, string>;
            using size_type = map_type::size_type;

            const string FILLER = "FILLER"s;

            map_type map{SMALL_MAX_HASH_VAL};

            for (size_type i = size_type(0); i < SMALL_MAX_HASH_VAL; ++i) {
                map.Insert(std::make_pair(i + (SMALL_MAX_HASH_VAL * i), FILLER));
            }

            auto preMaxHashValue = map.MaxHashValue();

            Assert::IsTrue(map.RehashIfNeeded(map_type::DefaultRehash{}, [&map](std::size_t, std::size_t){ return map.LoadFactor() > 0.7; }));
            Assert::AreNotEqual(preMaxHashValue, map.MaxHashValue());
            Assert::IsTrue(preMaxHashValue < map.MaxHashValue());
        }

        TEST_METHOD(Swap) {
            using std::swap;

            const string ONE_A = "ONE A"s;
            const string ONE_B = "ONE B"s;
            const string TWO_A = "TWO A"s;
            const Foo FILLER{23};

            HashMap<string, Foo> one{};
            HashMap<string, Foo> two{SMALL_MAX_HASH_VAL};

            one.Insert(std::make_pair(ONE_A, FILLER));
            one.Insert(std::make_pair(ONE_B, FILLER));
            two.Insert(std::make_pair(TWO_A, FILLER));

            auto oneSizePreSwap = one.Size();
            auto oneMaxHashValuePreSwap = one.MaxHashValue();
            auto oneUsedHashValueCountPreSwap = one.UsedHashValueCount();
            auto oneBeginValuePreSwap = one.begin()->second;
            auto twoSizePreSwap = two.Size();
            auto twoMaxHashValuePreSwap = two.MaxHashValue();
            auto twoUsedHashValueCountPreSwap = two.UsedHashValueCount();
            auto twoBeginValuePreSwap = two.begin()->second;

            one.swap(two);

            Assert::AreEqual(oneSizePreSwap, two.Size());
            Assert::AreEqual(oneMaxHashValuePreSwap, two.MaxHashValue());
            Assert::AreEqual(oneUsedHashValueCountPreSwap, two.UsedHashValueCount());
            Assert::AreEqual(oneBeginValuePreSwap, two.begin()->second);
            Assert::AreEqual(twoSizePreSwap, one.Size());
            Assert::AreEqual(twoMaxHashValuePreSwap, one.MaxHashValue());
            Assert::AreEqual(twoUsedHashValueCountPreSwap, one.UsedHashValueCount());
            Assert::AreEqual(twoBeginValuePreSwap, one.begin()->second);

            swap(one, two);

            Assert::AreEqual(oneSizePreSwap, one.Size());
            Assert::AreEqual(oneMaxHashValuePreSwap, one.MaxHashValue());
            Assert::AreEqual(oneUsedHashValueCountPreSwap, one.UsedHashValueCount());
            Assert::AreEqual(oneBeginValuePreSwap, one.begin()->second);
            Assert::AreEqual(twoSizePreSwap, two.Size());
            Assert::AreEqual(twoMaxHashValuePreSwap, two.MaxHashValue());
            Assert::AreEqual(twoUsedHashValueCountPreSwap, two.UsedHashValueCount());
            Assert::AreEqual(twoBeginValuePreSwap, two.begin()->second);
        }

        TEST_METHOD(ForceRehashDefault) {
            using size_type = HashMap<std::size_t, string>::size_type;

            const string FILLER = "FILLER"s;

            HashMap<std::size_t, string> map{size_type(3)};

            map.Insert(std::make_pair(std::size_t(6), FILLER));
            map.Insert(std::make_pair(std::size_t(14), FILLER));
            auto firstSize = map.Size();

            Assert::IsTrue(map.LoadFactor() > 0.5);

            map.ForceRehash();

            Assert::AreEqual(firstSize, map.Size());
            Assert::AreEqual(size_type(7), map.MaxHashValue());

            map.Insert(std::make_pair(std::size_t(21), FILLER));
            map.Insert(std::make_pair(std::size_t(76), FILLER));
            map.Insert(std::make_pair(std::size_t(2), FILLER));
            map.Insert(std::make_pair(std::size_t(8), FILLER));
            map.Insert(std::make_pair(std::size_t(141), FILLER));
            map.Insert(std::make_pair(std::size_t(249), FILLER));
            map.Insert(std::make_pair(std::size_t(10), FILLER));
            map.Insert(std::make_pair(std::size_t(17), FILLER));
            map.Insert(std::make_pair(std::size_t(24), FILLER));
            map.Insert(std::make_pair(std::size_t(250), FILLER));
            map.Insert(std::make_pair(std::size_t(35), FILLER));
            map.Insert(std::make_pair(std::size_t(5), FILLER));
            map.Insert(std::make_pair(std::size_t(52), FILLER));
            map.Insert(std::make_pair(std::size_t(12), FILLER));
            auto secondSize = map.Size();

            Assert::IsTrue(map.LoadFactor() > 0.75);

            map.ForceRehash();

            Assert::AreEqual(secondSize, map.Size());
            Assert::AreEqual(size_type(13), map.MaxHashValue());
            Assert::IsTrue(map.LoadFactor() > 0.75);

            map.ForceRehash();

            Assert::AreEqual(size_type(23), map.MaxHashValue());
            Assert::IsFalse(map.LoadFactor() > 0.75);
        }

        TEST_METHOD(ForceRehashCustom) {
            using size_type = HashMap<std::size_t, string>::size_type;

            struct CustomRehash {
                size_type operator()(size_type current) const { return current + current; }
            } customRehash{};

            const string FILLER = "FILLER"s;

            HashMap<std::size_t, string> map{SMALL_MAX_HASH_VAL};

            for (size_type i = size_type(0); i < SMALL_MAX_HASH_VAL; ++i) {
                map.Insert(std::make_pair(i + (SMALL_MAX_HASH_VAL * i), FILLER));
            }

            map.ForceRehash(customRehash);

            Assert::AreEqual(SMALL_MAX_HASH_VAL + SMALL_MAX_HASH_VAL, map.MaxHashValue());
            Assert::IsTrue(map.LoadFactor() < 0.505);
        }

        TEST_METHOD(ForceRehashSetValue) {
            using map_type = HashMap<std::size_t, string>;
            using size_type = map_type::size_type;

            struct CustomRehash {
                size_type operator()(size_type current) const { return current + current; }
            } customRehash{};

            const string FILLER = "FILLER"s;

            map_type map{SMALL_MAX_HASH_VAL};

            for (size_type i = size_type(0); i < SMALL_MAX_HASH_VAL; ++i) {
                map.Insert(std::make_pair(i + (SMALL_MAX_HASH_VAL * i), FILLER));
            }

            Assert::ExpectException<std::invalid_argument>([&map](){ return map.ForceRehash(size_type(0)); });
            Assert::ExpectException<std::invalid_argument>([&map](){ return map.ForceRehash(size_type(1)); });

            map.ForceRehash(size_type(3));

            Assert::AreEqual(size_type(3), map.MaxHashValue());
            Assert::IsTrue(map.LoadFactor() > 0.999);

            map.ForceRehash(map_type::DEFAULT_MAX_HASH_VALUE);

            Assert::AreEqual(map_type::DEFAULT_MAX_HASH_VALUE, map.MaxHashValue());
            Assert::IsTrue(map.LoadFactor() < 0.5);
        }

        TEST_METHOD(InsertVsInsertOrAssignRValue) {
            using size_type = HashMap<string, Foo>::size_type;

            const string KEY = "KEY"s;
            const string KEYKEY = "KEY KEY"s;
            const Foo BEFORE{24};
            const Foo AFTER{25};

            HashMap<string, Foo> map{};

            auto insertedBeforePosition = map.Insert(std::move(std::make_pair(KEY, BEFORE)));
            Foo insertedBefore = insertedBeforePosition->second;

            auto insertedAfterPosition = map.Insert(std::move(std::make_pair(KEY, AFTER)));
            Foo insertedAfter = insertedAfterPosition->second;

            Assert::AreEqual(size_type(1), map.Size());
            Assert::AreEqual(size_type(1), map.UsedHashValueCount());
            Assert::AreEqual(insertedBeforePosition, insertedAfterPosition);
            Assert::AreNotSame(insertedBeforePosition, insertedAfterPosition);
            Assert::AreEqual(insertedBefore, insertedAfter);
            Assert::AreNotSame(insertedBefore, insertedAfter);
            Assert::AreSame(*insertedBeforePosition, *insertedAfterPosition);
            Assert::AreSame(insertedBeforePosition->second, insertedAfterPosition->second);

            auto insertedOrAssignedAfterPosition = map.InsertOrAssign(std::move(std::make_pair(KEY, AFTER)));
            Foo insertedOrAssignedAfter = insertedOrAssignedAfterPosition->second;

            Assert::AreEqual(size_type(1), map.Size());
            Assert::AreEqual(size_type(1), map.UsedHashValueCount());
            Assert::AreEqual(insertedBeforePosition, insertedAfterPosition);
            Assert::AreEqual(insertedBeforePosition, insertedOrAssignedAfterPosition);
            Assert::AreEqual(insertedAfterPosition, insertedOrAssignedAfterPosition);
            Assert::AreEqual(insertedBefore, insertedAfter);
            Assert::AreNotEqual(insertedBefore, insertedOrAssignedAfter);
            Assert::AreNotEqual(insertedAfter, insertedOrAssignedAfter);
            Assert::AreSame(*insertedBeforePosition, *insertedAfterPosition);
            Assert::AreSame(insertedBeforePosition->second, insertedAfterPosition->second);
            Assert::AreSame(*insertedBeforePosition, *insertedOrAssignedAfterPosition);
            Assert::AreSame(insertedBeforePosition->second, insertedOrAssignedAfterPosition->second);
            Assert::AreSame(*insertedAfterPosition, *insertedOrAssignedAfterPosition);
            Assert::AreSame(insertedAfterPosition->second, insertedOrAssignedAfterPosition->second);

            map.InsertOrAssign(std::move(std::make_pair(KEYKEY, BEFORE)));

            Assert::AreEqual(size_type(2), map.Size());
            Assert::AreEqual(size_type(2), map.UsedHashValueCount());
        }

        TEST_METHOD(InitializerListSemantics) {
            using size_type = HashMap<string, Foo>::size_type;

            const string S_ONE = "1"s;
            const string S_TWO = "2"s;
            const string S_THREE = "3"s;
            const string S_FOUR = "4"s;
            const string S_FIVE = "5"s;

            const Foo F_ONE{1};
            const Foo F_TWO{2};
            const Foo F_THREE{3};
            const Foo F_FOUR{4};
            const Foo F_FIVE{4};

            HashMap<string, Foo> listed{{S_ONE, F_ONE}, {S_TWO, F_TWO}, {S_THREE, F_THREE}};
            const decltype(listed)& cref = listed;

            Assert::AreEqual(size_type(3), cref.Size());
            Assert::AreEqual(F_ONE, cref.Find(S_ONE)->second);
            Assert::AreEqual(F_TWO, cref[S_TWO]);
            Assert::AreEqual(F_THREE, listed.Find(S_THREE)->second);

            listed = {{S_FOUR, F_FOUR}, {S_FIVE, F_FIVE}};

            Assert::AreEqual(size_type(2), cref.Size());
            Assert::AreEqual(F_FOUR, cref.Find(S_FOUR)->second);
            Assert::AreEqual(F_FIVE, cref[S_FIVE]);
        }
    };
}
