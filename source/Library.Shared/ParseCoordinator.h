#pragma once
#include <memory>
#include "RTTI.h"

namespace FieaGameEngine {
    class ParseCoordinator {

    public:
        class Wrapper : public RTTI {
            RTTI_DECLARATIONS(Wrapper, RTTI);

        public:
            using depth_type = std::size_t;
            using WrapperSharedPointer = std::shared_ptr<Wrapper>;

            Wrapper();
            virtual ~Wrapper() = default;

            virtual WrapperSharedPointer Create() const = 0;

            bool IncrementDepth();
            virtual bool IncrementDepth(const std::string& subobjectName, bool isArray = false, std::size_t arrayIndex = std::size_t(0));
            virtual bool DecrementDepth();
            [[nodiscard]] depth_type Depth() const;
            [[nodiscard]] ParseCoordinator* Coordinator() const;

        protected:
            depth_type _depth;

            Wrapper(ParseCoordinator*);

            virtual void HandleSubobjectNameAndIndex(const std::string& subobjectName, bool isArray, std::size_t arrayIndex);

        private:
            ParseCoordinator* _coordinator;

            friend ParseCoordinator;

        };

        using WrapperSharedPointer = Wrapper::WrapperSharedPointer;

        explicit ParseCoordinator(WrapperSharedPointer);
        ParseCoordinator(const ParseCoordinator&) = delete;
        ParseCoordinator(ParseCoordinator&&) noexcept;
        ParseCoordinator& operator=(const ParseCoordinator&) = delete;
        ParseCoordinator& operator=(ParseCoordinator&&) noexcept;
        virtual ~ParseCoordinator() = default;

        [[nodiscard]] virtual std::shared_ptr<ParseCoordinator> Clone() const = 0;

        void DeserializeIntoWrapperFromString(const std::string& payload);
        void DeserializeIntoWrapperFromFile(const std::string& filename);
        virtual void DeserializeIntoWrapper(std::istream* payload) = 0;

        [[nodiscard]] WrapperSharedPointer GetWrapper();
        [[nodiscard]] const WrapperSharedPointer& GetWrapper() const;
        void SetWrapper(WrapperSharedPointer);

    protected:
        std::shared_ptr<Wrapper> _wrapper;

    };
}

#include "ParseCoordinator.inl"
