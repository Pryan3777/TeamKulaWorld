#pragma once
#include <cstddef>
#include <string>

namespace FieaGameEngine
{
	class RTTI
	{
	public:
		using IdType = std::size_t;

		RTTI() = default;
		RTTI(const RTTI&) = default;
		RTTI& operator=(const RTTI&) = default;
		RTTI(RTTI&&) noexcept = default;
		RTTI& operator=(RTTI&&) noexcept = default;
		virtual ~RTTI() = default;

		virtual IdType TypeIdInstance() const = 0;
		virtual std::string TypeNameInstance() const = 0;
		virtual bool Is(IdType) const;

		template <typename T>
		T* As();

		template <typename T>
		const T* As() const;

		virtual std::string ToString() const;
		virtual bool Equals(const RTTI* rhs) const;
	};
}

#define RTTI_DECLARATIONS(Type, ParentType)																					\
	public:																													\
		static FieaGameEngine::RTTI::IdType TypeIdClass() { return _typeId; }												\
		static std::string TypeNameClass() { return #Type; }																\
		FieaGameEngine::RTTI::IdType TypeIdInstance() const override { return TypeIdClass(); }								\
		std::string TypeNameInstance() const override { return TypeNameClass(); }											\
		bool Is(FieaGameEngine::RTTI::IdType id) const override { return (id == _typeId ? true : ParentType::Is(id)); }		\
	private:																												\
		static const FieaGameEngine::RTTI::IdType _typeId;

#define RTTI_DEFINITIONS(Type)																								\
	const FieaGameEngine::RTTI::IdType Type::_typeId = reinterpret_cast<FieaGameEngine::RTTI::IdType>(&Type::_typeId);

#define RTTI_TEMPLATED_DEFINITIONS(Type, T)																					\
	template <typename T> const FieaGameEngine::RTTI::IdType Type<T>::_typeId = reinterpret_cast<FieaGameEngine::RTTI::IdType>(&Type<T>::_typeId);


#include "RTTI.inl"