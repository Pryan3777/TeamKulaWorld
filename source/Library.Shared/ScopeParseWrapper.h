#pragma once
#include <memory>
#include "ParseCoordinator.h"
#include "Scope.h"
#include "ShuntingYardParser.h"
#include "Stack.h"

namespace FieaGameEngine {
    /*
     * Stack frames are either "keyworded" or "not keyworded".
     * If a stack frame is keyworded, then all of the keys in the subobject MUST be keywords. If not, the JSON cannot be parsed.
     * If a stack frame is not keyworded, then NONE of the keys in the subobject can be keywords.
     *     Also, the moment a subobject is determined to not be keyworded, it is therefore a scope and the stack frame is updated as such.
     * Keyworded stack frames can also result in the creation of a scope if the subobject's type is "object".
     * Every keyworded stack frame MUST be followed by a non-keyworded frame.
     *     If a frame following a keyworded frame attempts to also be keyworded, the JSON cannot be parsed.
     */

    class ScopeParseWrapper : public ParseCoordinator::Wrapper {
        RTTI_DECLARATIONS(ScopeParseWrapper, ParseCoordinator::Wrapper);

    public:
        class StackFrame final {

        public:
            enum class KeywordState : std::uint8_t {
                Undecided = 0U,
                Keyworded,
                NotKeyworded
            };

            KeywordState _keywordState{KeywordState::Undecided};
            Scope* _scope{nullptr};
            std::string _name{};
            std::string _class{};
            std::size_t _index{std::size_t(0)};

        private:
            StackFrame(Scope* scope, const std::string& name, std::size_t index = std::size_t(0));

            friend ScopeParseWrapper;

        };

        [[nodiscard]] static bool IsKeyKeyword(const std::string& key);

        static const std::string KEYWORD_TYPE;
        static const std::string KEYWORD_CLASS;
        static const std::string KEYWORD_VALUE;
        static const std::string TYPE_OBJECT;

        explicit ScopeParseWrapper(std::shared_ptr<Scope>);
        ScopeParseWrapper(std::shared_ptr<Scope>, ParseCoordinator*); // TODO - make protected
        ScopeParseWrapper(const ScopeParseWrapper&) = delete;
        ScopeParseWrapper(ScopeParseWrapper&&) noexcept;
        ScopeParseWrapper& operator=(const ScopeParseWrapper&) = delete;
        ScopeParseWrapper& operator=(ScopeParseWrapper&&) noexcept;
        virtual ~ScopeParseWrapper() = default;

        [[nodiscard]] virtual WrapperSharedPointer Create() const override;

        virtual bool DecrementDepth() override;

        [[nodiscard]] bool IsEmpty() const;

        [[nodiscard]] const Stack<StackFrame>::iterator TopStackFrame();
        [[nodiscard]] const Stack<StackFrame>::const_iterator TopStackFrame() const;
        [[nodiscard]] const Stack<StackFrame>::const_iterator CTopStackFrame() const;

        bool CreateShuntingYardParser();
        bool CreateShuntingYardParser(std::initializer_list<std::pair<std::string, ShuntingYardParser::OperatorDetails>> operatorOverrides);
        bool CreateShuntingYardParser(std::string leftParenthesis, std::string rightParenthesis, std::string comma);
        bool CreateShuntingYardParser(
            std::string leftParenthesis,
            std::string rightParenthesis,
            std::string comma,
            std::initializer_list<std::pair<std::string, ShuntingYardParser::OperatorDetails>> operatorOverrides
        );
        void DestroyShuntingYardParser();
        [[nodiscard]] const ShuntingYardParser& GetShuntingYardParser() const;

    protected:
        virtual void HandleSubobjectNameAndIndex(const std::string& subobjectName, bool isArray, std::size_t arrayIndex) override;

    private:
        std::shared_ptr<Scope> _scope;
        Stack<StackFrame> _stack{};
        std::unique_ptr<ShuntingYardParser> _shuntingYardParser{};

        bool CreateShuntingYardParser(
            bool useDefaultConfiguration,
            std::string leftParenthesis,
            std::string rightParenthesis,
            std::string comma,
            std::initializer_list<std::pair<std::string, ShuntingYardParser::OperatorDetails>> operatorOverrides
        );

    };
}

#include "ScopeParseWrapper.inl"
