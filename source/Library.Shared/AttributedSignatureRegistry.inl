#pragma once
#include "AttributedSignatureRegistry.h"
#include <cassert>

namespace FieaGameEngine {
    template <typename TRegisterThis> inline bool AttributedSignatureRegistry::RegisterSignatures() {
        assert(_instance); return _instance->_RegisterSignatures(TRegisterThis::TypeIdClass(), TRegisterThis::Signatures(), 0);
    }

    template <typename TRegisterThis, typename TParent> inline bool AttributedSignatureRegistry::RegisterSignatures() {
        assert(_instance); return _instance->_RegisterSignatures(TRegisterThis::TypeIdClass(), TRegisterThis::Signatures(), TParent::TypeIdClass());
    }

    inline bool AttributedSignatureRegistry::RegisterSignatures(
        const key_type& key,
        SignatureVector&& signatures,
        const key_type& parent
    ) {
        assert(_instance); return _instance->_RegisterSignatures(key, std::forward<SignatureVector>(signatures), parent);
    }

    template <typename TUnregisterThis> inline bool AttributedSignatureRegistry::UnregisterSignatures() {
        assert(_instance); return _instance->_UnregisterSignatures(TUnregisterThis::TypeIdClass());
    }

    inline bool AttributedSignatureRegistry::UnregisterSignatures(const key_type& key) {
        assert(_instance); return _instance->_UnregisterSignatures(key);
    }

    inline typename AttributedSignatureRegistry::const_iterator AttributedSignatureRegistry::FindSignatures(const key_type& key) {
        assert(_instance); return _instance->_FindSignatures(key);
    }

    inline bool AttributedSignatureRegistry::IsFound(const const_iterator& iterator) {
        assert(_instance); return _instance->_IsFound(iterator);
    }

    inline typename AttributedSignatureRegistry::const_iterator AttributedSignatureRegistry::_FindSignatures(
        const key_type& key
    ) const { return _registered.CFind(key); }

    inline bool AttributedSignatureRegistry::_IsFound(const const_iterator& iterator) const {
        return iterator != _registered.cend();
    }
}
