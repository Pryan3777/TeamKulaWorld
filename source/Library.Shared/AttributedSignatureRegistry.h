#pragma once
#include "Attributed.h"

namespace FieaGameEngine {
    using SignatureVector = Vector<Attributed::Signature>;

    class AttributedSignatureRegistry final {

        friend std::unique_ptr<AttributedSignatureRegistry> std::make_unique();

    public:
        using key_type = Attributed::IdType;
        using Map = HashMap<key_type, SignatureVector>;
        using const_iterator = Map::const_iterator;

    private:
        static std::unique_ptr<AttributedSignatureRegistry> _instance;

        HashMap<key_type, SignatureVector> _registered;

        AttributedSignatureRegistry() = default;

        bool _RegisterSignatures(const key_type& key, SignatureVector&& signatures, const key_type& parent);
        bool _UnregisterSignatures(const key_type& key);
        const_iterator _FindSignatures(const key_type& key) const;
        bool _IsFound(const const_iterator& itr) const;

    public:
        static bool CreateSingleton();
        static bool DestroySingleton();

        template <typename TRegisterThis> static bool RegisterSignatures();
        template <typename TRegisterThis, typename TParent> static bool RegisterSignatures();
        static bool RegisterSignatures(const key_type& key, SignatureVector&& signatures, const key_type& parent = key_type{0});

        template <typename TUnregisterThis> static bool UnregisterSignatures();
        static bool UnregisterSignatures(const key_type& key);

        static const_iterator FindSignatures(const key_type& key);
        static bool IsFound(const const_iterator& iterator);

        AttributedSignatureRegistry(const AttributedSignatureRegistry&) = delete;
        AttributedSignatureRegistry(AttributedSignatureRegistry&&) noexcept = delete; // discuss with Paul
        AttributedSignatureRegistry& operator=(const AttributedSignatureRegistry&) = delete;
        AttributedSignatureRegistry& operator=(AttributedSignatureRegistry&&) noexcept = delete; // discuss with Paul
        ~AttributedSignatureRegistry() = default;

    };
}

#include "AttributedSignatureRegistry.inl"
