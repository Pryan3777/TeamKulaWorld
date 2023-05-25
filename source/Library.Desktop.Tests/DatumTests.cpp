#include "pch.h"
#include "CppUnitTest.h"
#include "Datum.h"
#include "Scope.h"
#include "Foo.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std::literals::string_literals;

namespace LibraryDesktopTests {
    TEST_CLASS(DatumTests) {

    private:
        inline static _CrtMemState _startMemState;

        using DatumType = Datum::DatumType;
        using Integer = Datum::Integer;
        using Float = Datum::Float;
        using String = Datum::String;
        using Vector = Datum::Vector;
        using Matrix = Datum::Matrix;
        using Pointer = Datum::Pointer;
        using Table = Datum::Table;
        using size_type = Datum::size_type;

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
            const String DEFAULT_TOSTRING = "[  ]"s;

            Datum datum{};

            auto defaultSize = datum.Size();
            auto defaultCapacity = datum.Capacity();

            Assert::IsTrue(datum.IsEmpty());
            Assert::AreEqual(DatumType::Unknown, datum.ApparentType());
            Assert::AreEqual(size_type(0), defaultSize);
            Assert::AreEqual(size_type(0), defaultCapacity);
            Assert::IsFalse(datum.RemoveAt(size_type(0)));
            Assert::AreEqual(DEFAULT_TOSTRING, datum.ToString());
            Assert::ExpectException<std::logic_error>([&datum](){ datum.Resize(size_type(1)); });
            Assert::ExpectException<std::out_of_range>([&datum](){ auto tmp = datum.ElementToString(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&datum](){ auto tmp = datum.ToString(size_type(0)); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&datum](){ datum.SetElementFromString(""s); });
            Assert::ExpectException<std::logic_error>([&datum](){ auto& tmp = datum.BackInteger(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ auto& tmp = datum.BackFloat(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ auto& tmp = datum.BackString(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ auto& tmp = datum.BackVector(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ auto& tmp = datum.BackMatrix(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ auto& tmp = datum.BackPointer(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ auto& tmp = datum.FrontInteger(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ auto& tmp = datum.FrontFloat(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ auto& tmp = datum.FrontString(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ auto& tmp = datum.FrontVector(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ auto& tmp = datum.FrontMatrix(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ auto& tmp = datum.FrontPointer(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ auto& tmp = datum.GetIntegerElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ auto& tmp = datum.GetFloatElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ auto& tmp = datum.GetStringElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ auto& tmp = datum.GetVectorElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ auto& tmp = datum.GetMatrixElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ auto& tmp = datum.GetPointerElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ auto tmp = datum.CBackInteger(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ auto tmp = datum.CBackFloat(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ const auto& tmp = datum.CBackString(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ const auto& tmp = datum.CBackVector(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ const auto& tmp = datum.CBackMatrix(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ auto tmp = datum.CBackPointer(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ auto tmp = datum.CFrontInteger(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ auto tmp = datum.CFrontFloat(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ const auto& tmp = datum.CFrontString(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ const auto& tmp = datum.CFrontVector(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ const auto& tmp = datum.CFrontMatrix(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ auto tmp = datum.CFrontPointer(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ auto tmp = datum.CGetIntegerElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ auto tmp = datum.CGetFloatElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ const auto& tmp = datum.CGetStringElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ const auto& tmp = datum.CGetVectorElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ const auto& tmp = datum.CGetMatrixElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&datum](){ auto tmp = datum.CGetPointerElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::invalid_argument>([&datum](){ datum.SetElement(Integer{}); });
            Assert::ExpectException<std::invalid_argument>([&datum](){ datum.SetElement(Float{}); });
            Assert::ExpectException<std::invalid_argument>([&datum](){ datum.SetElement(String{}); });
            Assert::ExpectException<std::invalid_argument>([&datum](){ datum.SetElement(Vector{}); });
            Assert::ExpectException<std::invalid_argument>([&datum](){ datum.SetElement(Matrix{}); });
            Assert::ExpectException<std::invalid_argument>([&datum](){ datum.SetElement(nullptr); });
            Assert::IsTrue(datum != Integer{});
            Assert::IsTrue(datum != Float{});
            Assert::IsTrue(datum != String{});
            Assert::IsTrue(datum != String{});
            Assert::IsTrue(datum != String{});
            Assert::IsTrue(datum == nullptr);
            Assert::IsFalse(datum.IsContaining(Integer{}));
            Assert::IsFalse(datum.IsContaining(Float{}));
            Assert::IsFalse(datum.IsContaining(String{}));
            Assert::IsFalse(datum.IsContaining(String{}));
            Assert::IsFalse(datum.IsContaining(String{}));
            Assert::IsFalse(datum.IsContaining(nullptr));

            Foo foo{0};

            Assert::IsTrue(datum != &foo);

            datum.PopBack();

            Assert::AreEqual(datum.Size(), defaultSize);
            Assert::AreEqual(datum.Capacity(), defaultCapacity);

            datum.Clear();

            Assert::AreEqual(datum.Size(), defaultSize);
            Assert::AreEqual(datum.Capacity(), defaultCapacity);

            datum.ShrinkToFit();

            Assert::AreEqual(datum.Size(), defaultSize);
            Assert::AreEqual(datum.Capacity(), defaultCapacity);

            datum.SetType(DatumType::Integer);

            Assert::AreEqual(DatumType::Integer, datum.ApparentType());

            const Datum cdatum{};

            Assert::ExpectException<std::logic_error>([&cdatum](){ auto tmp = cdatum.BackInteger(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&cdatum](){ auto tmp = cdatum.BackFloat(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&cdatum](){ const auto& tmp = cdatum.BackString(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&cdatum](){ const auto& tmp = cdatum.BackVector(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&cdatum](){ const auto& tmp = cdatum.BackMatrix(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&cdatum](){ auto tmp = cdatum.BackPointer(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&cdatum](){ auto tmp = cdatum.FrontInteger(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&cdatum](){ auto tmp = cdatum.FrontFloat(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&cdatum](){ const auto& tmp = cdatum.FrontString(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&cdatum](){ const auto& tmp = cdatum.FrontVector(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&cdatum](){ const auto& tmp = cdatum.FrontMatrix(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&cdatum](){ auto tmp = cdatum.FrontPointer(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&cdatum](){ auto tmp = cdatum.GetIntegerElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&cdatum](){ auto tmp = cdatum.GetFloatElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&cdatum](){ const auto& tmp = cdatum.GetStringElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&cdatum](){ const auto& tmp = cdatum.GetVectorElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&cdatum](){ const auto& tmp = cdatum.GetMatrixElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::logic_error>([&cdatum](){ auto tmp = cdatum.GetPointerElement(); UNREFERENCED_LOCAL(tmp); });
        }

        TEST_METHOD(DatumTypeConstructor) {
            const size_type INTEGER_CAPACITY = size_type(0);
            const size_type FLOAT_CAPACITY = size_type(1);
            const size_type STRING_CAPACITY = size_type(2);
            const size_type VECTOR_CAPACITY = size_type(3);
            const size_type MATRIX_CAPACITY = size_type(4);
            const size_type POINTER_CAPACITY = size_type(5);
            const size_type UNKNOWN_CAPACITY = size_type(6);

            Datum integerDatum{DatumType::Integer};
            Datum floatDatum{DatumType::Float, FLOAT_CAPACITY};
            Datum stringDatum{DatumType::String, STRING_CAPACITY};
            Datum vectorDatum{DatumType::Vector, VECTOR_CAPACITY};
            Datum matrixDatum{DatumType::Matrix, MATRIX_CAPACITY};
            Datum pointerDatum{DatumType::Pointer, POINTER_CAPACITY};
            Datum unknownDatum{DatumType::Unknown, UNKNOWN_CAPACITY};

            Assert::AreEqual(DatumType::Integer, integerDatum.ApparentType());
            Assert::AreEqual(DatumType::Float, floatDatum.ApparentType());
            Assert::AreEqual(DatumType::String, stringDatum.ApparentType());
            Assert::AreEqual(DatumType::Vector, vectorDatum.ApparentType());
            Assert::AreEqual(DatumType::Matrix, matrixDatum.ApparentType());
            Assert::AreEqual(DatumType::Pointer, pointerDatum.ApparentType());
            Assert::AreEqual(DatumType::Unknown, unknownDatum.ApparentType());
            Assert::IsTrue(integerDatum.IsDataInternal());
            Assert::IsTrue(floatDatum.IsDataInternal());
            Assert::IsTrue(stringDatum.IsDataInternal());
            Assert::IsTrue(vectorDatum.IsDataInternal());
            Assert::IsTrue(matrixDatum.IsDataInternal());
            Assert::IsTrue(pointerDatum.IsDataInternal());
            Assert::IsTrue(unknownDatum.IsDataInternal());
            Assert::IsTrue(integerDatum.IsEmpty());
            Assert::IsTrue(floatDatum.IsEmpty());
            Assert::IsTrue(stringDatum.IsEmpty());
            Assert::IsTrue(vectorDatum.IsEmpty());
            Assert::IsTrue(matrixDatum.IsEmpty());
            Assert::IsTrue(pointerDatum.IsEmpty());
            Assert::IsTrue(unknownDatum.IsEmpty());
            Assert::AreEqual(INTEGER_CAPACITY, integerDatum.Capacity());
            Assert::AreEqual(FLOAT_CAPACITY, floatDatum.Capacity());
            Assert::AreEqual(STRING_CAPACITY, stringDatum.Capacity());
            Assert::AreEqual(VECTOR_CAPACITY, vectorDatum.Capacity());
            Assert::AreEqual(MATRIX_CAPACITY, matrixDatum.Capacity());
            Assert::AreEqual(POINTER_CAPACITY, pointerDatum.Capacity());
            Assert::AreNotEqual(UNKNOWN_CAPACITY, unknownDatum.Capacity());
            Assert::AreEqual(size_type(0), unknownDatum.Capacity());
            Assert::AreEqual(size_type(0), integerDatum.Size());
            Assert::AreEqual(size_type(0), floatDatum.Size());
            Assert::AreEqual(size_type(0), stringDatum.Size());
            Assert::AreEqual(size_type(0), vectorDatum.Size());
            Assert::AreEqual(size_type(0), matrixDatum.Size());
            Assert::AreEqual(size_type(0), pointerDatum.Size());
            Assert::AreEqual(size_type(0), unknownDatum.Size());
            Assert::IsFalse(integerDatum == Integer{});
            Assert::IsFalse(floatDatum == Float{});
            Assert::IsFalse(stringDatum == String{});
            Assert::IsFalse(vectorDatum == Vector{});
            Assert::IsFalse(matrixDatum == Matrix{});
            Assert::IsFalse(pointerDatum == nullptr);
            Assert::ExpectException<std::invalid_argument>([&integerDatum](){ integerDatum.SetType(DatumType::Float); });
            Assert::ExpectException<std::invalid_argument>([&floatDatum](){ floatDatum.SetType(DatumType::String); });
            Assert::ExpectException<std::invalid_argument>([&stringDatum](){ stringDatum.SetType(DatumType::Vector); });
            Assert::ExpectException<std::invalid_argument>([&vectorDatum](){ vectorDatum.SetType(DatumType::Matrix); });
            Assert::ExpectException<std::invalid_argument>([&matrixDatum](){ matrixDatum.SetType(DatumType::Pointer); });
            Assert::ExpectException<std::invalid_argument>([&pointerDatum](){ pointerDatum.SetType(DatumType::Integer); });
            Assert::ExpectException<std::out_of_range>([&integerDatum](){ auto& tmp = integerDatum.BackInteger(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&floatDatum](){ auto& tmp = floatDatum.BackFloat(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&stringDatum](){ auto& tmp = stringDatum.BackString(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&vectorDatum](){ auto& tmp = vectorDatum.BackVector(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&matrixDatum](){ auto& tmp = matrixDatum.BackMatrix(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&pointerDatum](){ auto& tmp = pointerDatum.BackPointer(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&integerDatum](){ auto& tmp = integerDatum.FrontInteger(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&floatDatum](){ auto& tmp = floatDatum.FrontFloat(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&stringDatum](){ auto& tmp = stringDatum.FrontString(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&vectorDatum](){ auto& tmp = vectorDatum.FrontVector(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&matrixDatum](){ auto& tmp = matrixDatum.FrontMatrix(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&pointerDatum](){ auto& tmp = pointerDatum.FrontPointer(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&integerDatum](){ auto& tmp = integerDatum.GetIntegerElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&floatDatum](){ auto& tmp = floatDatum.GetFloatElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&stringDatum](){ auto& tmp = stringDatum.GetStringElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&vectorDatum](){ auto& tmp = vectorDatum.GetVectorElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&matrixDatum](){ auto& tmp = matrixDatum.GetMatrixElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&pointerDatum](){ auto& tmp = pointerDatum.GetPointerElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&integerDatum](){ auto tmp = integerDatum.CBackInteger(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&floatDatum](){ auto tmp = floatDatum.CBackFloat(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&stringDatum](){ const auto& tmp = stringDatum.CBackString(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&vectorDatum](){ const auto& tmp = vectorDatum.CBackVector(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&matrixDatum](){ const auto& tmp = matrixDatum.CBackMatrix(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&pointerDatum](){ auto tmp = pointerDatum.CBackPointer(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&integerDatum](){ auto tmp = integerDatum.CFrontInteger(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&floatDatum](){ auto tmp = floatDatum.CFrontFloat(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&stringDatum](){ const auto& tmp = stringDatum.CFrontString(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&vectorDatum](){ const auto& tmp = vectorDatum.CFrontVector(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&matrixDatum](){ const auto& tmp = matrixDatum.CFrontMatrix(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&pointerDatum](){ auto tmp = pointerDatum.CFrontPointer(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&integerDatum](){ auto tmp = integerDatum.CGetIntegerElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&floatDatum](){ auto tmp = floatDatum.CGetFloatElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&stringDatum](){ const auto& tmp = stringDatum.CGetStringElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&vectorDatum](){ const auto& tmp = vectorDatum.CGetVectorElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&matrixDatum](){ const auto& tmp = matrixDatum.CGetMatrixElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&pointerDatum](){ auto tmp = pointerDatum.CGetPointerElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&integerDatum](){ integerDatum.SetElement(Integer{}); });
            Assert::ExpectException<std::out_of_range>([&floatDatum](){ floatDatum.SetElement(Float{}); });
            Assert::ExpectException<std::out_of_range>([&stringDatum](){ stringDatum.SetElement(String{}); });
            Assert::ExpectException<std::out_of_range>([&vectorDatum](){ vectorDatum.SetElement(Vector{}); });
            Assert::ExpectException<std::out_of_range>([&matrixDatum](){ matrixDatum.SetElement(Matrix{}); });
            Assert::ExpectException<std::out_of_range>([&pointerDatum](){ pointerDatum.SetElement(nullptr); });
            Assert::IsFalse(integerDatum.IsContaining(Integer{}));
            Assert::IsFalse(floatDatum.IsContaining(Float{}));
            Assert::IsFalse(stringDatum.IsContaining(String{}));
            Assert::IsFalse(vectorDatum.IsContaining(Vector{}));
            Assert::IsFalse(matrixDatum.IsContaining(Matrix{}));
            Assert::IsFalse(pointerDatum.IsContaining(nullptr));

            size_type index;

            integerDatum.Find(Integer{}, index);

            Assert::AreEqual(size_type(0), index);

            floatDatum.Find(Float{}, index);

            Assert::AreEqual(size_type(0), index);

            stringDatum.Find(String{}, index);

            Assert::AreEqual(size_type(0), index);

            vectorDatum.Find(Vector{}, index);

            Assert::AreEqual(size_type(0), index);

            matrixDatum.Find(Matrix{}, index);

            Assert::AreEqual(size_type(0), index);

            pointerDatum.Find(nullptr, index);

            Assert::AreEqual(size_type(0), index);

            integerDatum.Reserve(UNKNOWN_CAPACITY);

            Assert::AreEqual(UNKNOWN_CAPACITY, integerDatum.Capacity());

            integerDatum.ShrinkToFit();
            floatDatum.ShrinkToFit();
            stringDatum.ShrinkToFit();
            vectorDatum.ShrinkToFit();
            matrixDatum.ShrinkToFit();
            pointerDatum.ShrinkToFit();

            Assert::AreNotEqual(UNKNOWN_CAPACITY, integerDatum.Capacity());
            Assert::AreNotEqual(FLOAT_CAPACITY, floatDatum.Capacity());
            Assert::AreNotEqual(STRING_CAPACITY, stringDatum.Capacity());
            Assert::AreNotEqual(VECTOR_CAPACITY, vectorDatum.Capacity());
            Assert::AreNotEqual(MATRIX_CAPACITY, matrixDatum.Capacity());
            Assert::AreNotEqual(POINTER_CAPACITY, pointerDatum.Capacity());
            Assert::AreEqual(size_type(0), integerDatum.Capacity());
            Assert::AreEqual(size_type(0), floatDatum.Capacity());
            Assert::AreEqual(size_type(0), stringDatum.Capacity());
            Assert::AreEqual(size_type(0), vectorDatum.Capacity());
            Assert::AreEqual(size_type(0), matrixDatum.Capacity());
            Assert::AreEqual(size_type(0), pointerDatum.Capacity());

            const Datum cintegerDatum{DatumType::Integer};
            const Datum cfloatDatum{DatumType::Float, FLOAT_CAPACITY};
            const Datum cstringDatum{DatumType::String, STRING_CAPACITY};
            const Datum cvectorDatum{DatumType::Vector, VECTOR_CAPACITY};
            const Datum cmatrixDatum{DatumType::Matrix, MATRIX_CAPACITY};
            const Datum cpointerDatum{DatumType::Pointer, POINTER_CAPACITY};

            Assert::ExpectException<std::out_of_range>([&cintegerDatum](){ auto tmp = cintegerDatum.BackInteger(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&cfloatDatum](){ auto tmp = cfloatDatum.BackFloat(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&cstringDatum](){ const auto& tmp = cstringDatum.BackString(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&cvectorDatum](){ const auto& tmp = cvectorDatum.BackVector(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&cmatrixDatum](){ const auto& tmp = cmatrixDatum.BackMatrix(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&cpointerDatum](){ auto tmp = cpointerDatum.BackPointer(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&cintegerDatum](){ auto tmp = cintegerDatum.FrontInteger(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&cfloatDatum](){ auto tmp = cfloatDatum.FrontFloat(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&cstringDatum](){ const auto& tmp = cstringDatum.FrontString(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&cvectorDatum](){ const auto& tmp = cvectorDatum.FrontVector(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&cmatrixDatum](){ const auto& tmp = cmatrixDatum.FrontMatrix(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&cpointerDatum](){ auto tmp = cpointerDatum.FrontPointer(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&cintegerDatum](){ auto tmp = cintegerDatum.GetIntegerElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&cfloatDatum](){ auto tmp = cfloatDatum.GetFloatElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&cstringDatum](){ const auto& tmp = cstringDatum.GetStringElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&cvectorDatum](){ const auto& tmp = cvectorDatum.GetVectorElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&cmatrixDatum](){ const auto& tmp = cmatrixDatum.GetMatrixElement(); UNREFERENCED_LOCAL(tmp); });
            Assert::ExpectException<std::out_of_range>([&cpointerDatum](){ auto tmp = cpointerDatum.GetPointerElement(); UNREFERENCED_LOCAL(tmp); });
        }

        TEST_METHOD(CopyScalarConstructor) {
            const size_type ONE = size_type(1);
            const Integer INTEGER = 7;
            const Float FLOAT = 8.f;
            const String STRING = "9"s;
            const Vector VECTOR{10.f, 11.f, 12.f, 13.f};
            const Matrix MATRIX{VECTOR, VECTOR, VECTOR, VECTOR};

            Foo foo{14};

            Datum integerDatum = INTEGER;
            Datum floatDatum = FLOAT;
            Datum stringDatum = STRING;
            Datum vectorDatum = VECTOR;
            Datum matrixDatum = MATRIX;
            Datum pointerDatum = &foo;

            Assert::AreEqual(DatumType::Integer, integerDatum.ApparentType());
            Assert::AreEqual(DatumType::Float, floatDatum.ApparentType());
            Assert::AreEqual(DatumType::String, stringDatum.ApparentType());
            Assert::AreEqual(DatumType::Vector, vectorDatum.ApparentType());
            Assert::AreEqual(DatumType::Matrix, matrixDatum.ApparentType());
            Assert::AreEqual(DatumType::Pointer, pointerDatum.ApparentType());
            Assert::IsTrue(integerDatum.IsDataInternal());
            Assert::IsTrue(floatDatum.IsDataInternal());
            Assert::IsTrue(stringDatum.IsDataInternal());
            Assert::IsTrue(vectorDatum.IsDataInternal());
            Assert::IsTrue(matrixDatum.IsDataInternal());
            Assert::IsTrue(pointerDatum.IsDataInternal());
            Assert::IsFalse(integerDatum.IsEmpty());
            Assert::IsFalse(floatDatum.IsEmpty());
            Assert::IsFalse(stringDatum.IsEmpty());
            Assert::IsFalse(vectorDatum.IsEmpty());
            Assert::IsFalse(matrixDatum.IsEmpty());
            Assert::IsFalse(pointerDatum.IsEmpty());
            Assert::AreEqual(ONE, integerDatum.Capacity());
            Assert::AreEqual(ONE, floatDatum.Capacity());
            Assert::AreEqual(ONE, stringDatum.Capacity());
            Assert::AreEqual(ONE, vectorDatum.Capacity());
            Assert::AreEqual(ONE, matrixDatum.Capacity());
            Assert::AreEqual(ONE, pointerDatum.Capacity());
            Assert::AreEqual(ONE, integerDatum.Size());
            Assert::AreEqual(ONE, floatDatum.Size());
            Assert::AreEqual(ONE, stringDatum.Size());
            Assert::AreEqual(ONE, vectorDatum.Size());
            Assert::AreEqual(ONE, matrixDatum.Size());
            Assert::AreEqual(ONE, pointerDatum.Size());
            Assert::IsTrue(integerDatum == INTEGER);
            Assert::IsTrue(floatDatum == FLOAT);
            Assert::IsTrue(stringDatum == STRING);
            Assert::IsTrue(vectorDatum == VECTOR);
            Assert::IsTrue(matrixDatum == MATRIX);
            Assert::IsTrue(pointerDatum == &foo);
            Assert::AreEqual(INTEGER, integerDatum.FrontInteger());
            Assert::AreEqual(INTEGER, integerDatum.BackInteger());
            Assert::AreEqual(INTEGER, integerDatum.GetIntegerElement());
            Assert::AreNotSame(INTEGER, integerDatum.FrontInteger());
            Assert::AreNotSame(INTEGER, integerDatum.BackInteger());
            Assert::AreNotSame(INTEGER, integerDatum.GetIntegerElement());
            Assert::AreEqual(FLOAT, floatDatum.FrontFloat());
            Assert::AreEqual(FLOAT, floatDatum.BackFloat());
            Assert::AreEqual(FLOAT, floatDatum.GetFloatElement());
            Assert::AreNotSame(FLOAT, floatDatum.FrontFloat());
            Assert::AreNotSame(FLOAT, floatDatum.BackFloat());
            Assert::AreNotSame(FLOAT, floatDatum.GetFloatElement());
            Assert::AreEqual(STRING, stringDatum.FrontString());
            Assert::AreEqual(STRING, stringDatum.BackString());
            Assert::AreEqual(STRING, stringDatum.GetStringElement());
            Assert::AreNotSame(STRING, stringDatum.FrontString());
            Assert::AreNotSame(STRING, stringDatum.BackString());
            Assert::AreNotSame(STRING, stringDatum.GetStringElement());
            Assert::AreEqual(VECTOR, vectorDatum.FrontVector());
            Assert::AreEqual(VECTOR, vectorDatum.BackVector());
            Assert::AreEqual(VECTOR, vectorDatum.GetVectorElement());
            Assert::AreNotSame(VECTOR, vectorDatum.FrontVector());
            Assert::AreNotSame(VECTOR, vectorDatum.BackVector());
            Assert::AreNotSame(VECTOR, vectorDatum.GetVectorElement());
            Assert::AreEqual(MATRIX, matrixDatum.FrontMatrix());
            Assert::AreEqual(MATRIX, matrixDatum.BackMatrix());
            Assert::AreEqual(MATRIX, matrixDatum.GetMatrixElement());
            Assert::AreNotSame(MATRIX, matrixDatum.FrontMatrix());
            Assert::AreNotSame(MATRIX, matrixDatum.BackMatrix());
            Assert::AreNotSame(MATRIX, matrixDatum.GetMatrixElement());
            Assert::IsTrue(foo.Equals(pointerDatum.FrontPointer()));
            Assert::IsTrue(foo.Equals(pointerDatum.BackPointer()));
            Assert::IsTrue(foo.Equals(pointerDatum.GetPointerElement()));
            Assert::IsTrue(&foo == pointerDatum.FrontPointer());
            Assert::IsTrue(&foo == pointerDatum.BackPointer());
            Assert::IsTrue(&foo == pointerDatum.GetPointerElement());
            Assert::AreEqual(INTEGER, integerDatum.CFrontInteger());
            Assert::AreEqual(INTEGER, integerDatum.CBackInteger());
            Assert::AreEqual(INTEGER, integerDatum.CGetIntegerElement());
            Assert::AreNotSame(INTEGER, integerDatum.CFrontInteger());
            Assert::AreNotSame(INTEGER, integerDatum.CBackInteger());
            Assert::AreNotSame(INTEGER, integerDatum.CGetIntegerElement());
            Assert::AreEqual(FLOAT, floatDatum.CFrontFloat());
            Assert::AreEqual(FLOAT, floatDatum.CBackFloat());
            Assert::AreEqual(FLOAT, floatDatum.CGetFloatElement());
            Assert::AreNotSame(FLOAT, floatDatum.CFrontFloat());
            Assert::AreNotSame(FLOAT, floatDatum.CBackFloat());
            Assert::AreNotSame(FLOAT, floatDatum.CGetFloatElement());
            Assert::AreEqual(STRING, stringDatum.CFrontString());
            Assert::AreEqual(STRING, stringDatum.CBackString());
            Assert::AreEqual(STRING, stringDatum.CGetStringElement());
            Assert::AreNotSame(STRING, stringDatum.CFrontString());
            Assert::AreNotSame(STRING, stringDatum.CBackString());
            Assert::AreNotSame(STRING, stringDatum.CGetStringElement());
            Assert::AreEqual(VECTOR, vectorDatum.CFrontVector());
            Assert::AreEqual(VECTOR, vectorDatum.CBackVector());
            Assert::AreEqual(VECTOR, vectorDatum.CGetVectorElement());
            Assert::AreNotSame(VECTOR, vectorDatum.CFrontVector());
            Assert::AreNotSame(VECTOR, vectorDatum.CBackVector());
            Assert::AreNotSame(VECTOR, vectorDatum.CGetVectorElement());
            Assert::AreEqual(MATRIX, matrixDatum.CFrontMatrix());
            Assert::AreEqual(MATRIX, matrixDatum.CBackMatrix());
            Assert::AreEqual(MATRIX, matrixDatum.CGetMatrixElement());
            Assert::AreNotSame(MATRIX, matrixDatum.CFrontMatrix());
            Assert::AreNotSame(MATRIX, matrixDatum.CBackMatrix());
            Assert::AreNotSame(MATRIX, matrixDatum.CGetMatrixElement());
            Assert::IsTrue(foo.Equals(pointerDatum.CFrontPointer()));
            Assert::IsTrue(foo.Equals(pointerDatum.CBackPointer()));
            Assert::IsTrue(foo.Equals(pointerDatum.CGetPointerElement()));
            Assert::IsTrue(&foo == pointerDatum.CFrontPointer());
            Assert::IsTrue(&foo == pointerDatum.CBackPointer());
            Assert::IsTrue(&foo == pointerDatum.CGetPointerElement());
            Assert::IsTrue(integerDatum.IsContaining(INTEGER));
            Assert::IsTrue(floatDatum.IsContaining(FLOAT));
            Assert::IsTrue(stringDatum.IsContaining(STRING));
            Assert::IsTrue(vectorDatum.IsContaining(VECTOR));
            Assert::IsTrue(matrixDatum.IsContaining(MATRIX));
            Assert::IsTrue(pointerDatum.IsContaining(&foo));

            char buffer[200];
            sprintf_s(buffer, 200, "%d", INTEGER);

            Assert::AreEqual(String{buffer}, integerDatum.ElementToString());

            sprintf_s(buffer, 200, "%F", FLOAT);

            Assert::AreEqual(String{buffer}, floatDatum.ElementToString());
            Assert::AreEqual(STRING, stringDatum.ElementToString());

            sprintf_s(buffer, 200, Datum::VECTOR_STRING_FORMAT.c_str(), VECTOR.x, VECTOR.y, VECTOR.z, VECTOR.w);

            Assert::AreEqual(String{buffer}, vectorDatum.ElementToString());

            const Matrix::row_type& R0 = MATRIX[0];
            const Matrix::row_type& R1 = MATRIX[1];
            const Matrix::row_type& R2 = MATRIX[2];
            const Matrix::row_type& R3 = MATRIX[3];
            sprintf_s(buffer, 200, Datum::MATRIX_STRING_FORMAT.c_str(),
                R0.x, R0.y, R0.z, R0.w,
                R1.x, R1.y, R1.z, R1.w,
                R2.x, R2.y, R2.z, R2.w,
                R3.x, R3.y, R3.z, R3.w
            );

            Assert::AreEqual(String{buffer}, matrixDatum.ElementToString());
            Assert::AreEqual(foo.ToString(), pointerDatum.ElementToString());
        }

        TEST_METHOD(MoveScalarConstructor) {
            const size_type ONE = size_type(1);
            const String STRING = "9"s;
            const Vector VECTOR{10.f, 11.f, 12.f, 13.f};
            const Matrix MATRIX{VECTOR, VECTOR, VECTOR, VECTOR};

            String copyString{STRING};
            Vector copyVector{VECTOR};
            Matrix copyMatrix{MATRIX};

            Datum stringDatum = std::move(copyString);
            Datum vectorDatum = std::move(copyVector);
            Datum matrixDatum = std::move(copyMatrix);

            Assert::AreEqual(DatumType::String, stringDatum.ApparentType());
            Assert::AreEqual(DatumType::Vector, vectorDatum.ApparentType());
            Assert::AreEqual(DatumType::Matrix, matrixDatum.ApparentType());
            Assert::IsTrue(stringDatum.IsDataInternal());
            Assert::IsTrue(vectorDatum.IsDataInternal());
            Assert::IsTrue(matrixDatum.IsDataInternal());
            Assert::IsFalse(stringDatum.IsEmpty());
            Assert::IsFalse(vectorDatum.IsEmpty());
            Assert::IsFalse(matrixDatum.IsEmpty());
            Assert::AreEqual(ONE, stringDatum.Capacity());
            Assert::AreEqual(ONE, vectorDatum.Capacity());
            Assert::AreEqual(ONE, matrixDatum.Capacity());
            Assert::AreEqual(ONE, stringDatum.Size());
            Assert::AreEqual(ONE, vectorDatum.Size());
            Assert::AreEqual(ONE, matrixDatum.Size());
            Assert::IsFalse(stringDatum != STRING);
            Assert::IsFalse(vectorDatum != VECTOR);
            Assert::IsFalse(matrixDatum != MATRIX);
            Assert::AreEqual(STRING, stringDatum.FrontString());
            Assert::AreEqual(STRING, stringDatum.BackString());
            Assert::AreEqual(STRING, stringDatum.GetStringElement());
            Assert::AreNotSame(STRING, stringDatum.FrontString());
            Assert::AreNotSame(STRING, stringDatum.BackString());
            Assert::AreNotSame(STRING, stringDatum.GetStringElement());
            Assert::AreEqual(VECTOR, vectorDatum.FrontVector());
            Assert::AreEqual(VECTOR, vectorDatum.BackVector());
            Assert::AreEqual(VECTOR, vectorDatum.GetVectorElement());
            Assert::AreNotSame(VECTOR, vectorDatum.FrontVector());
            Assert::AreNotSame(VECTOR, vectorDatum.BackVector());
            Assert::AreNotSame(VECTOR, vectorDatum.GetVectorElement());
            Assert::AreEqual(MATRIX, matrixDatum.FrontMatrix());
            Assert::AreEqual(MATRIX, matrixDatum.BackMatrix());
            Assert::AreEqual(MATRIX, matrixDatum.GetMatrixElement());
            Assert::AreNotSame(MATRIX, matrixDatum.FrontMatrix());
            Assert::AreNotSame(MATRIX, matrixDatum.BackMatrix());
            Assert::AreNotSame(MATRIX, matrixDatum.GetMatrixElement());
            Assert::AreEqual(STRING, stringDatum.CFrontString());
            Assert::AreEqual(STRING, stringDatum.CBackString());
            Assert::AreEqual(STRING, stringDatum.CGetStringElement());
            Assert::AreNotSame(STRING, stringDatum.CFrontString());
            Assert::AreNotSame(STRING, stringDatum.CBackString());
            Assert::AreNotSame(STRING, stringDatum.CGetStringElement());
            Assert::AreEqual(VECTOR, vectorDatum.CFrontVector());
            Assert::AreEqual(VECTOR, vectorDatum.CBackVector());
            Assert::AreEqual(VECTOR, vectorDatum.CGetVectorElement());
            Assert::AreNotSame(VECTOR, vectorDatum.CFrontVector());
            Assert::AreNotSame(VECTOR, vectorDatum.CBackVector());
            Assert::AreNotSame(VECTOR, vectorDatum.CGetVectorElement());
            Assert::AreEqual(MATRIX, matrixDatum.CFrontMatrix());
            Assert::AreEqual(MATRIX, matrixDatum.CBackMatrix());
            Assert::AreEqual(MATRIX, matrixDatum.CGetMatrixElement());
            Assert::AreNotSame(MATRIX, matrixDatum.CFrontMatrix());
            Assert::AreNotSame(MATRIX, matrixDatum.CBackMatrix());
            Assert::AreNotSame(MATRIX, matrixDatum.CGetMatrixElement());
            Assert::IsTrue(stringDatum.IsContaining(STRING));
            Assert::IsTrue(vectorDatum.IsContaining(VECTOR));
            Assert::IsTrue(matrixDatum.IsContaining(MATRIX));

            Assert::AreEqual(STRING, stringDatum.ElementToString());

            char buffer[200];
            sprintf_s(buffer, 200, Datum::VECTOR_STRING_FORMAT.c_str(), VECTOR.x, VECTOR.y, VECTOR.z, VECTOR.w);

            Assert::AreEqual(String{buffer}, vectorDatum.ElementToString());

            const Matrix::row_type& R0 = MATRIX[0];
            const Matrix::row_type& R1 = MATRIX[1];
            const Matrix::row_type& R2 = MATRIX[2];
            const Matrix::row_type& R3 = MATRIX[3];
            sprintf_s(buffer, 200, Datum::MATRIX_STRING_FORMAT.c_str(),
                R0.x, R0.y, R0.z, R0.w,
                R1.x, R1.y, R1.z, R1.w,
                R2.x, R2.y, R2.z, R2.w,
                R3.x, R3.y, R3.z, R3.w
            );

            Assert::AreEqual(String{buffer}, matrixDatum.ElementToString());
        }

        TEST_METHOD(InitializerListConstructor) {
            const size_type THREE = size_type(3);
            const Integer INTEGER0 = 15;
            const Integer INTEGER1 = 16;
            const Integer INTEGER2 = 17;
            const Float FLOAT0 = 18.f;
            const Float FLOAT1 = 19.f;
            const Float FLOAT2 = 20.f;
            const String STRING0 = "21"s;
            const String STRING1 = "22"s;
            const String STRING2 = "23"s;
            const Vector VECTOR0{24.f, 25.f, 26.f, 27.f};
            const Vector VECTOR1{28.f, 29.f, 30.f, 31.f};
            const Vector VECTOR2{32.f, 33.f, 34.f, 35.f};
            const Matrix MATRIX0{VECTOR0, VECTOR0, VECTOR0, VECTOR0};
            const Matrix MATRIX1{VECTOR1, VECTOR1, VECTOR1, VECTOR1};
            const Matrix MATRIX2{VECTOR2, VECTOR2, VECTOR2, VECTOR2};
            Foo foo0{36};
            Foo foo1{37};
            Foo foo2{38};

            Datum integerDatum{INTEGER0, INTEGER1, INTEGER2};
            Datum floatDatum{FLOAT0, FLOAT1, FLOAT2};
            Datum stringDatum{STRING0, STRING1, STRING2};
            Datum vectorDatum{VECTOR0, VECTOR1, VECTOR2};
            Datum matrixDatum{MATRIX0, MATRIX1, MATRIX2};
            Datum pointerDatum{&foo0, &foo1, &foo2};

            Assert::AreEqual(DatumType::Integer, integerDatum.ApparentType());
            Assert::AreEqual(DatumType::Float, floatDatum.ApparentType());
            Assert::AreEqual(DatumType::String, stringDatum.ApparentType());
            Assert::AreEqual(DatumType::Vector, vectorDatum.ApparentType());
            Assert::AreEqual(DatumType::Matrix, matrixDatum.ApparentType());
            Assert::AreEqual(DatumType::Pointer, pointerDatum.ApparentType());
            Assert::IsTrue(integerDatum.IsDataInternal());
            Assert::IsTrue(floatDatum.IsDataInternal());
            Assert::IsTrue(stringDatum.IsDataInternal());
            Assert::IsTrue(vectorDatum.IsDataInternal());
            Assert::IsTrue(matrixDatum.IsDataInternal());
            Assert::IsTrue(pointerDatum.IsDataInternal());
            Assert::IsFalse(integerDatum.IsEmpty());
            Assert::IsFalse(floatDatum.IsEmpty());
            Assert::IsFalse(stringDatum.IsEmpty());
            Assert::IsFalse(vectorDatum.IsEmpty());
            Assert::IsFalse(matrixDatum.IsEmpty());
            Assert::IsFalse(pointerDatum.IsEmpty());
            Assert::AreEqual(THREE, integerDatum.Capacity());
            Assert::AreEqual(THREE, floatDatum.Capacity());
            Assert::AreEqual(THREE, stringDatum.Capacity());
            Assert::AreEqual(THREE, vectorDatum.Capacity());
            Assert::AreEqual(THREE, matrixDatum.Capacity());
            Assert::AreEqual(THREE, pointerDatum.Capacity());
            Assert::AreEqual(THREE, integerDatum.Size());
            Assert::AreEqual(THREE, floatDatum.Size());
            Assert::AreEqual(THREE, stringDatum.Size());
            Assert::AreEqual(THREE, vectorDatum.Size());
            Assert::AreEqual(THREE, matrixDatum.Size());
            Assert::AreEqual(THREE, pointerDatum.Size());
            Assert::AreEqual(INTEGER0, integerDatum.FrontInteger());
            Assert::IsTrue(integerDatum.IsContaining(INTEGER1));
            Assert::AreEqual(INTEGER2, integerDatum.BackInteger());
            Assert::AreEqual(FLOAT0, floatDatum.FrontFloat());
            Assert::IsTrue(floatDatum.IsContaining(FLOAT1));
            Assert::AreEqual(FLOAT2, floatDatum.BackFloat());
            Assert::AreEqual(STRING0, stringDatum.FrontString());
            Assert::IsTrue(stringDatum.IsContaining(STRING1));
            Assert::AreEqual(STRING2, stringDatum.BackString());
            Assert::AreEqual(VECTOR0, vectorDatum.FrontVector());
            Assert::IsTrue(vectorDatum.IsContaining(VECTOR1));
            Assert::AreEqual(VECTOR2, vectorDatum.BackVector());
            Assert::AreEqual(MATRIX0, matrixDatum.FrontMatrix());
            Assert::IsTrue(matrixDatum.IsContaining(MATRIX1));
            Assert::AreEqual(MATRIX2, matrixDatum.BackMatrix());
            Assert::IsTrue(&foo0 == pointerDatum.FrontPointer());
            Assert::IsTrue(pointerDatum.IsContaining(&foo1));
            Assert::IsTrue(&foo2 == pointerDatum.BackPointer());
        }

        TEST_METHOD(CopyScalarAssignment) {
            const size_type ONE = size_type(1);
            const Integer INTEGER = 7;
            const Float FLOAT = 8.f;
            const String STRING = "9"s;
            const Vector VECTOR{10.f, 11.f, 12.f, 13.f};
            const Matrix MATRIX{VECTOR, VECTOR, VECTOR, VECTOR};

            Foo foo{14};

            Datum datum{};
            datum = INTEGER;

            Assert::AreEqual(DatumType::Integer, datum.ApparentType());
            Assert::IsTrue(datum.IsDataInternal());
            Assert::IsFalse(datum.IsEmpty());
            Assert::AreEqual(ONE, datum.Capacity());
            Assert::AreEqual(ONE, datum.Size());
            Assert::IsTrue(datum == INTEGER);
            Assert::AreEqual(INTEGER, datum.FrontInteger());
            Assert::AreEqual(INTEGER, datum.BackInteger());
            Assert::AreEqual(INTEGER, datum.GetIntegerElement());
            Assert::AreNotSame(INTEGER, datum.FrontInteger());
            Assert::AreNotSame(INTEGER, datum.BackInteger());
            Assert::AreNotSame(INTEGER, datum.GetIntegerElement());
            Assert::AreEqual(INTEGER, datum.CFrontInteger());
            Assert::AreEqual(INTEGER, datum.CBackInteger());
            Assert::AreEqual(INTEGER, datum.CGetIntegerElement());
            Assert::AreNotSame(INTEGER, datum.CFrontInteger());
            Assert::AreNotSame(INTEGER, datum.CBackInteger());
            Assert::AreNotSame(INTEGER, datum.CGetIntegerElement());
            Assert::IsTrue(datum.IsContaining(INTEGER));

            char buffer[200];
            sprintf_s(buffer, 200, "%d", INTEGER);

            Assert::AreEqual(String{buffer}, datum.ElementToString());

            datum = FLOAT;

            Assert::AreEqual(DatumType::Float, datum.ApparentType());
            Assert::IsTrue(datum.IsDataInternal());
            Assert::IsFalse(datum.IsEmpty());
            Assert::AreEqual(ONE, datum.Capacity());
            Assert::AreEqual(ONE, datum.Size());
            Assert::IsTrue(datum == FLOAT);
            Assert::AreEqual(FLOAT, datum.FrontFloat());
            Assert::AreEqual(FLOAT, datum.BackFloat());
            Assert::AreEqual(FLOAT, datum.GetFloatElement());
            Assert::AreNotSame(FLOAT, datum.FrontFloat());
            Assert::AreNotSame(FLOAT, datum.BackFloat());
            Assert::AreNotSame(FLOAT, datum.GetFloatElement());
            Assert::AreEqual(FLOAT, datum.CFrontFloat());
            Assert::AreEqual(FLOAT, datum.CBackFloat());
            Assert::AreEqual(FLOAT, datum.CGetFloatElement());
            Assert::AreNotSame(FLOAT, datum.CFrontFloat());
            Assert::AreNotSame(FLOAT, datum.CBackFloat());
            Assert::AreNotSame(FLOAT, datum.CGetFloatElement());

            sprintf_s(buffer, 200, "%F", FLOAT);

            Assert::AreEqual(String{buffer}, datum.ElementToString());
            Assert::IsTrue(datum.IsContaining(FLOAT));

            datum = STRING;

            Assert::AreEqual(DatumType::String, datum.ApparentType());
            Assert::IsTrue(datum.IsDataInternal());
            Assert::IsFalse(datum.IsEmpty());
            Assert::AreEqual(ONE, datum.Capacity());
            Assert::AreEqual(ONE, datum.Size());
            Assert::IsTrue(datum == STRING);
            Assert::AreEqual(STRING, datum.FrontString());
            Assert::AreEqual(STRING, datum.BackString());
            Assert::AreEqual(STRING, datum.GetStringElement());
            Assert::AreNotSame(STRING, datum.FrontString());
            Assert::AreNotSame(STRING, datum.BackString());
            Assert::AreNotSame(STRING, datum.GetStringElement());
            Assert::AreEqual(STRING, datum.CFrontString());
            Assert::AreEqual(STRING, datum.CBackString());
            Assert::AreEqual(STRING, datum.CGetStringElement());
            Assert::AreNotSame(STRING, datum.CFrontString());
            Assert::AreNotSame(STRING, datum.CBackString());
            Assert::AreNotSame(STRING, datum.CGetStringElement());
            Assert::AreEqual(STRING, datum.ElementToString());
            Assert::IsTrue(datum.IsContaining(STRING));

            datum = VECTOR;

            Assert::AreEqual(DatumType::Vector, datum.ApparentType());
            Assert::IsTrue(datum.IsDataInternal());
            Assert::IsFalse(datum.IsEmpty());
            Assert::AreEqual(ONE, datum.Capacity());
            Assert::AreEqual(ONE, datum.Size());
            Assert::IsTrue(datum == VECTOR);
            Assert::AreEqual(VECTOR, datum.FrontVector());
            Assert::AreEqual(VECTOR, datum.BackVector());
            Assert::AreEqual(VECTOR, datum.GetVectorElement());
            Assert::AreNotSame(VECTOR, datum.FrontVector());
            Assert::AreNotSame(VECTOR, datum.BackVector());
            Assert::AreNotSame(VECTOR, datum.GetVectorElement());
            Assert::AreEqual(VECTOR, datum.CFrontVector());
            Assert::AreEqual(VECTOR, datum.CBackVector());
            Assert::AreEqual(VECTOR, datum.CGetVectorElement());
            Assert::AreNotSame(VECTOR, datum.CFrontVector());
            Assert::AreNotSame(VECTOR, datum.CBackVector());
            Assert::AreNotSame(VECTOR, datum.CGetVectorElement());

            sprintf_s(buffer, 200, Datum::VECTOR_STRING_FORMAT.c_str(), VECTOR.x, VECTOR.y, VECTOR.z, VECTOR.w);

            Assert::AreEqual(String{buffer}, datum.ElementToString());
            Assert::IsTrue(datum.IsContaining(VECTOR));

            datum = MATRIX;

            Assert::AreEqual(DatumType::Matrix, datum.ApparentType());
            Assert::IsTrue(datum.IsDataInternal());
            Assert::IsFalse(datum.IsEmpty());
            Assert::AreEqual(ONE, datum.Capacity());
            Assert::AreEqual(ONE, datum.Size());
            Assert::IsTrue(datum == MATRIX);
            Assert::AreEqual(MATRIX, datum.FrontMatrix());
            Assert::AreEqual(MATRIX, datum.BackMatrix());
            Assert::AreEqual(MATRIX, datum.GetMatrixElement());
            Assert::AreNotSame(MATRIX, datum.FrontMatrix());
            Assert::AreNotSame(MATRIX, datum.BackMatrix());
            Assert::AreNotSame(MATRIX, datum.GetMatrixElement());
            Assert::AreEqual(MATRIX, datum.CFrontMatrix());
            Assert::AreEqual(MATRIX, datum.CBackMatrix());
            Assert::AreEqual(MATRIX, datum.CGetMatrixElement());
            Assert::AreNotSame(MATRIX, datum.CFrontMatrix());
            Assert::AreNotSame(MATRIX, datum.CBackMatrix());
            Assert::AreNotSame(MATRIX, datum.CGetMatrixElement());

            const Matrix::row_type& R0 = MATRIX[0];
            const Matrix::row_type& R1 = MATRIX[1];
            const Matrix::row_type& R2 = MATRIX[2];
            const Matrix::row_type& R3 = MATRIX[3];
            sprintf_s(buffer, 200, Datum::MATRIX_STRING_FORMAT.c_str(),
                R0.x, R0.y, R0.z, R0.w,
                R1.x, R1.y, R1.z, R1.w,
                R2.x, R2.y, R2.z, R2.w,
                R3.x, R3.y, R3.z, R3.w
            );

            Assert::AreEqual(String{buffer}, datum.ElementToString());
            Assert::IsTrue(datum.IsContaining(MATRIX));

            datum = &foo;

            Assert::AreEqual(DatumType::Pointer, datum.ApparentType());
            Assert::IsTrue(datum.IsDataInternal());
            Assert::IsFalse(datum.IsEmpty());
            Assert::AreEqual(ONE, datum.Capacity());
            Assert::AreEqual(ONE, datum.Size());
            Assert::IsTrue(datum == &foo);
            Assert::IsTrue(foo.Equals(datum.FrontPointer()));
            Assert::IsTrue(foo.Equals(datum.BackPointer()));
            Assert::IsTrue(foo.Equals(datum.GetPointerElement()));
            Assert::IsTrue(&foo == datum.FrontPointer());
            Assert::IsTrue(&foo == datum.BackPointer());
            Assert::IsTrue(&foo == datum.GetPointerElement());
            Assert::IsTrue(foo.Equals(datum.CFrontPointer()));
            Assert::IsTrue(foo.Equals(datum.CBackPointer()));
            Assert::IsTrue(foo.Equals(datum.CGetPointerElement()));
            Assert::IsTrue(&foo == datum.CFrontPointer());
            Assert::IsTrue(&foo == datum.CBackPointer());
            Assert::IsTrue(&foo == datum.CGetPointerElement());
            Assert::IsTrue(datum.IsContaining(&foo));
            Assert::AreEqual(foo.ToString(), datum.ElementToString());
        }

        TEST_METHOD(MoveScalarAssignment) {
            const size_type ONE = size_type(1);
            const String STRING = "9"s;
            const Vector VECTOR{10.f, 11.f, 12.f, 13.f};
            const Matrix MATRIX{VECTOR, VECTOR, VECTOR, VECTOR};

            String copyString{STRING};
            Vector copyVector{VECTOR};
            Matrix copyMatrix{MATRIX};

            Datum datum{};
            datum = std::move(copyString);

            Assert::AreEqual(DatumType::String, datum.ApparentType());
            Assert::IsTrue(datum.IsDataInternal());
            Assert::IsFalse(datum.IsEmpty());
            Assert::AreEqual(ONE, datum.Capacity());
            Assert::AreEqual(ONE, datum.Size());
            Assert::IsTrue(datum == STRING);
            Assert::AreEqual(STRING, datum.FrontString());
            Assert::AreEqual(STRING, datum.BackString());
            Assert::AreEqual(STRING, datum.GetStringElement());
            Assert::AreNotSame(STRING, datum.FrontString());
            Assert::AreNotSame(STRING, datum.BackString());
            Assert::AreNotSame(STRING, datum.GetStringElement());
            Assert::AreEqual(STRING, datum.CFrontString());
            Assert::AreEqual(STRING, datum.CBackString());
            Assert::AreEqual(STRING, datum.CGetStringElement());
            Assert::AreNotSame(STRING, datum.CFrontString());
            Assert::AreNotSame(STRING, datum.CBackString());
            Assert::AreNotSame(STRING, datum.CGetStringElement());
            Assert::IsTrue(datum.IsContaining(STRING));

            Assert::AreEqual(STRING, datum.ElementToString());

            datum = std::move(copyVector);

            Assert::AreEqual(DatumType::Vector, datum.ApparentType());
            Assert::IsTrue(datum.IsDataInternal());
            Assert::IsFalse(datum.IsEmpty());
            Assert::AreEqual(ONE, datum.Capacity());
            Assert::AreEqual(ONE, datum.Size());
            Assert::IsTrue(datum == VECTOR);
            Assert::AreEqual(VECTOR, datum.FrontVector());
            Assert::AreEqual(VECTOR, datum.BackVector());
            Assert::AreEqual(VECTOR, datum.GetVectorElement());
            Assert::AreNotSame(VECTOR, datum.FrontVector());
            Assert::AreNotSame(VECTOR, datum.BackVector());
            Assert::AreNotSame(VECTOR, datum.GetVectorElement());
            Assert::AreEqual(VECTOR, datum.CFrontVector());
            Assert::AreEqual(VECTOR, datum.CBackVector());
            Assert::AreEqual(VECTOR, datum.CGetVectorElement());
            Assert::AreNotSame(VECTOR, datum.CFrontVector());
            Assert::AreNotSame(VECTOR, datum.CBackVector());
            Assert::AreNotSame(VECTOR, datum.CGetVectorElement());
            Assert::IsTrue(datum.IsContaining(VECTOR));

            char buffer[200];
            sprintf_s(buffer, 200, Datum::VECTOR_STRING_FORMAT.c_str(), VECTOR.x, VECTOR.y, VECTOR.z, VECTOR.w);

            Assert::AreEqual(String{buffer}, datum.ElementToString());

            datum = std::move(copyMatrix);

            Assert::AreEqual(DatumType::Matrix, datum.ApparentType());
            Assert::IsTrue(datum.IsDataInternal());
            Assert::IsFalse(datum.IsEmpty());
            Assert::AreEqual(ONE, datum.Capacity());
            Assert::AreEqual(ONE, datum.Size());
            Assert::IsTrue(datum == MATRIX);
            Assert::AreEqual(MATRIX, datum.FrontMatrix());
            Assert::AreEqual(MATRIX, datum.BackMatrix());
            Assert::AreEqual(MATRIX, datum.GetMatrixElement());
            Assert::AreNotSame(MATRIX, datum.FrontMatrix());
            Assert::AreNotSame(MATRIX, datum.BackMatrix());
            Assert::AreNotSame(MATRIX, datum.GetMatrixElement());
            Assert::AreEqual(MATRIX, datum.CFrontMatrix());
            Assert::AreEqual(MATRIX, datum.CBackMatrix());
            Assert::AreEqual(MATRIX, datum.CGetMatrixElement());
            Assert::AreNotSame(MATRIX, datum.CFrontMatrix());
            Assert::AreNotSame(MATRIX, datum.CBackMatrix());
            Assert::AreNotSame(MATRIX, datum.CGetMatrixElement());
            Assert::IsTrue(datum.IsContaining(MATRIX));

            const Matrix::row_type& R0 = MATRIX[0];
            const Matrix::row_type& R1 = MATRIX[1];
            const Matrix::row_type& R2 = MATRIX[2];
            const Matrix::row_type& R3 = MATRIX[3];
            sprintf_s(buffer, 200, Datum::MATRIX_STRING_FORMAT.c_str(),
                R0.x, R0.y, R0.z, R0.w,
                R1.x, R1.y, R1.z, R1.w,
                R2.x, R2.y, R2.z, R2.w,
                R3.x, R3.y, R3.z, R3.w
            );

            Assert::AreEqual(String{buffer}, datum.ElementToString());
        }

        TEST_METHOD(InitializerListAssignment) {
            const size_type THREE = size_type(3);
            const Integer INTEGER0 = 15;
            const Integer INTEGER1 = 16;
            const Integer INTEGER2 = 17;
            const Float FLOAT0 = 18.f;
            const Float FLOAT1 = 19.f;
            const Float FLOAT2 = 20.f;
            const String STRING0 = "21"s;
            const String STRING1 = "22"s;
            const String STRING2 = "23"s;
            const Vector VECTOR0{24.f, 25.f, 26.f, 27.f};
            const Vector VECTOR1{28.f, 29.f, 30.f, 31.f};
            const Vector VECTOR2{32.f, 33.f, 34.f, 35.f};
            const Matrix MATRIX0{VECTOR0, VECTOR0, VECTOR0, VECTOR0};
            const Matrix MATRIX1{VECTOR1, VECTOR1, VECTOR1, VECTOR1};
            const Matrix MATRIX2{VECTOR2, VECTOR2, VECTOR2, VECTOR2};
            Foo foo0{36};
            Foo foo1{37};
            Foo foo2{38};

            Datum datum{};
            datum = {INTEGER0, INTEGER1, INTEGER2};

            Assert::AreEqual(DatumType::Integer, datum.ApparentType());
            Assert::IsTrue(datum.IsDataInternal());
            Assert::IsFalse(datum.IsEmpty());
            Assert::AreEqual(THREE, datum.Capacity());
            Assert::AreEqual(THREE, datum.Size());
            Assert::AreEqual(INTEGER0, datum.FrontInteger());
            Assert::IsTrue(datum.IsContaining(INTEGER1));
            Assert::AreEqual(INTEGER2, datum.BackInteger());

            datum = {FLOAT0, FLOAT1, FLOAT2};

            Assert::AreEqual(DatumType::Float, datum.ApparentType());
            Assert::IsTrue(datum.IsDataInternal());
            Assert::IsFalse(datum.IsEmpty());
            Assert::AreEqual(THREE, datum.Capacity());
            Assert::AreEqual(THREE, datum.Size());
            Assert::AreEqual(FLOAT0, datum.FrontFloat());
            Assert::IsTrue(datum.IsContaining(FLOAT1));
            Assert::AreEqual(FLOAT2, datum.BackFloat());

            datum = {STRING0, STRING1, STRING2};

            Assert::AreEqual(DatumType::String, datum.ApparentType());
            Assert::IsTrue(datum.IsDataInternal());
            Assert::IsFalse(datum.IsEmpty());
            Assert::AreEqual(THREE, datum.Capacity());
            Assert::AreEqual(THREE, datum.Size());
            Assert::AreEqual(STRING0, datum.FrontString());
            Assert::IsTrue(datum.IsContaining(STRING1));
            Assert::AreEqual(STRING2, datum.BackString());

            datum = {VECTOR0, VECTOR1, VECTOR2};

            Assert::AreEqual(DatumType::Vector, datum.ApparentType());
            Assert::IsTrue(datum.IsDataInternal());
            Assert::IsFalse(datum.IsEmpty());
            Assert::AreEqual(THREE, datum.Capacity());
            Assert::AreEqual(THREE, datum.Size());
            Assert::AreEqual(VECTOR0, datum.FrontVector());
            Assert::IsTrue(datum.IsContaining(VECTOR1));
            Assert::AreEqual(VECTOR2, datum.BackVector());

            datum = {MATRIX0, MATRIX1, MATRIX2};

            Assert::AreEqual(DatumType::Matrix, datum.ApparentType());
            Assert::IsTrue(datum.IsDataInternal());
            Assert::IsFalse(datum.IsEmpty());
            Assert::AreEqual(THREE, datum.Capacity());
            Assert::AreEqual(THREE, datum.Size());
            Assert::AreEqual(MATRIX0, datum.FrontMatrix());
            Assert::IsTrue(datum.IsContaining(MATRIX1));
            Assert::AreEqual(MATRIX2, datum.BackMatrix());

            datum = {&foo0, &foo1, &foo2};

            Assert::AreEqual(DatumType::Pointer, datum.ApparentType());
            Assert::IsTrue(datum.IsDataInternal());
            Assert::IsFalse(datum.IsEmpty());
            Assert::AreEqual(THREE, datum.Capacity());
            Assert::AreEqual(THREE, datum.Size());
            Assert::IsTrue(&foo0 == datum.FrontPointer());
            Assert::IsTrue(datum.IsContaining(&foo1));
            Assert::IsTrue(&foo2 == datum.BackPointer());
        }

        TEST_METHOD(IntegerDatum) {
            Datum datum{};
            datum.PushBack(0);
            datum.PushBack(1);
            datum.PushBack(2);
            datum.PushBack(3);

            Assert::AreEqual(DatumType::Integer, datum.ApparentType());
            Assert::AreEqual(size_type(4), datum.Size());
            Assert::IsTrue(datum.IsContaining(1));

            datum.GetIntegerElement(size_type(1)) = 4;

            Assert::IsFalse(datum.IsContaining(1));
            Assert::IsTrue(datum.IsContaining(4));
            Assert::IsTrue(datum.IsContaining(3));

            datum.SetElement(5, size_type(3));

            Assert::IsFalse(datum.IsContaining(3));
            Assert::IsTrue(datum.IsContaining(5));
            Assert::IsTrue(datum.IsContaining(2));

            datum.SetElementFromString("6"s, size_type(2));

            Assert::IsFalse(datum.IsContaining(2));
            Assert::IsTrue(datum.IsContaining(6));
            Assert::AreEqual(0, datum.FrontInteger());

            datum.Remove(0);

            Assert::AreNotEqual(0, datum.FrontInteger());
            Assert::AreEqual(4, datum.FrontInteger());
            Assert::AreEqual(size_type(3), datum.Size());
            Assert::AreEqual(5, datum.BackInteger());

            datum.RemoveAt(size_type(2));

            Assert::AreNotEqual(5, datum.BackInteger());
            Assert::AreEqual(6, datum.BackInteger());
        }

        TEST_METHOD(FloatDatum) {
            Datum datum{};
            datum.PushBack(0.f);
            datum.PushBack(1.f);
            datum.PushBack(2.f);
            datum.PushBack(3.f);

            Assert::AreEqual(DatumType::Float, datum.ApparentType());
            Assert::AreEqual(size_type(4), datum.Size());
            Assert::IsTrue(datum.IsContaining(1.f));

            datum.GetFloatElement(size_type(1)) = 4.f;

            Assert::IsFalse(datum.IsContaining(1.f));
            Assert::IsTrue(datum.IsContaining(4.f));
            Assert::IsTrue(datum.IsContaining(3.f));

            datum.SetElement(5.f, size_type(3));

            Assert::IsFalse(datum.IsContaining(3.f));
            Assert::IsTrue(datum.IsContaining(5.f));
            Assert::IsTrue(datum.IsContaining(2.f));

            datum.SetElementFromString("6"s, size_type(2));

            Assert::IsFalse(datum.IsContaining(2.f));
            Assert::IsTrue(datum.IsContaining(6.f));
            Assert::AreEqual(0.f, datum.FrontFloat());

            datum.Remove(0.f);

            Assert::AreNotEqual(0.f, datum.FrontFloat());
            Assert::AreEqual(4.f, datum.FrontFloat());
            Assert::AreEqual(size_type(3), datum.Size());
            Assert::AreEqual(5.f, datum.BackFloat());

            datum.RemoveAt(size_type(2));

            Assert::AreNotEqual(5.f, datum.BackFloat());
            Assert::AreEqual(6.f, datum.BackFloat());
        }

        TEST_METHOD(StringDatum) {
            const String ZERO = "0"s;
            const String ONE = "1.0"s;
            const String TWO = "2.00"s;
            const String THREE = "3.000"s;
            const String FOUR = "4.0000"s;
            const String FIVE = "5.00000"s;
            const String SIX = "6"s;

            String local = ONE;

            Datum datum{};
            datum.PushBack(ZERO);
            datum.PushBack(std::move(local));
            datum.EmplaceBackString("2.00");
            datum.PushBack(THREE);

            Assert::AreEqual(DatumType::String, datum.ApparentType());
            Assert::AreEqual(size_type(4), datum.Size());
            Assert::IsTrue(datum.IsContaining(ONE));

            datum.GetStringElement(size_type(1)) = FOUR;

            Assert::IsFalse(datum.IsContaining(ONE));
            Assert::IsTrue(datum.IsContaining(FOUR));
            Assert::IsTrue(datum.IsContaining(THREE));

            datum.SetElement(FIVE, size_type(3));

            Assert::IsFalse(datum.IsContaining(THREE));
            Assert::IsTrue(datum.IsContaining(FIVE));
            Assert::IsTrue(datum.IsContaining(TWO));

            datum.SetElementFromString(SIX, size_type(2));

            Assert::IsFalse(datum.IsContaining(TWO));
            Assert::IsTrue(datum.IsContaining(SIX));
            Assert::AreEqual(ZERO, datum.FrontString());

            datum.Remove(ZERO);

            Assert::AreNotEqual(ZERO, datum.FrontString());
            Assert::AreEqual(FOUR, datum.FrontString());
            Assert::AreEqual(size_type(3), datum.Size());
            Assert::AreEqual(FIVE, datum.BackString());

            datum.RemoveAt(size_type(2));

            Assert::AreNotEqual(FIVE, datum.BackString());
            Assert::AreEqual(SIX, datum.BackString());
        }

        TEST_METHOD(VectorDatum) {
            const Vector ZERO{0.f, 1.f, 2.f, 3.f};
            const Vector ONE{1.f, 2.f, 3.f, 4.f};
            const Vector TWO{2.f, 3.f, 4.f, 5.f};
            const Vector THREE{3.f, 4.f, 5.f, 6.f};
            const Vector FOUR{4.f, 5.f, 6.f, 7.f};
            const Vector FIVE{5.f, 6.f, 7.f, 8.f};
            const Vector SIX{6.f, 7.f, 8.f, 9.f};

            Vector local{ONE};

            Datum datum{};
            datum.PushBack(ZERO);
            datum.PushBack(std::move(local));
            datum.EmplaceBackVector(TWO.x, TWO.y, TWO.z, TWO.w);
            datum.PushBack(THREE);

            Assert::AreEqual(DatumType::Vector, datum.ApparentType());
            Assert::AreEqual(size_type(4), datum.Size());
            Assert::IsTrue(datum.IsContaining(ONE));

            datum.GetVectorElement(size_type(1)) = FOUR;

            Assert::IsFalse(datum.IsContaining(ONE));
            Assert::IsTrue(datum.IsContaining(FOUR));
            Assert::IsTrue(datum.IsContaining(THREE));

            datum.SetElement(FIVE, size_type(3));

            Assert::IsFalse(datum.IsContaining(THREE));
            Assert::IsTrue(datum.IsContaining(FIVE));
            Assert::IsTrue(datum.IsContaining(TWO));

            char buffer[50];
            sprintf_s(buffer, 50, Datum::VECTOR_STRING_FORMAT.c_str(), SIX.x, SIX.y, SIX.z, SIX.w);

            datum.SetElementFromString(String{buffer}, size_type(2));

            Assert::IsFalse(datum.IsContaining(TWO));
            Assert::IsTrue(datum.IsContaining(SIX));
            Assert::AreEqual(ZERO, datum.FrontVector());

            datum.Remove(ZERO);

            Assert::AreNotEqual(ZERO, datum.FrontVector());
            Assert::AreEqual(FOUR, datum.FrontVector());
            Assert::AreEqual(size_type(3), datum.Size());
            Assert::AreEqual(FIVE, datum.BackVector());

            datum.RemoveAt(size_type(2));

            Assert::AreNotEqual(FIVE, datum.BackVector());
            Assert::AreEqual(SIX, datum.BackVector());
        }

        TEST_METHOD(MatrixDatum) {
            const Matrix::row_type ZERO_ROW{0.f, 1.f, 2.f, 3.f};
            const Matrix::row_type ONE_ROW{1.f, 2.f, 3.f, 4.f};
            const Matrix::row_type TWO_ROW{2.f, 3.f, 4.f, 5.f};
            const Matrix::row_type THREE_ROW{3.f, 4.f, 5.f, 6.f};
            const Matrix::row_type FOUR_ROW{4.f, 5.f, 6.f, 7.f};
            const Matrix::row_type FIVE_ROW{5.f, 6.f, 7.f, 8.f};
            const Matrix::row_type SIX_ROW{6.f, 7.f, 8.f, 9.f};

            const Matrix ZERO{ZERO_ROW, ZERO_ROW, ZERO_ROW, ZERO_ROW};
            const Matrix ONE{ONE_ROW, ONE_ROW, ONE_ROW, ONE_ROW};
            const Matrix TWO{TWO_ROW, TWO_ROW, TWO_ROW, TWO_ROW};
            const Matrix THREE{THREE_ROW, THREE_ROW, THREE_ROW, THREE_ROW};
            const Matrix FOUR{FOUR_ROW, FOUR_ROW, FOUR_ROW, FOUR_ROW};
            const Matrix FIVE{FIVE_ROW, FIVE_ROW, FIVE_ROW, FIVE_ROW};
            const Matrix SIX{SIX_ROW, SIX_ROW, SIX_ROW, SIX_ROW};

            Matrix local{ONE};

            Datum datum{};
            datum.PushBack(ZERO);
            datum.PushBack(std::move(local));
            datum.EmplaceBackMatrix(TWO_ROW, TWO_ROW, TWO_ROW, TWO_ROW);
            datum.PushBack(THREE);

            Assert::AreEqual(DatumType::Matrix, datum.ApparentType());
            Assert::AreEqual(size_type(4), datum.Size());
            Assert::IsTrue(datum.IsContaining(ONE));

            datum.GetMatrixElement(size_type(1)) = FOUR;

            Assert::IsFalse(datum.IsContaining(ONE));
            Assert::IsTrue(datum.IsContaining(FOUR));
            Assert::IsTrue(datum.IsContaining(THREE));

            datum.SetElement(FIVE, size_type(3));

            Assert::IsFalse(datum.IsContaining(THREE));
            Assert::IsTrue(datum.IsContaining(FIVE));
            Assert::IsTrue(datum.IsContaining(TWO));

            char buffer[200];
            sprintf_s(buffer, 200, Datum::MATRIX_STRING_FORMAT.c_str(),
                SIX_ROW.x, SIX_ROW.y, SIX_ROW.z, SIX_ROW.w,
                SIX_ROW.x, SIX_ROW.y, SIX_ROW.z, SIX_ROW.w,
                SIX_ROW.x, SIX_ROW.y, SIX_ROW.z, SIX_ROW.w,
                SIX_ROW.x, SIX_ROW.y, SIX_ROW.z, SIX_ROW.w);

            datum.SetElementFromString(String{buffer}, size_type(2));

            Assert::IsFalse(datum.IsContaining(TWO));
            Assert::IsTrue(datum.IsContaining(SIX));
            Assert::AreEqual(ZERO, datum.FrontMatrix());

            datum.Remove(ZERO);

            Assert::AreNotEqual(ZERO, datum.FrontMatrix());
            Assert::AreEqual(FOUR, datum.FrontMatrix());
            Assert::AreEqual(size_type(3), datum.Size());
            Assert::AreEqual(FIVE, datum.BackMatrix());

            datum.RemoveAt(size_type(2));

            Assert::AreNotEqual(FIVE, datum.BackMatrix());
            Assert::AreEqual(SIX, datum.BackMatrix());
        }

        TEST_METHOD(PointerDatum) {
            Foo ZERO{0};
            Foo ONE{1};
            Foo TWO{2};
            Foo THREE{3};
            Foo FOUR{4};
            Foo FIVE{5};
            Foo SIX{6};

            Datum datum{};
            datum.PushBack(&ZERO);
            datum.PushBack(&ONE);
            datum.PushBack(&TWO);
            datum.PushBack(&THREE);

            Assert::AreEqual(DatumType::Pointer, datum.ApparentType());
            Assert::AreEqual(size_type(4), datum.Size());
            Assert::IsTrue(datum.IsContaining(&ONE));

            datum.GetPointerElement(size_type(1)) = &FOUR;

            Assert::IsFalse(datum.IsContaining(&ONE));
            Assert::IsTrue(datum.IsContaining(&FOUR));
            Assert::IsTrue(datum.IsContaining(&THREE));

            datum.SetElement(&FIVE, size_type(3));

            Assert::IsFalse(datum.IsContaining(&THREE));
            Assert::IsTrue(datum.IsContaining(&FIVE));
            Assert::IsTrue(datum.IsContaining(&TWO));

            Assert::ExpectException<std::invalid_argument>([&datum](){ datum.SetElementFromString("6"s, size_type(2)); });
            datum.SetElement(&SIX, size_type(2));

            Assert::IsFalse(datum.IsContaining(&TWO));
            Assert::IsTrue(datum.IsContaining(&SIX));
            Assert::IsTrue(ZERO.Equals(datum.FrontPointer()));

            datum.Remove(&ZERO);

            Assert::IsFalse(ZERO.Equals(datum.FrontPointer()));
            Assert::IsTrue(FOUR.Equals(datum.FrontPointer()));
            Assert::AreEqual(size_type(3), datum.Size());
            Assert::IsTrue(FIVE.Equals(datum.BackPointer()));

            datum.RemoveAt(size_type(2));

            Assert::IsFalse(FIVE.Equals(datum.BackPointer()));
            Assert::IsTrue(SIX.Equals(datum.BackPointer()));
        }

        TEST_METHOD(CannotSetTypeToUnknown) {
            Datum datum = 1;
            Assert::ExpectException<std::invalid_argument>([&datum](){ datum.SetType(DatumType::Unknown); });
        }

        TEST_METHOD(NonzeroShrink) {
            const size_type BIG = size_type(10);
            const size_type TWO = size_type(2);

            Datum datum = "Hello"s;

            datum.Reserve(BIG);

            Assert::AreEqual(BIG, datum.Capacity());

            datum.PushBack("World"s);

            Assert::AreEqual(TWO, datum.Size());
            Assert::AreNotEqual(datum.Size(), datum.Capacity());
            Assert::AreNotEqual(TWO, datum.Capacity());

            datum.ShrinkToFit();

            Assert::AreEqual(datum.Size(), datum.Capacity());
            Assert::AreEqual(TWO, datum.Capacity());
        }

        TEST_METHOD(PopBack) {
            Datum datum{1, 2, 3};

            Assert::AreEqual(size_type(3), datum.Size());
            Assert::AreEqual(3, datum.CBackInteger());

            datum.PopBack();

            Assert::AreEqual(size_type(2), datum.Size());
            Assert::AreEqual(2, datum.CBackInteger());
        }

        TEST_METHOD(ElementToStringExceptions) {
            const String BAD = "Hello World!"s;

            Datum datum = 1;

            Assert::ExpectException<std::invalid_argument>([&datum, &BAD](){ datum.SetElementFromString(BAD); });

            datum = 2.f;

            Assert::ExpectException<std::invalid_argument>([&datum, &BAD](){ datum.SetElementFromString(BAD); });

            Vector v{3.f, 4.f, 5.f, 6.f};
            datum = v;

            Assert::ExpectException<std::invalid_argument>([&datum, &BAD](){ datum.SetElementFromString(BAD); });

            datum = Matrix{v, v, v, v};

            Assert::ExpectException<std::invalid_argument>([&datum, &BAD](){ datum.SetElementFromString(BAD); });
        }

        TEST_METHOD(Swap) {
            const size_type ZERO = size_type(0);
            const size_type TWO = size_type(2);
            const size_type FOUR = size_type(4);

            Float external[] = {4.f, 5.f, 6.f, 7.f};

            Datum startEmpty{};
            Datum startWithTwo{2, 3};
            Datum startWithFour{};
            startWithFour.SetStorage(external, FOUR);

            size_type startEmptyCapacity = startEmpty.Capacity();
            size_type startWithTwoCapacity = startWithTwo.Capacity();
            size_type startWithFourCapacity = startWithFour.Capacity();

            Assert::AreEqual(DatumType::Unknown, startEmpty.ApparentType());
            Assert::AreEqual(DatumType::Integer, startWithTwo.ApparentType());
            Assert::AreEqual(DatumType::Float, startWithFour.ApparentType());
            Assert::AreEqual(ZERO, startEmpty.Size());
            Assert::AreEqual(TWO, startWithTwo.Size());
            Assert::AreEqual(FOUR, startWithFour.Size());
            Assert::IsTrue(startEmpty.IsDataInternal());
            Assert::IsTrue(startWithTwo.IsDataInternal());
            Assert::IsFalse(startWithFour.IsDataInternal());

            using std::swap;

            swap(startWithTwo, startWithFour);

            Assert::AreEqual(DatumType::Float, startWithTwo.ApparentType());
            Assert::AreEqual(startWithFourCapacity, startWithTwo.Capacity());
            Assert::AreEqual(FOUR, startWithTwo.Size());
            Assert::IsFalse(startWithTwo.IsDataInternal());
            Assert::AreEqual(DatumType::Integer, startWithFour.ApparentType());
            Assert::AreEqual(startWithTwoCapacity, startWithFour.Capacity());
            Assert::AreEqual(TWO, startWithFour.Size());
            Assert::IsTrue(startWithFour.IsDataInternal());

            swap(startEmpty, startWithTwo);

            Assert::AreEqual(DatumType::Float, startEmpty.ApparentType());
            Assert::AreEqual(startWithFourCapacity, startEmpty.Capacity());
            Assert::AreEqual(FOUR, startEmpty.Size());
            Assert::IsFalse(startEmpty.IsDataInternal());
            Assert::AreEqual(DatumType::Unknown, startWithTwo.ApparentType());
            Assert::AreEqual(startEmptyCapacity, startWithTwo.Capacity());
            Assert::AreEqual(ZERO, startWithTwo.Size());
            Assert::IsTrue(startWithTwo.IsDataInternal());
        }

        TEST_METHOD(ToString) {
            const Vector first{1.f, 2.f, 3.f, 4.f};
            const Vector second{5.f, 6.f, 7.f, 8.f};
            const Vector third{9.f, 10.f, 11.f, 12.f};

            Datum datum{first, second, third};

            Assert::AreEqual(
                "[ "s + datum.ElementToString(size_type(0)) + ", "s + datum.ElementToString(size_type(1)) + ", "s + datum.ElementToString(size_type(2)) + " ]"s,
                datum.ToString());

            datum = {Matrix{first, first, first, first}, Matrix{second, second, second, second}, Matrix{third, third, third, third}};

            Assert::AreEqual(
                "[ "s + datum.ElementToString(size_type(0)) + ", "s + datum.ElementToString(size_type(1)) + ", "s + datum.ElementToString(size_type(2)) + " ]"s,
                datum.ToString());
        }

        TEST_METHOD(CopySemantics) {
            Integer external[] = {1, 2};
            Datum integerDatum{};
            integerDatum.SetStorage(external, size_type(2));
            Datum floatDatum{3.f, 4.f, 5.f, 6.f};

            Datum datum{floatDatum};

            Assert::AreEqual(floatDatum, datum);
            Assert::AreNotSame(floatDatum, datum);
            Assert::IsTrue(floatDatum == datum);
            Assert::AreEqual(floatDatum.IsDataInternal(), datum.IsDataInternal());
            Assert::AreEqual(floatDatum.ToString(), datum.ToString());

            datum = integerDatum;

            Assert::AreNotEqual(floatDatum, datum);
            Assert::IsTrue(floatDatum != datum);
            Assert::AreEqual(integerDatum, datum);
            Assert::IsTrue(integerDatum == datum);
            Assert::AreEqual(integerDatum.IsDataInternal(), datum.IsDataInternal());
            Assert::AreEqual(integerDatum.ToString(), datum.ToString());

            datum = floatDatum;

            Assert::AreNotEqual(integerDatum, datum);
            Assert::IsTrue(integerDatum != datum);
            Assert::AreEqual(floatDatum, datum);
            Assert::IsTrue(floatDatum == datum);
            Assert::AreEqual(floatDatum.IsDataInternal(), datum.IsDataInternal());
            Assert::AreEqual(floatDatum.ToString(), datum.ToString());
        }

        TEST_METHOD(MoveSemantics) {
            String external[] = {"Hello"s, "World"s};
            Datum stringDatum{};
            stringDatum.SetStorage(external, size_type(2));
            Vector internal{3.f, 4.f, 5.f, 6.f};
            Datum vectorDatum{internal, internal, internal, internal};

            Datum copy{stringDatum};

            Assert::AreEqual(stringDatum, copy);
            Assert::IsTrue(stringDatum == copy);
            Assert::IsFalse(copy.IsEmpty());
            Assert::IsFalse(copy.IsDataInternal());

            Datum datum{std::move(copy)};

#pragma warning(push)
#pragma warning(disable: 26800)
            Assert::AreNotEqual(stringDatum, copy);
            Assert::IsFalse(stringDatum == copy);
            Assert::IsTrue(copy.IsEmpty());
            Assert::IsTrue(copy.IsDataInternal());
            Assert::AreEqual(stringDatum, datum);
            Assert::AreNotSame(stringDatum, datum);
            Assert::IsTrue(stringDatum == datum);
            Assert::AreEqual(stringDatum.IsDataInternal(), datum.IsDataInternal());
            Assert::AreEqual(stringDatum.ToString(), datum.ToString());

            copy = vectorDatum;

            Assert::AreEqual(vectorDatum, copy);
            Assert::IsTrue(vectorDatum == copy);
            Assert::IsFalse(copy.IsEmpty());
            Assert::IsTrue(copy.IsDataInternal());

            datum = std::move(copy);

            Assert::AreNotEqual(vectorDatum, copy);
            Assert::IsFalse(vectorDatum == copy);
            Assert::IsTrue(copy.IsEmpty());
            Assert::IsTrue(copy.IsDataInternal());
            Assert::AreNotEqual(stringDatum, datum);
            Assert::IsTrue(stringDatum != datum);
            Assert::AreEqual(vectorDatum, datum);
            Assert::IsTrue(vectorDatum == datum);
            Assert::AreEqual(vectorDatum.IsDataInternal(), datum.IsDataInternal());
            Assert::AreEqual(vectorDatum.ToString(), datum.ToString());

            copy = stringDatum;

            Assert::AreEqual(stringDatum, copy);
            Assert::IsTrue(stringDatum == copy);
            Assert::IsFalse(copy.IsEmpty());
            Assert::IsFalse(copy.IsDataInternal());

            datum = std::move(copy);

            Assert::AreNotEqual(stringDatum, copy);
            Assert::IsFalse(stringDatum == copy);
            Assert::IsTrue(copy.IsEmpty());
            Assert::IsTrue(copy.IsDataInternal());
#pragma warning(pop)
            Assert::AreNotEqual(vectorDatum, datum);
            Assert::IsTrue(vectorDatum != datum);
            Assert::AreEqual(stringDatum, datum);
            Assert::IsTrue(stringDatum == datum);
            Assert::AreEqual(stringDatum.IsDataInternal(), datum.IsDataInternal());
            Assert::AreEqual(stringDatum.ToString(), datum.ToString());
        }

        TEST_METHOD(StringDatumPointerDatumComparisons) {
            Datum first{"Hello"s, "World"s};
            Datum second{first};

            Assert::IsTrue(first == second);

            second.PushBack("!"s);

            Assert::IsTrue(first != second);

            first.PushBack(" :)");

            Assert::IsFalse(first == second);

            first.PopBack();
            first.PushBack("!"s);

            Assert::IsFalse(first != second);

            String externalStrings[] = {"Hello"s, "World"s, "!"s};
            second.SetStorage(externalStrings, size_type(3));

            Assert::IsTrue(first == second);

            Foo one{1};
            Foo two{2};
            Foo three{3};
            Foo four{4};
            Foo otherThree{3};
            Foo otherOtherOne{1};
            Foo otherOtherTwo{2};
            Foo otherOtherThree{3};

            first = {&one, &two};
            second = first;

            Assert::IsTrue(first == second);

            second.PushBack(&otherThree);

            Assert::IsTrue(first != second);

            first.PushBack(&four);

            Assert::IsFalse(first == second);

            first.PopBack();
            first.PushBack(&three);

            Assert::IsFalse(first != second);

            Pointer externalPointers[] = {&otherOtherOne, &otherOtherTwo, &otherOtherThree};
            second.SetPointerStorage(externalPointers, size_type(3));

            Assert::IsTrue(first == second);
        }

        TEST_METHOD(IllegalExternalOperations) {
            const size_type TWO = size_type(2);
            const size_type FOUR = size_type(4);

            Vector v{7.f, 8.f, 9.f, 10.f};
            Foo f{11};
            Table t{};

            Integer externalInts[] = { 1, 2 };
            Float externalFloats[] = { 3.f, 4.f };
            String externalStrings[] = { "5"s, "6"s };
            Vector externalVectors[] = { v, v };
            Matrix externalMatrices[] = { Matrix{v, v, v, v}, Matrix{v, v, v, v} };
            Pointer externalPointers[] = { &f, &f };
            Table* externalTables[] = { &t, &t };

            Datum datum{};

            datum.SetStorage(externalInts, TWO);

            Assert::ExpectException<std::logic_error>([&datum](){ datum.Clear(); });
            Assert::ExpectException<std::logic_error>([&datum](){ datum.PopBack(); });
            Assert::ExpectException<std::logic_error>([&datum](){ datum.PushBack(Integer{}); });
            Assert::ExpectException<std::logic_error>([&datum](){ datum.Resize(FOUR); });

            datum.SetStorage(externalFloats, TWO);

            Assert::ExpectException<std::logic_error>([&datum](){ datum.Clear(); });
            Assert::ExpectException<std::logic_error>([&datum](){ datum.PopBack(); });
            Assert::ExpectException<std::logic_error>([&datum](){ datum.PushBack(Float{}); });
            Assert::ExpectException<std::logic_error>([&datum](){ datum.Resize(FOUR); });

            datum.SetStorage(externalStrings, TWO);

            Assert::ExpectException<std::logic_error>([&datum](){ datum.Clear(); });
            Assert::ExpectException<std::logic_error>([&datum](){ datum.PopBack(); });
            Assert::ExpectException<std::logic_error>([&datum](){ datum.PushBack(String{}); });
            Assert::ExpectException<std::logic_error>([&datum](){ datum.Resize(FOUR); });

            datum.SetStorage(externalVectors, TWO);

            Assert::ExpectException<std::logic_error>([&datum](){ datum.Clear(); });
            Assert::ExpectException<std::logic_error>([&datum](){ datum.PopBack(); });
            Assert::ExpectException<std::logic_error>([&datum](){ datum.PushBack(Vector{}); });
            Assert::ExpectException<std::logic_error>([&datum](){ datum.Resize(FOUR); });

            datum.SetStorage(externalMatrices, TWO);

            Assert::ExpectException<std::logic_error>([&datum](){ datum.Clear(); });
            Assert::ExpectException<std::logic_error>([&datum](){ datum.PopBack(); });
            Assert::ExpectException<std::logic_error>([&datum](){ datum.PushBack(Matrix{}); });
            Assert::ExpectException<std::logic_error>([&datum](){ datum.Resize(FOUR); });

            datum.SetPointerStorage(externalPointers, TWO);

            Assert::ExpectException<std::logic_error>([&datum](){ datum.Clear(); });
            Assert::ExpectException<std::logic_error>([&datum](){ datum.PopBack(); });
            Assert::ExpectException<std::logic_error>([&datum](){ datum.PushBack(nullptr); });
            Assert::ExpectException<std::logic_error>([&datum](){ datum.Resize(FOUR); });

            datum.SetTableStorage(externalTables, TWO);

            Assert::ExpectException<std::logic_error>([&datum](){ datum.Clear(); });
            Assert::ExpectException<std::logic_error>([&datum](){ datum.PopBack(); });
            Assert::ExpectException<std::logic_error>([&datum](){ datum.PushBack(std::make_unique<Table>()); });
            Assert::ExpectException<std::logic_error>([&datum](){ datum.Resize(FOUR); });
        }

        TEST_METHOD(IllegalPushOperations) {
            Datum datum{};

            Assert::ExpectException<std::invalid_argument>([&datum](){ datum.PushBack(std::unique_ptr<Table>()); });

            datum = 0;

            Assert::ExpectException<std::invalid_argument>([&datum](){ datum.PushBack(Float{}); });
            Assert::ExpectException<std::invalid_argument>([&datum](){ datum.PushBack(String{}); });
            Assert::ExpectException<std::invalid_argument>([&datum](){ datum.PushBack(Vector{}); });
            Assert::ExpectException<std::invalid_argument>([&datum](){ datum.PushBack(Matrix{}); });
            Assert::ExpectException<std::invalid_argument>([&datum](){ datum.PushBack(nullptr); });
            Assert::ExpectException<std::invalid_argument>([&datum](){ datum.PushBack(std::make_unique<Table>()); });

            datum = 0.f;

            Assert::ExpectException<std::invalid_argument>([&datum](){ datum.PushBack(Integer{}); });
        }

        TEST_METHOD(Resize) {
            const size_type ZERO = size_type(0);
            const size_type ONE = size_type(1);
            const size_type TWO = size_type(2);
            const size_type FOUR = size_type(4);

            Integer ipp = 17;
            Float fpp = 18.f;
            String spp = "19"s;
            Vector v{7.f, 8.f, 9.f, 10.f};
            Vector vpp{12.f, 13.f, 14.f, 15.f};
            Matrix mpp{vpp, vpp, vpp, vpp};
            Foo f{11};
            Foo opp{16};

            Datum datum{ 1, 2 };

            Assert::AreEqual(TWO, datum.Size());

            datum.Resize(FOUR);

            Assert::AreEqual(FOUR, datum.Size());
            Assert::AreEqual(datum.Size(), datum.Capacity());
            Assert::AreEqual(Integer{}, datum.CBackInteger());

            datum.PopBack();
            datum.GetIntegerElement(TWO) = ipp;

            Assert::AreEqual(ipp, datum.CBackInteger());

            datum.Resize(ONE);

            Assert::AreEqual(ONE, datum.Size());
            Assert::AreEqual(datum.Size(), datum.Capacity());
            Assert::AreSame(datum.FrontInteger(), datum.BackInteger());

            datum.Resize(ZERO);

            Assert::IsTrue(datum.IsEmpty());
            Assert::AreEqual(ZERO, datum.Capacity());

            datum = { 3.f, 4.f };

            Assert::AreEqual(TWO, datum.Size());

            datum.Resize(FOUR);

            Assert::AreEqual(FOUR, datum.Size());
            Assert::AreEqual(datum.Size(), datum.Capacity());
            Assert::AreEqual(Float{}, datum.CBackFloat());

            datum.PopBack();
            datum.GetFloatElement(TWO) = fpp;

            Assert::AreEqual(fpp, datum.CBackFloat());

            datum.Resize(ONE);

            Assert::AreEqual(ONE, datum.Size());
            Assert::AreEqual(datum.Size(), datum.Capacity());
            Assert::AreSame(datum.FrontFloat(), datum.BackFloat());

            datum.Resize(ZERO);

            Assert::IsTrue(datum.IsEmpty());
            Assert::AreEqual(ZERO, datum.Capacity());

            datum = { "5"s, "6"s };

            Assert::AreEqual(TWO, datum.Size());

            datum.Resize(FOUR);

            Assert::AreEqual(FOUR, datum.Size());
            Assert::AreEqual(datum.Size(), datum.Capacity());
            Assert::AreEqual(String{}, datum.CBackString());

            datum.PopBack();
            datum.GetStringElement(TWO) = spp;

            Assert::AreEqual(spp, datum.CBackString());

            datum.Resize(ONE);

            Assert::AreEqual(ONE, datum.Size());
            Assert::AreEqual(datum.Size(), datum.Capacity());
            Assert::AreSame(datum.FrontString(), datum.BackString());

            datum.Resize(ZERO);

            Assert::IsTrue(datum.IsEmpty());
            Assert::AreEqual(ZERO, datum.Capacity());

            datum = { v, v };

            Assert::AreEqual(TWO, datum.Size());

            datum.Resize(FOUR);

            Assert::AreEqual(FOUR, datum.Size());
            Assert::AreEqual(datum.Size(), datum.Capacity());
            Assert::AreEqual(Vector{}, datum.CBackVector());

            datum.PopBack();
            datum.GetVectorElement(TWO) = vpp;

            Assert::AreEqual(vpp, datum.CBackVector());

            datum.Resize(ONE);

            Assert::AreEqual(ONE, datum.Size());
            Assert::AreEqual(datum.Size(), datum.Capacity());
            Assert::AreSame(datum.FrontVector(), datum.BackVector());

            datum.Resize(ZERO);

            Assert::IsTrue(datum.IsEmpty());
            Assert::AreEqual(ZERO, datum.Capacity());

            datum = { Matrix{v, v, v, v}, Matrix{v, v, v, v} };

            Assert::AreEqual(TWO, datum.Size());

            datum.Resize(FOUR);

            Assert::AreEqual(FOUR, datum.Size());
            Assert::AreEqual(datum.Size(), datum.Capacity());
            Assert::AreEqual(Matrix{}, datum.CBackMatrix());

            datum.PopBack();
            datum.GetMatrixElement(TWO) = mpp;

            Assert::AreEqual(mpp, datum.CBackMatrix());

            datum.Resize(ONE);

            Assert::AreEqual(ONE, datum.Size());
            Assert::AreEqual(datum.Size(), datum.Capacity());
            Assert::AreSame(datum.FrontMatrix(), datum.BackMatrix());

            datum.Resize(ZERO);

            Assert::IsTrue(datum.IsEmpty());
            Assert::AreEqual(ZERO, datum.Capacity());

            datum = { &f, &f };

            Assert::AreEqual(TWO, datum.Size());

            datum.Resize(FOUR);

            Assert::AreEqual(FOUR, datum.Size());
            Assert::AreEqual(datum.Size(), datum.Capacity());
            Assert::IsNull(datum.CBackPointer());

            datum.PopBack();
            datum.GetPointerElement(TWO) = &opp;

            Assert::IsTrue(&opp == datum.CBackPointer());

            datum.Resize(ONE);

            Assert::AreEqual(ONE, datum.Size());
            Assert::AreEqual(datum.Size(), datum.Capacity());
            Assert::AreSame(datum.FrontPointer(), datum.BackPointer());

            datum.Resize(ZERO);

            Assert::IsTrue(datum.IsEmpty());
            Assert::AreEqual(ZERO, datum.Capacity());
        }

        TEST_METHOD(GrowCapacityFunctor) {
            Datum datum{DatumType::Integer, size_type(1), [](size_type size, size_type){ return size_type(3) * size; }};

            size_type prepush = datum.Capacity();
            datum.PushBack(0);
            size_type postpush = datum.Capacity();

            Assert::AreEqual(prepush, postpush);

            prepush = datum.Capacity();
            datum.PushBack(0);
            postpush = datum.Capacity();

            Assert::AreNotEqual(prepush, postpush);
            Assert::AreEqual(datum.GetGrowCapacityFunctor()(prepush, prepush), postpush);

            prepush = datum.Capacity();
            datum.PushBack(0);
            postpush = datum.Capacity();

            Assert::AreEqual(prepush, postpush);

            datum.SetGrowCapacityFunctor([](size_type, size_type){ return size_type(0); });

            prepush = datum.Capacity();
            datum.PushBack(0);
            postpush = datum.Capacity();

            Assert::AreNotEqual(prepush, postpush);
            Assert::AreNotEqual(datum.GetGrowCapacityFunctor()(prepush, prepush), postpush);
            Assert::AreEqual(prepush + size_type(1), postpush);
        }

        TEST_METHOD(ConstExternalStorage) {
            Integer externalInts[] = { 1, 2 };

            Datum datum{};
            datum.SetStorage(externalInts, size_type(2), true);

            Assert::ExpectException<std::logic_error>([&datum](){ int& front = datum.FrontInteger(); UNREFERENCED_LOCAL(front); });
            Assert::ExpectException<std::logic_error>([&datum](){ datum.SetElement(3); });
        }
    };
}
