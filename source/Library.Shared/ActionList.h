#pragma once
#include "Action.h"

namespace FieaGameEngine::Actions {
    class ActionList : public Action {
        RTTI_DECLARATIONS(ActionList, Action);

    public:
        static SignatureVector Signatures();

        ActionList();
        explicit ActionList(const String& name);
        ActionList(const ActionList&);
        ActionList(ActionList&&) noexcept;
        ActionList& operator=(const ActionList&);
        ActionList& operator=(ActionList&&) noexcept;
        virtual ~ActionList();

        virtual void UpdateSelf(const GameTime& gameTime);
        void UpdateActions(const GameTime& gameTime, Datum& actions);
        virtual void Update(const GameTime& gameTime) override;

        [[nodiscard]] const Datum& GetActions() const;
        [[nodiscard]] const Action& GetAction(size_type index = size_type(0)) const;

        virtual void Clear() override;
        [[nodiscard]] virtual ScopeUniquePointer Clone() const override;
        [[nodiscard]] virtual std::string ToString() const override;

        void swap(ActionList&);

    protected:
        static const key_type ACTIONS_KEY;

        ActionList(IdType idOfSignaturesToAppend, const String& name);

        [[nodiscard]] Datum& Actions();
        [[nodiscard]] Action& ActionRef(size_type index = size_type(0));

    private:
        Datum* _actions{nullptr};

    };

    void swap(ActionList& lhs, ActionList& rhs);

    FACTORY(ActionList, Scope);
}

#include "ActionList.inl"
