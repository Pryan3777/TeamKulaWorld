#include "pch.h"
#include "AttributedSignatureRegistry.h"

namespace FieaGameEngine {
    std::unique_ptr<AttributedSignatureRegistry> AttributedSignatureRegistry::_instance{};

    bool AttributedSignatureRegistry::CreateSingleton() {
        if (_instance.operator bool()) {
            return false;
        }

        _instance = std::make_unique<AttributedSignatureRegistry>();
        return true;
    }

    /*
    Discuss this with Paul. IF the move semantics were to be implemented, their definitions would be as follows...

    AttributedSignatureRegistry::AttributedSignatureRegistry(AttributedSignatureRegistry&& other) noexcept
        : _registered{std::move(other._registered)} {}

    AttributedSignatureRegistry& AttributedSignatureRegistry::operator=(AttributedSignatureRegistry&& other) noexcept {
        _registered = std::move(other._registered);
        return *this;
    }
    */

    bool AttributedSignatureRegistry::DestroySingleton() {
        if (_instance.operator bool()) {
            _instance.reset();
            return true;
        }

        return false;
    }

    bool AttributedSignatureRegistry::_RegisterSignatures(
        const key_type& key,
        SignatureVector&& signatures,
        const key_type& parent
    ) {
        if (_registered.IsContainingKey(key)) {
            return false;
        }

        // TODO - Disallow duplicate signatures

        if ((parent != key_type{0})
            && (parent != Attributed::TypeIdClass())
            && (parent != Scope::TypeIdClass())
        ) {
            auto parentSignatures = _FindSignatures(parent);

            if (parentSignatures == _registered.cend()) {
                using namespace std::literals::string_literals;

                throw std::invalid_argument("Signatures for parent ID "s + std::to_string(parent) + " not found!");
            }

            SignatureVector copy = parentSignatures->second;
            signatures.swap(copy);

            for (const auto& sig : copy) {
                signatures.EmplaceBack(sig);
            }
        }

        _registered.Insert(std::make_pair(key, std::move(signatures)));
        return true;
    }

    bool AttributedSignatureRegistry::_UnregisterSignatures(const key_type& key) {
        auto found = _registered.Find(key);

        if (found == _registered.end()) {
            return false;
        }

        _registered.Remove(found);
        return true;
    }
}
