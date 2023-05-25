#pragma once
#include "Attributed.h"
#include "AttributedSignatureRegistry.h"
#include "IEventArgs.h"

namespace FieaGameEngine {
    namespace OverrideStructs {
        struct __AttributedEventArgs_Attributed : public Attributed {
            inline __AttributedEventArgs_Attributed(IdType idOfSignaturesToAppend) : Attributed{idOfSignaturesToAppend} {}
            virtual ~__AttributedEventArgs_Attributed() = default;
            virtual ScopeUniquePointer CloneAsScope() const = 0;
        private:
            virtual inline ScopeUniquePointer Clone() const override { return CloneAsScope(); }
        };
        struct __AttributedEventArgs_IEventArgs : public IEventArgs {
            virtual ~__AttributedEventArgs_IEventArgs() = default;
            virtual std::unique_ptr<IEventArgs> CloneAsEventArgs() const = 0;
        private:
            virtual inline std::unique_ptr<IEventArgs> Clone() const override { return CloneAsEventArgs(); }
        };
    }

    class AttributedEventArgs final : public OverrideStructs::__AttributedEventArgs_Attributed, public OverrideStructs::__AttributedEventArgs_IEventArgs {
#pragma region RTTI_DECLARATIONS
    public:
        inline static IdType TypeIdClass() { return _typeId; }
        inline static std::string TypeNameClass() { using namespace std::literals::string_literals; return "AttributedEventArgs"s; }
        inline IdType TypeIdInstance() const override { return TypeIdClass(); }
        inline std::string TypeNameInstance() const override { return TypeNameClass(); }
        inline bool Is(IdType id) const override { return (id == _typeId) ? true : (Attributed::Is(id) || IEventArgs::Is(id)); }
    private:
        static const IdType _typeId;
#pragma endregion RTTI_DECLARATIONS

    public:
        static SignatureVector Signatures();

        AttributedEventArgs(String subtype);

        String& Subtype();
        const String& GetSubtype() const;
        void SetSubtype(String) noexcept;

        ScopeUniquePointer CloneAsScope() const override;
        std::unique_ptr<IEventArgs> CloneAsEventArgs() const override;

    private:
        using BaseConstructor = __AttributedEventArgs_Attributed;

        static const key_type SUBTYPE_KEY;

        String _subtype;

    };

    FACTORY(AttributedEventArgs, Scope, typename AttributedEventArgs::String{});
}

#include "AttributedEventArgs.inl"
