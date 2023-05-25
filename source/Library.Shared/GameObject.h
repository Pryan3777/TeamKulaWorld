#pragma once
#include "Action.h"
#include "Attributed.h"
#include "AttributedSignatureRegistry.h"
#include "Factory.h"
#include "GameTime.h"
#include "Transform.h"

namespace FieaGameEngine {
    class GameObject : public Attributed {
        RTTI_DECLARATIONS(GameObject, Attributed);

    public:
        static SignatureVector Signatures();

        GameObject(const String& name, const Transform::vec4& localPosition, const Transform::vec4& localRotation, const Transform::vec4& localScale);
        GameObject(const String& name, const Transform::vec4& localPosition);
        explicit GameObject(const String& name);

        GameObject(const GameObject&);
        GameObject(GameObject&&) noexcept;
        GameObject& operator=(const GameObject&);
        GameObject& operator=(GameObject&&) noexcept;
        virtual ~GameObject();

        [[nodiscard]] const String& GetName() const;
        [[nodiscard]] const FieaGameEngine::Transform& GetTransform() const;
        [[nodiscard]] const Datum& GetChildren() const;
        [[nodiscard]] const Datum& GetActions() const;
        [[nodiscard]] const GameObject& GetChild(size_type index = size_type(0)) const;
        [[nodiscard]] const Action& GetAction(size_type index = size_type(0)) const;

        GameObject& CreateChild(const String& instname);
        GameObject& CreateChild(const String& classname, const String& instname);
        Action& CreateAction(const String& classname, const String& instname);
        void LocalTranslate(const Transform::vec4& translation);

        virtual void UpdateSelf(const GameTime& gameTime);
        void UpdateChildren(const GameTime& gameTime);
        void UpdateActions(const GameTime& gameTime);
        void Update(const GameTime& gameTime);

        virtual void Clear() override;
        [[nodiscard]] virtual ScopeUniquePointer Clone() const override;
        [[nodiscard]] virtual std::string ToString() const override;

        void swap(GameObject&);

    protected:
        static const key_type NAME_KEY;
        static const key_type TRANSFORM_KEY;
        static const key_type CHILDREN_KEY;
        static const key_type ACTIONS_KEY;

        GameObject(IdType idOfSignaturesToAppend, const String& name);

        // TODO - Create map of children

        [[nodiscard]] String& Name();
        [[nodiscard]] FieaGameEngine::Transform& Transform();
        [[nodiscard]] Datum& Children();
        [[nodiscard]] GameObject& ChildRef(size_type index = size_type(0));
        [[nodiscard]] Datum& Actions();
        [[nodiscard]] Action& ActionRef(size_type index = size_type(0));

    private:
        String _name{};
        ScopeAttribute<FieaGameEngine::Transform> _transform{};
        Datum* _children{nullptr};
        Datum* _actions{nullptr};

    };

    void swap(GameObject& lhs, GameObject& rhs);

    FACTORY(GameObject, Scope, Datum::String{});
}

#include "GameObject.inl"
