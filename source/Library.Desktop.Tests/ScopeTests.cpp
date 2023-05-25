#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "Scope.h"
#include "Bar.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std::literals::string_literals;

namespace LibraryDesktopTests {
    TEST_CLASS(ScopeTests) {

    private:
        inline static _CrtMemState _startMemState;

        using size_type = Scope::size_type;
        using key_type = Scope::key_type;

        using Integer = Datum::Integer;
        using Float = Datum::Float;
        using String = Datum::String;
        using Vector = Datum::Vector;
        using Matrix = Datum::Matrix;
        using Pointer = Datum::Pointer;
        using DatumType = Datum::DatumType;

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
            const key_type TEST = "TEST"s;

            Scope scope{};

            Assert::AreEqual(size_type(0), scope.Size());
            Assert::IsTrue(scope.IsEmpty());
            Assert::IsNull(scope.Parent());
            Assert::ExpectException<std::out_of_range>([&scope](){ auto& tmp = scope.At(size_type(0)); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&scope](){ auto& tmp = scope.CAt(size_type(0)); UNREFERENCED_LOCAL(tmp); });
            Assert::AreEqual(scope.begin(), scope.end());
            Assert::AreEqual(scope.cbegin(), scope.cend());

            Assert::ExpectException<std::out_of_range>([&scope, &TEST](){ auto& tmp = scope.At(TEST); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&scope, &TEST](){ auto& tmp = scope.CAt(TEST); UNREFERENCED_LOCAL(tmp); });

            scope[TEST];

            Assert::AreEqual(size_type(1), scope.Size());

            Datum& datum = scope[TEST];

            Assert::AreEqual(size_type(1), scope.Size());
            Assert::AreSame(datum, scope.At(TEST));
            Assert::AreSame(datum, scope.CAt(TEST));
            Assert::AreSame(datum, scope.At(size_type(0)));
            Assert::AreSame(datum, scope.CAt(size_type(0)));

            const Scope cscope{};

            Assert::ExpectException<std::out_of_range>([&cscope](){ auto& tmp = cscope.At(size_type(0)); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&cscope, &TEST](){ auto& tmp = cscope.At(TEST); UNREFERENCED_LOCAL(tmp); });
        }

        TEST_METHOD(AppendNonScopeAndIndex) {
            const key_type NAME = "Name"s;
            const key_type LEVEL = "Level"s;
            const key_type HEALTH = "Health"s;
            const key_type POSITION = "Position"s;
            const key_type FOOS = "Foos";

            Scope scope{};

            Assert::AreEqual(size_type(0), scope.Size());

            scope.Append(NAME);

            Assert::AreEqual(size_type(1), scope.Size());
            Assert::AreEqual(DatumType::Unknown, scope[NAME].ApparentType());

            scope[NAME] = "Character"s;

            Assert::AreEqual(DatumType::String, scope[NAME].ApparentType());
            Assert::AreEqual(size_type(1), scope[NAME].Size());

            scope[LEVEL] = 99;

            Assert::AreEqual(size_type(2), scope.Size());
            Assert::AreEqual(DatumType::Integer, scope[LEVEL].ApparentType());
            Assert::AreNotSame(scope[size_type(0)], scope[LEVEL]);
            Assert::AreSame(scope[size_type(0)], scope[NAME]);
            Assert::AreSame(scope[size_type(1)], scope[LEVEL]);

            Datum& health = scope[HEALTH];
            health = { 100.f, 100.f };

            Assert::AreEqual(size_type(3), scope.Size());
            Assert::AreSame(health, scope[HEALTH]);
            Assert::AreEqual(DatumType::Float, scope[HEALTH].ApparentType());
            Assert::AreEqual(size_type(2), scope[HEALTH].Size());
            Assert::AreNotSame(scope[size_type(1)], scope[HEALTH]);
            Assert::AreSame(scope[size_type(1)], scope[LEVEL]);
            Assert::AreSame(scope[size_type(2)], scope[HEALTH]);

            Datum& position = scope[POSITION];
            position = Vector{ 1.f, 2.f, 3.f, 0.f };

            Assert::AreEqual(DatumType::Vector, scope[POSITION].ApparentType());

            Foo one{1};
            Foo two{2};
            Foo three{3};
            scope[FOOS] = { &one, &two, &three };

            Assert::AreEqual(size_type(5), scope.Size());
            Assert::AreEqual(DatumType::Pointer, scope[FOOS].ApparentType());
            Assert::AreEqual(size_type(3), scope[FOOS].Size());
        }

        TEST_METHOD(AppendScopeAndFind) {
            const key_type IN_PARENT = "In Parent"s;
            const key_type OTHER = "Other"s;
            const key_type IN_CHILD = "In Child"s;
            const key_type CHILD = "Child"s;

            const Integer IN_VALUE = 1;
            const Float OTHER_VALUE = 2.f;

            Scope parent{size_type(3)};
            parent.Append(IN_PARENT) = IN_VALUE;
            parent.Append(OTHER) = OTHER_VALUE;
            Scope& child = parent.AppendScope(CHILD, size_type(2));
            child.Append(IN_CHILD) = IN_VALUE;
            child.Append(OTHER) = OTHER_VALUE;

            Assert::AreEqual(parent.end(), parent.Find(IN_CHILD));
            Assert::AreEqual(child.end(), child.Find(IN_PARENT));
            Assert::AreNotEqual(parent.end(), parent.Find(OTHER));
            Assert::AreEqual(OTHER, parent.Find(OTHER)->first);
            Assert::AreEqual(OTHER_VALUE, parent.Find(OTHER)->second.CFrontFloat());
            Assert::AreNotEqual(child.end(), child.Find(OTHER));
            Assert::AreEqual(OTHER, child.Find(OTHER)->first);
            Assert::AreEqual(OTHER_VALUE, child.Find(OTHER)->second.CFrontFloat());
            Assert::AreNotSame(*(parent.Find(OTHER)), *(child.Find(OTHER)));
            Assert::AreNotEqual(parent.end(), parent.Find(IN_PARENT));
            Assert::AreEqual(IN_PARENT, parent.Find(IN_PARENT)->first);
            Assert::AreEqual(IN_VALUE, parent.Find(IN_PARENT)->second.CFrontInteger());
            Assert::AreNotEqual(child.end(), child.Find(IN_CHILD));
            Assert::AreEqual(IN_CHILD, child.Find(IN_CHILD)->first);
            Assert::AreEqual(IN_VALUE, child.Find(IN_CHILD)->second.CFrontInteger());

            const Scope& cparent = parent;
            const Scope& cchild = child;

            Assert::AreEqual(cparent.end(), cparent.Find(IN_CHILD));
            Assert::AreEqual(cchild.end(), cchild.Find(IN_PARENT));
            Assert::AreNotEqual(cparent.end(), cparent.Find(OTHER));
            Assert::AreNotEqual(cchild.end(), cchild.Find(OTHER));
            Assert::AreNotSame(*(cparent.Find(OTHER)), *(cchild.Find(OTHER)));
            Assert::AreNotEqual(cparent.end(), cparent.Find(IN_PARENT));
            Assert::AreNotEqual(cchild.end(), cchild.Find(IN_CHILD));
        }

        TEST_METHOD(AppendScopeAndSearch) {
            const key_type IN_NESTED_PARENT = "In Parent"s;
            const key_type NESTED_PARENT = "Parent"s;
            const key_type OTHER = "Other"s;
            const key_type IN_CHILD = "In Child"s;
            const key_type CHILD = "Child"s;
            const key_type BLANK = "Blank"s;

            const Integer IN_VALUE = 1;
            const Float OTHER_VALUE = 2.f;

            Scope root{size_type(2)};
            root.Append(BLANK);
            Scope& nestedParent = root.AppendScope(NESTED_PARENT, size_type(3));

            nestedParent.Append(IN_NESTED_PARENT) = IN_VALUE;
            nestedParent.Append(OTHER) = OTHER_VALUE;
            Scope& firstChild = nestedParent.AppendScope(CHILD, size_type(2));
            firstChild.Append(IN_CHILD) = IN_VALUE;
            firstChild.Append(OTHER) = OTHER_VALUE;
            Scope& secondChild = nestedParent.AppendScope(CHILD, size_type(1));
            secondChild.Append(BLANK);

            Scope* containingScope = nullptr;
            Datum* foundDatum = nestedParent.Search(IN_CHILD, containingScope);

            foundDatum = firstChild.Search(IN_NESTED_PARENT, containingScope);

            Assert::IsNotNull(foundDatum);
            Assert::AreEqual(IN_VALUE, foundDatum->CFrontInteger());
            Assert::AreEqual(&nestedParent, containingScope);

            foundDatum = firstChild.Search(IN_CHILD, containingScope);

            Assert::IsNotNull(foundDatum);
            Assert::AreEqual(IN_VALUE, foundDatum->CFrontInteger());
            Assert::AreEqual(&firstChild, containingScope);

            foundDatum = firstChild.Search(OTHER, containingScope);

            Assert::IsNotNull(foundDatum);
            Assert::AreEqual(OTHER_VALUE, foundDatum->CFrontFloat());
            Assert::AreEqual(&firstChild, containingScope);

            foundDatum = firstChild.Search(BLANK, containingScope);

            Assert::IsNotNull(foundDatum);
            Assert::AreEqual(DatumType::Unknown, foundDatum->ApparentType());
            Assert::AreEqual(&root, containingScope);

            foundDatum = secondChild.Search(BLANK, containingScope);

            Assert::IsNotNull(foundDatum);
            Assert::AreEqual(DatumType::Unknown, foundDatum->ApparentType());
            Assert::AreEqual(&secondChild, containingScope);

            foundDatum = secondChild.Search(CHILD, containingScope);

            Assert::IsNotNull(foundDatum);
            Assert::AreEqual(DatumType::Table, foundDatum->ApparentType());
            Assert::AreEqual(DatumType::InternalTable, foundDatum->ActualType());
            Assert::AreSame(firstChild, foundDatum->CFrontTable());
            Assert::AreSame(secondChild, foundDatum->CBackTable());
            Assert::AreEqual(&nestedParent, containingScope);

            foundDatum = secondChild.Search(OTHER, containingScope);

            Assert::IsNotNull(foundDatum);
            Assert::AreEqual(OTHER_VALUE, foundDatum->CFrontFloat());
            Assert::AreEqual(&nestedParent, containingScope);

            foundDatum = secondChild.Search(IN_CHILD, containingScope);

            Assert::IsNull(foundDatum);
            Assert::IsNull(containingScope);

            foundDatum = root.Search(IN_CHILD, containingScope);

            Assert::IsNull(foundDatum);
            Assert::IsNull(containingScope);

            foundDatum = root.Search(IN_NESTED_PARENT, containingScope);

            Assert::IsNull(foundDatum);
            Assert::IsNull(containingScope);

            foundDatum = root.Search(OTHER, containingScope);

            Assert::IsNull(foundDatum);
            Assert::IsNull(containingScope);

            foundDatum = root.Search(NESTED_PARENT, containingScope);

            Assert::IsNotNull(foundDatum);
            Assert::AreEqual(DatumType::Table, foundDatum->ApparentType());
            Assert::AreEqual(DatumType::InternalTable, foundDatum->ActualType());
            Assert::AreSame(nestedParent, foundDatum->CFrontTable());
            Assert::AreEqual(&root, containingScope);

            const Scope& croot = root;
            const Scope& cnestedParent = nestedParent;
            const Scope& cfirstChild = firstChild;
            const Scope& csecondChild = secondChild;

            Assert::IsNull(cnestedParent.Search(IN_CHILD));

            Assert::IsNotNull(cfirstChild.Search(IN_NESTED_PARENT));
            Assert::AreEqual(IN_VALUE, cfirstChild.Search(IN_NESTED_PARENT)->CFrontInteger());
            Assert::IsNotNull(cfirstChild.Search(IN_CHILD));
            Assert::AreEqual(IN_VALUE, cfirstChild.Search(IN_CHILD)->CFrontInteger());
            Assert::IsNotNull(cfirstChild.Search(OTHER));
            Assert::AreEqual(OTHER_VALUE, cfirstChild.Search(OTHER)->CFrontFloat());
            Assert::IsNotNull(cfirstChild.Search(BLANK));
            Assert::AreEqual(DatumType::Unknown, cfirstChild.Search(BLANK)->ApparentType());

            Assert::IsNotNull(csecondChild.Search(BLANK));
            Assert::AreEqual(DatumType::Unknown, csecondChild.Search(BLANK)->ApparentType());
            Assert::IsNotNull(csecondChild.Search(CHILD));
            Assert::AreEqual(DatumType::Table, csecondChild.Search(CHILD)->ApparentType());
            Assert::AreEqual(DatumType::InternalTable, csecondChild.Search(CHILD)->ActualType());
            Assert::AreSame(cfirstChild, csecondChild.Search(CHILD)->CFrontTable());
            Assert::AreSame(csecondChild, csecondChild.Search(CHILD)->CBackTable());
            Assert::IsNotNull(csecondChild.Search(OTHER));
            Assert::AreEqual(OTHER_VALUE, csecondChild.Search(OTHER)->CFrontFloat());
            Assert::IsNull(csecondChild.Search(IN_CHILD));

            Assert::IsNull(croot.Search(IN_CHILD));
            Assert::IsNull(croot.Search(IN_NESTED_PARENT));
            Assert::IsNull(croot.Search(OTHER));
            Assert::IsNotNull(croot.Search(NESTED_PARENT));
            Assert::AreEqual(DatumType::Table, croot.Search(NESTED_PARENT)->ApparentType());
            Assert::AreEqual(DatumType::InternalTable, croot.Search(NESTED_PARENT)->ActualType());
            Assert::AreSame(cnestedParent, croot.Search(NESTED_PARENT)->CFrontTable());
        }

        TEST_METHOD(DetachProtectsFromClear) {
            const key_type VALUES = "Values"s;
            const key_type CHILD = "Child"s;

            Scope scope{};

            scope.Append(VALUES) = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
            Scope& child = scope.AppendScope(CHILD);

            child.Append(VALUES) = { 1.f, 2.f, 3.f, 4.f, 5.f };

            Assert::IsFalse(scope.IsEmpty());
            Assert::IsFalse(child.IsEmpty());

            child.DetachFromTree();

            scope.Clear();

            Assert::IsTrue(scope.IsEmpty());
            Assert::IsFalse(child.IsEmpty());
        }

        TEST_METHOD(CopySemanticsAndEquals) {
            const key_type NAME_KEY = "Name"s;
            const key_type TRANSFORM_KEY = "Transform"s;
            const key_type CHILDREN_KEY = "Children"s;
            const key_type CONTEXT_KEY = "Context"s;
            const key_type RANGE_KEY = "Range"s;

            const String CHARACTER_NAME = "Character"s;
            const String STATS_NAME = "Stats"s;
            const String POWER_NAME = "Power"s;
            const String TOUGHNESS_NAME = "Toughness"s;
            const String SIZE_CONTEXT = "Size"s;
            const String VALUE_CONTEXT = "Value"s;

            Scope root{};
            Scope* scope = &root;

            scope->Append(NAME_KEY) = CHARACTER_NAME;
            scope->Append(TRANSFORM_KEY) = Matrix{};
            scope->Append(RANGE_KEY) = { 0.f, 100.f };

            Scope* child = new Bar{2};
            child->Append(NAME_KEY) = STATS_NAME;
            child->Append(CONTEXT_KEY) = SIZE_CONTEXT;

            scope->AttachAsChild(CHILDREN_KEY, child->Clone());
            scope = &(scope->At(CHILDREN_KEY).BackTable());

            delete child;
            child = new Bar{4};
            child->Append(NAME_KEY) = POWER_NAME;
            child->Append(CONTEXT_KEY) = VALUE_CONTEXT;

            scope->AttachAsChild(CHILDREN_KEY, child->Clone());

            delete child;
            child = new Bar{5};
            child->Append(NAME_KEY) = TOUGHNESS_NAME;
            child->Append(CONTEXT_KEY) = VALUE_CONTEXT;

            scope->AttachAsChild(CHILDREN_KEY, child->Clone());

            delete child;
            Scope copy{root};

            Assert::AreEqual(root, copy);
            Assert::AreNotSame(root, copy);
            Assert::IsTrue(root.Equals(&copy));
            Assert::AreEqual(root.Size(), copy.Size());
            Assert::AreEqual(root[NAME_KEY], copy[NAME_KEY]);
            Assert::AreNotSame(root[NAME_KEY], copy[NAME_KEY]);
            Assert::AreEqual(root[TRANSFORM_KEY], copy[TRANSFORM_KEY]);
            Assert::AreNotSame(root[TRANSFORM_KEY], copy[TRANSFORM_KEY]);
            Assert::AreEqual(root[RANGE_KEY], copy[RANGE_KEY]);
            Assert::AreNotSame(root[RANGE_KEY], copy[RANGE_KEY]);
            Assert::IsTrue(copy[CHILDREN_KEY][size_type(0)].Is(Bar::TypeIdClass()));
            Assert::AreEqual(root[CHILDREN_KEY], copy[CHILDREN_KEY]);
            Assert::AreNotSame(root[CHILDREN_KEY], copy[CHILDREN_KEY]);
            Assert::AreEqual(root[CHILDREN_KEY][size_type(0)], copy[CHILDREN_KEY][size_type(0)]);
            Assert::AreNotSame(root[CHILDREN_KEY][size_type(0)], copy[CHILDREN_KEY][size_type(0)]);
            Assert::IsTrue(root[CHILDREN_KEY][size_type(0)].Equals(&(copy[CHILDREN_KEY][size_type(0)])));
            Assert::AreEqual(root[CHILDREN_KEY][size_type(0)][CHILDREN_KEY], copy[CHILDREN_KEY][size_type(0)][CHILDREN_KEY]);
            Assert::AreNotSame(root[CHILDREN_KEY][size_type(0)][CHILDREN_KEY], copy[CHILDREN_KEY][size_type(0)][CHILDREN_KEY]);
            Assert::IsTrue(copy[CHILDREN_KEY][size_type(0)][CHILDREN_KEY][size_type(0)].Is(Bar::TypeIdClass()));
            Assert::IsTrue(copy[CHILDREN_KEY][size_type(0)][CHILDREN_KEY][size_type(1)].Is(Bar::TypeIdClass()));
            Assert::AreEqual(root[CHILDREN_KEY][size_type(0)][CHILDREN_KEY][size_type(0)], copy[CHILDREN_KEY][size_type(0)][CHILDREN_KEY][size_type(0)]);
            Assert::AreNotSame(root[CHILDREN_KEY][size_type(0)][CHILDREN_KEY][size_type(0)], copy[CHILDREN_KEY][size_type(0)][CHILDREN_KEY][size_type(0)]);
            Assert::IsTrue(root[CHILDREN_KEY][size_type(0)][CHILDREN_KEY][size_type(0)].Equals(&(copy[CHILDREN_KEY][size_type(0)][CHILDREN_KEY][size_type(0)])));
            Assert::AreEqual(root[CHILDREN_KEY][size_type(0)][CHILDREN_KEY][size_type(1)], copy[CHILDREN_KEY][size_type(0)][CHILDREN_KEY][size_type(1)]);
            Assert::AreNotSame(root[CHILDREN_KEY][size_type(0)][CHILDREN_KEY][size_type(1)], copy[CHILDREN_KEY][size_type(0)][CHILDREN_KEY][size_type(1)]);
            Assert::IsTrue(root[CHILDREN_KEY][size_type(0)][CHILDREN_KEY][size_type(1)].Equals(&(copy[CHILDREN_KEY][size_type(0)][CHILDREN_KEY][size_type(1)])));

            root.Clear();

            Assert::IsTrue(root.IsEmpty());
            Assert::IsFalse(copy.IsEmpty());

            const key_type MONSTERS_KEY = "Monsters"s;

            child = &(root.AppendScope(MONSTERS_KEY));
            child->Append(NAME_KEY) = "Grizzly Bears"s;
            child->Append(POWER_NAME) = 2;
            child->Append(TOUGHNESS_NAME) = 2;
            child->Append(SIZE_CONTEXT) = "Bear-Sized"s;

            child = &(root.AppendScope(MONSTERS_KEY));
            child->Append(NAME_KEY) = "Hill Giant"s;
            child->Append(POWER_NAME) = 3;
            child->Append(TOUGHNESS_NAME) = 3;
            child->Append(SIZE_CONTEXT) = "Giant (obviously)"s;

            copy = root;

            Assert::AreEqual(root, copy);
            Assert::AreNotSame(root, copy);
            Assert::IsTrue(root.Equals(&copy));
            Assert::AreEqual(root.Size(), copy.Size());
            Assert::AreEqual(root[MONSTERS_KEY], copy[MONSTERS_KEY]);
            Assert::AreNotSame(root[MONSTERS_KEY], copy[MONSTERS_KEY]);
            Assert::AreEqual(root[MONSTERS_KEY][size_type(0)], copy[MONSTERS_KEY][size_type(0)]);
            Assert::AreNotSame(root[MONSTERS_KEY][size_type(0)], copy[MONSTERS_KEY][size_type(0)]);
            Assert::IsTrue(root[MONSTERS_KEY][size_type(0)].Equals(&(copy[MONSTERS_KEY][size_type(0)])));
            Assert::AreEqual(root[MONSTERS_KEY][size_type(0)][NAME_KEY], copy[MONSTERS_KEY][size_type(0)][NAME_KEY]);
            Assert::AreNotSame(root[MONSTERS_KEY][size_type(0)][NAME_KEY], copy[MONSTERS_KEY][size_type(0)][NAME_KEY]);
            Assert::AreEqual(root[MONSTERS_KEY][size_type(0)][POWER_NAME], copy[MONSTERS_KEY][size_type(0)][POWER_NAME]);
            Assert::AreNotSame(root[MONSTERS_KEY][size_type(0)][POWER_NAME], copy[MONSTERS_KEY][size_type(0)][POWER_NAME]);
            Assert::AreEqual(root[MONSTERS_KEY][size_type(0)][TOUGHNESS_NAME], copy[MONSTERS_KEY][size_type(0)][TOUGHNESS_NAME]);
            Assert::AreNotSame(root[MONSTERS_KEY][size_type(0)][TOUGHNESS_NAME], copy[MONSTERS_KEY][size_type(0)][TOUGHNESS_NAME]);
            Assert::AreEqual(root[MONSTERS_KEY][size_type(0)][SIZE_CONTEXT], copy[MONSTERS_KEY][size_type(0)][SIZE_CONTEXT]);
            Assert::AreNotSame(root[MONSTERS_KEY][size_type(0)][SIZE_CONTEXT], copy[MONSTERS_KEY][size_type(0)][SIZE_CONTEXT]);
            Assert::AreEqual(root[MONSTERS_KEY][size_type(1)], copy[MONSTERS_KEY][size_type(1)]);
            Assert::AreNotSame(root[MONSTERS_KEY][size_type(1)], copy[MONSTERS_KEY][size_type(1)]);
            Assert::IsTrue(root[MONSTERS_KEY][size_type(1)].Equals(&(copy[MONSTERS_KEY][size_type(1)])));
            Assert::AreEqual(root[MONSTERS_KEY][size_type(1)][NAME_KEY], copy[MONSTERS_KEY][size_type(1)][NAME_KEY]);
            Assert::AreNotSame(root[MONSTERS_KEY][size_type(1)][NAME_KEY], copy[MONSTERS_KEY][size_type(1)][NAME_KEY]);
            Assert::AreEqual(root[MONSTERS_KEY][size_type(1)][POWER_NAME], copy[MONSTERS_KEY][size_type(1)][POWER_NAME]);
            Assert::AreNotSame(root[MONSTERS_KEY][size_type(1)][POWER_NAME], copy[MONSTERS_KEY][size_type(1)][POWER_NAME]);
            Assert::AreEqual(root[MONSTERS_KEY][size_type(1)][TOUGHNESS_NAME], copy[MONSTERS_KEY][size_type(1)][TOUGHNESS_NAME]);
            Assert::AreNotSame(root[MONSTERS_KEY][size_type(1)][TOUGHNESS_NAME], copy[MONSTERS_KEY][size_type(1)][TOUGHNESS_NAME]);
            Assert::AreEqual(root[MONSTERS_KEY][size_type(1)][SIZE_CONTEXT], copy[MONSTERS_KEY][size_type(1)][SIZE_CONTEXT]);
            Assert::AreNotSame(root[MONSTERS_KEY][size_type(1)][SIZE_CONTEXT], copy[MONSTERS_KEY][size_type(1)][SIZE_CONTEXT]);
        }

        TEST_METHOD(MoveSemantics) {
            const key_type NAME_KEY = "Name"s;
            const key_type TRANSFORM_KEY = "Transform"s;
            const key_type CHILDREN_KEY = "Children"s;
            const key_type CONTEXT_KEY = "Context"s;
            const key_type RANGE_KEY = "Range"s;

            const String CHARACTER_NAME = "Character"s;
            const String STATS_NAME = "Stats"s;
            const String POWER_NAME = "Power"s;
            const String TOUGHNESS_NAME = "Toughness"s;
            const String SIZE_CONTEXT = "Size"s;
            const String VALUE_CONTEXT = "Value"s;

            Scope root{};
            Scope* scope = &root;

            scope->Append(NAME_KEY) = CHARACTER_NAME;
            scope->Append(TRANSFORM_KEY) = Matrix{};
            scope->Append(RANGE_KEY) = { 0.f, 100.f };

            Scope* child = new Bar{2};
            child->Append(NAME_KEY) = STATS_NAME;
            child->Append(CONTEXT_KEY) = SIZE_CONTEXT;

            scope->AttachAsChild(CHILDREN_KEY, child->Clone());
            scope = &(scope->At(CHILDREN_KEY).BackTable());

            delete child;
            child = new Bar{4};
            child->Append(NAME_KEY) = POWER_NAME;
            child->Append(CONTEXT_KEY) = VALUE_CONTEXT;

            scope->AttachAsChild(CHILDREN_KEY, child->Clone());

            delete child;
            child = new Bar{5};
            child->Append(NAME_KEY) = TOUGHNESS_NAME;
            child->Append(CONTEXT_KEY) = VALUE_CONTEXT;

            scope->AttachAsChild(CHILDREN_KEY, child->Clone());

            delete child;
            Scope copy{root};

            Assert::IsFalse(copy.IsEmpty());

            Scope destination{std::move(copy)};

#pragma warning(push)
#pragma warning(disable: 26800)
            Assert::IsTrue(copy.IsEmpty());
#pragma warning(pop)

            Assert::AreEqual(root, destination);
            Assert::AreNotSame(root, destination);
            Assert::IsTrue(root.Equals(&destination));
            Assert::AreEqual(root.Size(), destination.Size());
            Assert::AreEqual(root[NAME_KEY], destination[NAME_KEY]);
            Assert::AreEqual(root[TRANSFORM_KEY], destination[TRANSFORM_KEY]);
            Assert::AreEqual(root[RANGE_KEY], destination[RANGE_KEY]);
            Assert::IsTrue(destination[CHILDREN_KEY][size_type(0)].Is(Bar::TypeIdClass()));
            Assert::AreEqual(root[CHILDREN_KEY], destination[CHILDREN_KEY]);
            Assert::AreEqual(root[CHILDREN_KEY][size_type(0)], destination[CHILDREN_KEY][size_type(0)]);
            Assert::IsTrue(root[CHILDREN_KEY][size_type(0)].Equals(&(destination[CHILDREN_KEY][size_type(0)])));
            Assert::AreEqual(root[CHILDREN_KEY][size_type(0)][CHILDREN_KEY], destination[CHILDREN_KEY][size_type(0)][CHILDREN_KEY]);
            Assert::IsTrue(destination[CHILDREN_KEY][size_type(0)][CHILDREN_KEY][size_type(0)].Is(Bar::TypeIdClass()));
            Assert::IsTrue(destination[CHILDREN_KEY][size_type(0)][CHILDREN_KEY][size_type(1)].Is(Bar::TypeIdClass()));
            Assert::AreEqual(root[CHILDREN_KEY][size_type(0)][CHILDREN_KEY][size_type(0)], destination[CHILDREN_KEY][size_type(0)][CHILDREN_KEY][size_type(0)]);
            Assert::IsTrue(root[CHILDREN_KEY][size_type(0)][CHILDREN_KEY][size_type(0)].Equals(&(destination[CHILDREN_KEY][size_type(0)][CHILDREN_KEY][size_type(0)])));
            Assert::AreEqual(root[CHILDREN_KEY][size_type(0)][CHILDREN_KEY][size_type(1)], destination[CHILDREN_KEY][size_type(0)][CHILDREN_KEY][size_type(1)]);
            Assert::IsTrue(root[CHILDREN_KEY][size_type(0)][CHILDREN_KEY][size_type(1)].Equals(&(destination[CHILDREN_KEY][size_type(0)][CHILDREN_KEY][size_type(1)])));

            root.Clear();

            Assert::IsTrue(root.IsEmpty());
            Assert::IsFalse(destination.IsEmpty());

            const key_type MONSTERS_KEY = "Monsters"s;

            child = &(root.AppendScope(MONSTERS_KEY));
            child->Append(NAME_KEY) = "Grizzly Bears"s;
            child->Append(POWER_NAME) = 2;
            child->Append(TOUGHNESS_NAME) = 2;
            child->Append(SIZE_CONTEXT) = "Bear-Sized"s;

            child = &(root.AppendScope(MONSTERS_KEY));
            child->Append(NAME_KEY) = "Hill Giant"s;
            child->Append(POWER_NAME) = 3;
            child->Append(TOUGHNESS_NAME) = 3;
            child->Append(SIZE_CONTEXT) = "Giant (obviously)"s;

            Scope second{root};
            second = std::move(second);

#pragma warning(push)
#pragma warning(disable: 26800)
            destination = std::move(second);
#pragma warning(pop)

            Assert::AreEqual(root, destination);
            Assert::AreNotSame(root, destination);
            Assert::IsTrue(root.Equals(&destination));
            Assert::AreEqual(root.Size(), destination.Size());
            Assert::AreEqual(root[MONSTERS_KEY], destination[MONSTERS_KEY]);
            Assert::AreEqual(root[MONSTERS_KEY][size_type(0)], destination[MONSTERS_KEY][size_type(0)]);
            Assert::IsTrue(root[MONSTERS_KEY][size_type(0)].Equals(&(destination[MONSTERS_KEY][size_type(0)])));
            Assert::AreEqual(root[MONSTERS_KEY][size_type(0)][NAME_KEY], destination[MONSTERS_KEY][size_type(0)][NAME_KEY]);
            Assert::AreEqual(root[MONSTERS_KEY][size_type(0)][POWER_NAME], destination[MONSTERS_KEY][size_type(0)][POWER_NAME]);
            Assert::AreEqual(root[MONSTERS_KEY][size_type(0)][TOUGHNESS_NAME], destination[MONSTERS_KEY][size_type(0)][TOUGHNESS_NAME]);
            Assert::AreEqual(root[MONSTERS_KEY][size_type(0)][SIZE_CONTEXT], destination[MONSTERS_KEY][size_type(0)][SIZE_CONTEXT]);
            Assert::AreEqual(root[MONSTERS_KEY][size_type(1)], destination[MONSTERS_KEY][size_type(1)]);
            Assert::IsTrue(root[MONSTERS_KEY][size_type(1)].Equals(&(destination[MONSTERS_KEY][size_type(1)])));
            Assert::AreEqual(root[MONSTERS_KEY][size_type(1)][NAME_KEY], destination[MONSTERS_KEY][size_type(1)][NAME_KEY]);
            Assert::AreEqual(root[MONSTERS_KEY][size_type(1)][POWER_NAME], destination[MONSTERS_KEY][size_type(1)][POWER_NAME]);
            Assert::AreEqual(root[MONSTERS_KEY][size_type(1)][TOUGHNESS_NAME], destination[MONSTERS_KEY][size_type(1)][TOUGHNESS_NAME]);
            Assert::AreEqual(root[MONSTERS_KEY][size_type(1)][SIZE_CONTEXT], destination[MONSTERS_KEY][size_type(1)][SIZE_CONTEXT]);
        }

        TEST_METHOD(MoveDetachesChild) {
            const key_type NAME_KEY = "Name"s;
            const key_type CHILDREN_KEY = "Children"s;
            const String TOP_NAME = "Top"s;
            const String MIDDLE_NAME = "Middle"s;
            const String BOTTOM_NAME = "Bottom"s;

            Scope root{};
            Scope* scope = &root;
            scope->Append(NAME_KEY) = TOP_NAME;
            scope = &(scope->AppendScope(CHILDREN_KEY));
            scope->Append(NAME_KEY) = MIDDLE_NAME;
            scope = &(scope->AppendScope(CHILDREN_KEY));
            scope->Append(NAME_KEY) = BOTTOM_NAME;

            Scope copy{root};

            Assert::AreEqual(root.Size(), copy.Size());
            Assert::AreEqual(root[CHILDREN_KEY].Size(), copy[CHILDREN_KEY].Size());
            Assert::IsFalse(root[CHILDREN_KEY].Size() > copy[CHILDREN_KEY].Size());

            Scope destination = std::move(copy[CHILDREN_KEY][size_type(0)]);

            Assert::AreEqual(root.Size(), copy.Size());
            Assert::AreNotEqual(root[CHILDREN_KEY].Size(), copy[CHILDREN_KEY].Size());
            Assert::IsTrue(root[CHILDREN_KEY].Size() > copy[CHILDREN_KEY].Size());
            Assert::AreEqual(root[CHILDREN_KEY][size_type(0)][NAME_KEY], destination[NAME_KEY]);
            Assert::AreEqual(root[CHILDREN_KEY][size_type(0)][CHILDREN_KEY][size_type(0)][NAME_KEY], destination[CHILDREN_KEY][size_type(0)][NAME_KEY]);
        }

        TEST_METHOD(CannotAppendScopeToNonTableKey) {
            const key_type NOT_TABLE = "Definitely Not A Table"s;

            Scope scope{size_type(1)};
            scope.Append(NOT_TABLE) = 2;

            Assert::ExpectException<std::runtime_error>([&scope, &NOT_TABLE](){ scope.AppendScope(NOT_TABLE); });
        }

        TEST_METHOD(EqualsEdgeCases) {
            const key_type APPLES = "Apples"s;
            const key_type ORANGES = "Oranges"s;

            Scope first{};
            first.Append(APPLES) = 1;

            Scope second{};

            Assert::AreNotEqual(first, second);

            second.Append(ORANGES) = 2;

            Assert::AreNotEqual(first, second);

            second[ORANGES] = 3.f;

            Assert::AreNotEqual(first, second);
        }

        TEST_METHOD(TableDatumEqualityEdgeCases) {
            const key_type CHILDREN = "Children"s;
            const key_type INTEGER = "Integer"s;
            const key_type STRING = "String"s;

            Scope first{};
            const Scope& cfirst = first;
            Scope* child = &(first.AppendScope(CHILDREN));
            child->Append(INTEGER) = 1;

            Scope second{};
            const Scope& csecond = second;
            child = &(second.AppendScope(CHILDREN));
            child->Append(STRING) = "Hello World!"s;

            Assert::AreNotEqual(cfirst[CHILDREN], csecond[CHILDREN]);
            Assert::IsFalse(cfirst[CHILDREN][size_type(0)][INTEGER] == csecond[CHILDREN][size_type(0)]);

            Scope integer{};
            integer.Append(INTEGER, 1);

            Assert::IsTrue(cfirst[CHILDREN] == integer);
            Assert::IsTrue(cfirst[size_type(0)] == integer);
            Assert::IsTrue(cfirst[CHILDREN].IsContaining(integer));

            first[CHILDREN][size_type(0)][INTEGER] = { 2, 3 };

            Assert::IsTrue(cfirst[CHILDREN] != integer);
            Assert::IsTrue(cfirst[size_type(0)] != integer);
            Assert::IsFalse(cfirst[CHILDREN].IsContaining(integer));
        }

        TEST_METHOD(CannotAppendEmptyKey) {
            Scope scope{};

            Assert::ExpectException<std::invalid_argument>([&scope](){ scope.AppendScope(""s); });
        }

        TEST_METHOD(DetachFromTree) {
            const key_type CHILDREN = "Children"s;

            Scope root{};

            const auto& children = root.Append(CHILDREN);
            root.AppendScope(CHILDREN).Append("Name"s) = "Name"s;
            root.AppendScope(CHILDREN).Append("One") = 1;
            auto& detaching = root.AppendScope(CHILDREN);
            root.AppendScope(CHILDREN).Append("Point"s) = 2.3f;

            Assert::AreSame(children, root.CAt(CHILDREN));
            Assert::AreEqual(size_type(4), children.Size());
            Assert::IsTrue(children.CGetTableElement(size_type(2)).IsEmpty());

            detaching.DetachFromTree();

            Assert::AreEqual(size_type(3), children.Size());
            Assert::IsFalse(children.CGetTableElement(size_type(2)).IsEmpty());
        }

        TEST_METHOD(TrySetParent) {
            Scope parent{};
            Scope child{};
            Scope root{};

            Assert::IsFalse(child.TrySetParent(nullptr));
            Assert::IsFalse(child.TrySetParent(&child));
            Assert::IsTrue(child.TrySetParent(&parent));
            Assert::IsFalse(child.TrySetParent(&parent));
            Assert::IsFalse(child.TrySetParent(&root));
            Assert::IsFalse(parent.TrySetParent(nullptr));
            Assert::IsFalse(parent.TrySetParent(&child));
            Assert::IsFalse(parent.TrySetParent(&parent));
            Assert::IsTrue(parent.TrySetParent(&root));
            Assert::IsFalse(root.TrySetParent(nullptr));
            Assert::IsFalse(root.TrySetParent(&parent));
            Assert::IsFalse(root.TrySetParent(&child));
            Assert::IsFalse(parent.TrySetParent(&root));
            Assert::IsFalse(child.TrySetParent(&root));
        }

        TEST_METHOD(AttachAsChild) {
            const key_type ATTACHED = "Attached"s;

            Scope root{};
            Scope tmp{};

            tmp.Append("Name"s) = "Test"s;
            tmp.Append("Value"s) = 12.34f;
            tmp.AppendScope("Child"s);

            Scope copy{tmp};

            Assert::IsFalse(tmp.IsEmpty());

            root.AttachAsChild(ATTACHED, std::move(tmp));

#pragma warning(push)
#pragma warning(disable: 26800)
            Assert::IsTrue(tmp.IsEmpty());
#pragma warning(pop)

            Scope& child = root.At(ATTACHED).FrontTable();
            Assert::AreEqual(copy, child);

            Assert::ExpectException<std::invalid_argument>([&ATTACHED, &child, &root](){ child.AttachAsChild(ATTACHED, std::move(root)); });

            Scope::ScopeUniquePointer ptr = std::make_unique<Scope>();
            ptr->TrySetParent(&root);

            Assert::ExpectException<std::invalid_argument>([&ATTACHED, &ptr, &root](){ ptr->AttachAsChild(ATTACHED, std::move(root)); });
        }
    };
}
