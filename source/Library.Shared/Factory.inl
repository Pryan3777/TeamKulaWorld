#pragma once
#include "Factory.h"

namespace FieaGameEngine {
    template <typename T> inline Factory<T>* Factory<T>::Find(const std::string& createdClassName) {
        auto found = _factories.Find(createdClassName);
        return (found == _factories.end()) ? nullptr : found->second.get();
    }

    template <typename T> inline bool Factory<T>::TryFind(const std::string& createdClassName, Factory*& found) {
        found = Find(createdClassName);
        return found != nullptr;
    }

    template <typename T> inline std::unique_ptr<T> Factory<T>::StaticCreate(const std::string& nameOfClassToCreate) {
        Factory* factory = Find(nameOfClassToCreate);
        return (factory == nullptr) ? std::unique_ptr<T>{} : factory->Create();
    }

    template <typename T> inline void Factory<T>::Add(std::unique_ptr<Factory> factory) {
        if (factory) {
            std::string createdClassName = factory->CreatedClassName();
            _factories.Insert(std::make_pair(createdClassName, std::move(factory)));
        }
    }

    template <typename T> inline void Factory<T>::Remove(const std::string& factoryCreatedClassName) {
        _factories.Remove(factoryCreatedClassName);
    }
}
