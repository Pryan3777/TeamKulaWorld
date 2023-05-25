#include "pch.h"
#include "GameObject.h"
#include <sstream>

using namespace std::literals::string_literals;

namespace FieaGameEngine {
    RTTI_DEFINITIONS(GameObject);

    const typename GameObject::key_type GameObject::NAME_KEY = "Name"s;
    const typename GameObject::key_type GameObject::TRANSFORM_KEY = "Transform"s;
    const typename GameObject::key_type GameObject::CHILDREN_KEY = "Children"s;
    const typename GameObject::key_type GameObject::ACTIONS_KEY = "Actions"s;

    SignatureVector GameObject::Signatures() {
        using DatumType = FieaGameEngine::Datum::DatumType;
        using size_type = FieaGameEngine::Datum::size_type;

        return FieaGameEngine::SignatureVector{
            Signature{NAME_KEY, DatumType::String, true, size_type(1), offsetof(GameObject, _name)},
            Signature{TRANSFORM_KEY, DatumType::Table, true, size_type(1), offsetof(GameObject, _transform)},
            Signature{CHILDREN_KEY, DatumType::InternalTable, false, size_type(0), std::size_t(0)},
            Signature{ACTIONS_KEY, DatumType::InternalTable, false, size_type(0), std::size_t(0)}
        };
    }

    GameObject::GameObject(
        const String& name,
        const Transform::vec4& localPosition,
        const Transform::vec4& localRotation,
        const Transform::vec4& localScale
    ) : Attributed{GameObject::TypeIdClass()}, _name{name}, _transform{localPosition, localRotation, localScale} {
        _children = &(At(CHILDREN_KEY));
        _actions = &(At(ACTIONS_KEY));
    }

    GameObject::GameObject(GameObject&& other) noexcept : Attributed{std::move(other)}, _name{std::move(other._name)}, _transform{std::move(other._transform)} {
        _children = &(At(CHILDREN_KEY));
        _actions = &(At(ACTIONS_KEY));
        other._name.clear();
        other._transform.get().Reset();
    }

    GameObject& GameObject::operator=(GameObject&& other) noexcept {
        if (this != &other) {
            Attributed::operator=(std::move(other));
            _children = &(At(CHILDREN_KEY));
            _actions = &(At(ACTIONS_KEY));
            _name = std::move(other._name);
            _transform.get() = std::move(other._transform.get());
            other._name.clear();
            other._transform.get().Reset();
        }

        return *this;
    }

    void GameObject::UpdateChildren(const GameTime& gameTime) {
        auto& children = Children();
        for (auto i = size_type(0); i < children.Size(); ++i) {
            ChildRef(i).Update(gameTime);
        }
    }

    void GameObject::UpdateActions(const GameTime& gameTime) {
        auto& actions = Actions();
        for (auto i = size_type(0); i < actions.Size(); ++i) {
            ActionRef(i).Update(gameTime);
        }
    }

    GameObject& GameObject::CreateChild(const String& classname, const String& instname) {
        auto child = Factory<Scope>::StaticCreate(classname);

        if (!(child->Is(GameObject::TypeIdClass()))) {
            throw std::invalid_argument("Cannot create non-GameObject child of GameObject!"s);
        }

        static_cast<GameObject*>(child.get())->_name = instname;
        AttachAsChild(CHILDREN_KEY, std::move(child));
        return static_cast<GameObject&>(At(CHILDREN_KEY).BackTable());
    }

    Action& GameObject::CreateAction(const String& classname, const String& instname) {
        auto action = Factory<Scope>::StaticCreate(classname);

        if (!(action->Is(Action::TypeIdClass()))) {
            throw std::invalid_argument("Cannot create non-Action action of GameObject!");
        }

        static_cast<Action*>(action.get())->SetName(instname);
        AttachAsChild(ACTIONS_KEY, std::move(action));
        return static_cast<Action&>(At(ACTIONS_KEY).BackTable());
    }

    void GameObject::LocalTranslate(const Transform::vec4& translation) {
        _transform.get().LocalTranslate(translation);
    }

    void GameObject::Clear() {
        Attributed::Clear();
        assert(_children != nullptr);
        _children->Clear();
        assert(_actions != nullptr);
        _actions->Clear();
    }

    std::string GameObject::ToString() const {
        assert(_children != nullptr);
        assert(_actions != nullptr);
        std::stringstream tostring{};
        tostring << TypeNameInstance() << ": { \""s
            << NAME_KEY
            << "\": "s
            << _name
            << ", number of children: "s
            << _children->Size()
            << ", number of actions: "s
            << _actions->Size()
        << " }"s;
        return tostring.str();
    }

    void GameObject::swap(GameObject& other) {
        Attributed::swap(other);
        _children = &(At(CHILDREN_KEY));
        other._children = &(other.At(CHILDREN_KEY));
        _actions = &(At(ACTIONS_KEY));
        other._actions = &(other.At(ACTIONS_KEY));

        using std::swap;

        swap(_name, other._name);
        swap(_transform.get(), other._transform.get());
    }
}
