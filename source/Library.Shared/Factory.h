#pragma once
#include <memory>
#include <string>
#include "HashMap.h"

namespace FieaGameEngine {
    /// <summary>
    /// Templated base class for factories. To define a factory, it's recommended to use the FACTORY macro rather than to inherit from this class directly.
    /// </summary>
    template <typename T>
    class Factory {

    public:
        virtual ~Factory() = default;

        /// <returns>The name of the class this factory creates instances of.</returns>
        [[nodiscard]] virtual std::string CreatedClassName() const = 0;

        /// <returns>A heap-allocated instance of T.</returns>
        [[nodiscard]] virtual std::unique_ptr<T> Create() const = 0;

        /// <summary>
        /// Finds a factory associated with this base class which creates instances of the class that has the given name.
        /// </summary>
        /// <returns>nullptr if no such factory exists, otherwise a pointer to the appropriate factory.</returns>
        [[nodiscard]] static Factory* Find(const std::string& createdClassName);

        /// <summary>
        /// Finds a factory associated with this base class which creates instances of the class that has the given name.
        /// </summary>
        /// <param name="found"> - Output parameter. nullptr if no such factory exists, otherwise a pointer to the appropriate factory.</param>
        /// <returns>True if the factory was found, false otherwise.</returns>
        [[nodiscard]] static bool TryFind(const std::string& createdClassName, Factory*& found);

        /// <summary>
        /// Finds the appropriate factory, then creates a heap-allocated instance of the class.
        /// </summary>
        /// <returns>An empty pointer if no such factory is associated with this base class, otherwise a heap-allocated instance of T.</returns>
        [[nodiscard]] static std::unique_ptr<T> StaticCreate(const std::string& nameOfClassToCreate);

    protected:
        /// <summary>
        /// Associates the given factory to this base class. After adding, the factory can be retrieved publicly using `Find` and `TryFind`, or used indirectly through `StaticCreate`.
        /// </summary>
        static void Add(std::unique_ptr<Factory>);

        /// <summary>
        /// Removes the association from this factory.
        /// </summary>
        static void Remove(const std::string& factoryCreatedClassName);

    private:
        inline static HashMap<std::string, std::unique_ptr<Factory>> _factories{};

    };
}

#define FACTORY(ConcreteCreatedClass, AbstractCreatedClass, ...)                                                                                    \
    class ConcreteCreatedClass ## Factory final : public FieaGameEngine::Factory<AbstractCreatedClass> {                                            \
    public:                                                                                                                                         \
        inline static void Register() { FieaGameEngine::Factory<AbstractCreatedClass>::Add(std::make_unique<ConcreteCreatedClass ## Factory>()); }  \
        inline static void Unregister() { FieaGameEngine::Factory<AbstractCreatedClass>::Remove(#ConcreteCreatedClass); }                           \
        [[nodiscard]] inline std::string CreatedClassName() const override { return #ConcreteCreatedClass; }                                        \
        [[nodiscard]] inline std::unique_ptr<AbstractCreatedClass> Create() const override {                                                        \
            return Create##ConcreteCreatedClass(__VA_ARGS__);                                                                                       \
        }                                                                                                                                           \
        template <typename... Args> [[nodiscard]] inline std::unique_ptr<ConcreteCreatedClass> Create##ConcreteCreatedClass(Args&&... args) const { \
            return std::make_unique<ConcreteCreatedClass>(std::forward<Args>(args)...);                                                             \
        }                                                                                                                                           \
    }

#include "Factory.inl"
