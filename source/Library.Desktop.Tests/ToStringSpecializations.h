#pragma once

#include "CppUnitTest.h"
#include "Datum.h"
#include "Foo.h"
#include "SList.h"
#include "Vector.h"
#include "HashMap.h"
#include "Scope.h"
#include "AttributedTestMonster.h"
#include "Direction3D.h"

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<LibraryDesktopTests::Foo>(const LibraryDesktopTests::Foo& t)
	{
		return ToString(t.Data());
	}

	template<>
	inline std::wstring ToString<LibraryDesktopTests::Foo>(const LibraryDesktopTests::Foo* t)
	{
		return ToString(*t);
	}

	template<>
	inline std::wstring ToString<LibraryDesktopTests::Foo>(LibraryDesktopTests::Foo* t)
	{
		return ToString(*t);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::SList<LibraryDesktopTests::Foo>::iterator>(const FieaGameEngine::SList<LibraryDesktopTests::Foo>::iterator& t)
	{
		using namespace std::literals::string_literals;
		return t.IsDereferenceable() ? ToString(*t) : L"end()"s;
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::SList<LibraryDesktopTests::Foo>::const_iterator>(const FieaGameEngine::SList<LibraryDesktopTests::Foo>::const_iterator& t)
	{
		using namespace std::literals::string_literals;
		return t.IsDereferenceable() ? ToString(*t) : L"end()"s;
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Vector<LibraryDesktopTests::Foo>::iterator>(const FieaGameEngine::Vector<LibraryDesktopTests::Foo>::iterator& t)
	{
		using namespace std::literals::string_literals;
		return t.IsDereferenceable() ? ToString(*t) : L"end()"s;
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Vector<LibraryDesktopTests::Foo>::const_iterator>(const FieaGameEngine::Vector<LibraryDesktopTests::Foo>::const_iterator& t)
	{
		using namespace std::literals::string_literals;
		return t.IsDereferenceable() ? ToString(*t) : L"end()"s;
	}

	template<>
	inline std::wstring ToString<std::pair<const std::string, LibraryDesktopTests::Foo>>(const std::pair<const std::string, LibraryDesktopTests::Foo>& t)
	{
		using namespace std::literals::string_literals;
		RETURN_WIDE_STRING(L"("s << ToString(t.first) << L", "s << ToString(t.second) << L")"s);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::HashMap<std::string, LibraryDesktopTests::Foo>::iterator>(const FieaGameEngine::HashMap<std::string, LibraryDesktopTests::Foo>::iterator& t)
	{
		using namespace std::literals::string_literals;
		return t.IsDereferenceable() ? ToString(*t) : L"end()"s;
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::HashMap<std::string, LibraryDesktopTests::Foo>::const_iterator>(const FieaGameEngine::HashMap<std::string, LibraryDesktopTests::Foo>::const_iterator& t)
	{
		using namespace std::literals::string_literals;
		return t.IsDereferenceable() ? ToString(*t) : L"end()"s;
	}

	template<>
	inline std::wstring ToString<std::pair<const LibraryDesktopTests::Foo, std::string>>(const std::pair<const LibraryDesktopTests::Foo, std::string>& t)
	{
		using namespace std::literals::string_literals;
		RETURN_WIDE_STRING(L"("s << ToString(t.first) << L", "s << ToString(t.second) << L")"s);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::HashMap<LibraryDesktopTests::Foo, std::string>::iterator>(const FieaGameEngine::HashMap<LibraryDesktopTests::Foo, std::string>::iterator& t)
	{
		using namespace std::literals::string_literals;
		return t.IsDereferenceable() ? ToString(*t) : L"end()"s;
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::HashMap<LibraryDesktopTests::Foo, std::string>::const_iterator>(const FieaGameEngine::HashMap<LibraryDesktopTests::Foo, std::string>::const_iterator& t)
	{
		using namespace std::literals::string_literals;
		return t.IsDereferenceable() ? ToString(*t) : L"end()"s;
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Datum::DatumType>(const FieaGameEngine::Datum::DatumType& t)
	{
		return ToString(ToStringDatumType(t));
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Datum::DatumType>(const FieaGameEngine::Datum::DatumType* t)
	{
		return ToString(*t);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Datum::DatumType>(FieaGameEngine::Datum::DatumType* t)
	{
		return ToString(*t);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Datum::Vector>(const FieaGameEngine::Datum::Vector& t)
	{
		RETURN_WIDE_STRING(ToString(t.x) << ToString(t.y) << ToString(t.z) << ToString(t.w));
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Datum::Vector>(const FieaGameEngine::Datum::Vector* t)
	{
		return ToString(*t);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Datum::Vector>(FieaGameEngine::Datum::Vector* t)
	{
		return ToString(*t);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Datum::Matrix>(const FieaGameEngine::Datum::Matrix& t)
	{
		RETURN_WIDE_STRING(ToString(t[0]) << ToString(t[1]) << ToString(t[2]) << ToString(t[3]));
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Datum::Matrix>(const FieaGameEngine::Datum::Matrix* t)
	{
		return ToString(*t);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Datum::Matrix>(FieaGameEngine::Datum::Matrix* t)
	{
		return ToString(*t);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::RTTI>(const FieaGameEngine::RTTI& t)
	{
		return ToString(t.ToString());
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::RTTI>(const FieaGameEngine::RTTI* t)
	{
		return ToString(t->ToString());
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::RTTI>(FieaGameEngine::RTTI* t)
	{
		return ToString(t->ToString());
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Datum>(const FieaGameEngine::Datum& t)
	{
		return ToString(t.ToString());
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Datum>(const FieaGameEngine::Datum* t)
	{
		return ToString(t->ToString());
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Datum>(FieaGameEngine::Datum* t)
	{
		return ToString(t->ToString());
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Scope>(const FieaGameEngine::Scope& t)
	{
		return ToString(t.ToString());
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Scope>(const FieaGameEngine::Scope* t)
	{
		return ToString(t->ToString());
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Scope>(FieaGameEngine::Scope* t)
	{
		return ToString(t->ToString());
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Scope::value_type>(const FieaGameEngine::Scope::value_type& t)
	{
		using namespace std::literals::string_literals;
		RETURN_WIDE_STRING(L"{ "s << ToString(t.first) << L", "s << ToString(t.second) << L" }"s);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Scope::value_type>(const FieaGameEngine::Scope::value_type* t)
	{
		using namespace std::literals::string_literals;
		RETURN_WIDE_STRING(L"{ "s << ToString(t->first) << L", "s << ToString(t->second) << L" }"s);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Scope::value_type>(FieaGameEngine::Scope::value_type* t)
	{
		using namespace std::literals::string_literals;

		RETURN_WIDE_STRING(L"{ "s << ToString(t->first) << L", "s << ToString(t->second) << L" }"s);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Scope::iterator>(const FieaGameEngine::Scope::iterator& t)
	{
		using namespace std::literals::string_literals;
		return t.IsDereferenceable() ? ToString(*t) : L"end()"s;
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Scope::const_iterator>(const FieaGameEngine::Scope::const_iterator& t)
	{
		using namespace std::literals::string_literals;
		return t.IsDereferenceable() ? ToString(*t) : L"end()"s;
	}

	template<>
	inline std::wstring ToString<LibraryDesktopTests::AttributedThing>(const LibraryDesktopTests::AttributedThing* t)
	{
		return ToString(t->ToString());
	}

	template<>
	inline std::wstring ToString<LibraryDesktopTests::AttributedThing>(LibraryDesktopTests::AttributedThing* t)
	{
		return ToString(t->ToString());
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Direction3D>(const FieaGameEngine::Direction3D& t)
	{
		return ToString(static_cast<FieaGameEngine::Direction3D_int_type>(t));
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Direction3D>(const FieaGameEngine::Direction3D* t)
	{
		return ToString(*t);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Direction3D>(FieaGameEngine::Direction3D* t)
	{
		return ToString(*t);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Direction3DInfo>(const FieaGameEngine::Direction3DInfo& t)
	{
		return ToString(t.GetDirection());
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Direction3DInfo>(const FieaGameEngine::Direction3DInfo* t)
	{
		return ToString(*t);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Direction3DInfo>(FieaGameEngine::Direction3DInfo* t)
	{
		return ToString(*t);
	}
}
