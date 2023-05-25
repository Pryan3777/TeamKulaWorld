#include "pch.h"
#include "Foo.h"

using namespace std;

namespace LibraryDesktopTests
{
	RTTI_DEFINITIONS(Foo);

	Foo::Foo(int32_t data) :
		_data(new int32_t(data))
	{

	}

	Foo::Foo(const Foo& rhs) :
		RTTI(rhs), _data(new int32_t(*rhs._data))
	{

	}

	Foo::Foo(Foo&& rhs) noexcept :
		RTTI(std::move(rhs)), _data(rhs._data)
	{
		rhs._data = nullptr;
	}

	Foo& Foo::operator=(const Foo& rhs)
	{
		if (this != &rhs)
		{
			RTTI::operator=(rhs);
			*_data = *rhs._data;
		}
		return *this;
	}

	Foo& Foo::operator=(Foo&& rhs) noexcept
	{
		if (this != &rhs)
		{
			RTTI::operator=(std::move(rhs));
			delete _data;
			_data = rhs._data;
			rhs._data = nullptr;
		}
		return *this;
	}
	
	Foo::~Foo()
	{
		delete _data;
	}

	bool Foo::operator==(const Foo& rhs) const
	{
		return *_data == *rhs._data;
	}

	bool Foo::operator!=(const Foo& rhs) const
	{
		return !operator == (rhs);
	}

	int32_t Foo::Data() const
	{
		return *_data;
	}

	void Foo::SetData(int32_t data)
	{
		*_data = data;
	}

    bool Foo::Equals(const RTTI* rhs) const {
        return rhs->Is(TypeIdInstance()) && (*this == *rhs->As<Foo>());
    }
}