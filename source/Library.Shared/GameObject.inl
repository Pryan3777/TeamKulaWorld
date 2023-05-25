#pragma once
#include "GameObject.h"
#include <cassert>

namespace FieaGameEngine {
    inline GameObject::GameObject(const String& name) : GameObject{name, Transform::ZERO} {}
    inline GameObject::GameObject(const String& name, const Transform::vec4& localPosition) : GameObject{name, localPosition, Transform::ZERO, Transform::ONE} {}
    inline GameObject::GameObject(IdType idOfSignaturesToAppend, const String& name) : Attributed{idOfSignaturesToAppend}, _name{name} {
        _children = &(At(CHILDREN_KEY));
        _actions = &(At(ACTIONS_KEY));
        _transform.get().TrySetParent(this);
    }
    inline GameObject::GameObject(const GameObject& other) : Attributed{other}, _name{other._name}, _transform{other._transform} {
        _children = &(At(CHILDREN_KEY));
        _actions = &(At(ACTIONS_KEY));
    }
    inline GameObject& GameObject::operator=(const GameObject& other) { GameObject copy{other}; swap(copy); return *this; }
    inline GameObject::~GameObject() { _children = nullptr; _actions = nullptr; }

    inline GameObject& GameObject::CreateChild(const String& instname) { return CreateChild(GameObject::TypeNameClass(), instname); }
    inline void GameObject::UpdateSelf(const GameTime&) {}
    inline void GameObject::Update(const GameTime& gameTime) { UpdateSelf(gameTime); UpdateActions(gameTime); UpdateChildren(gameTime); }

    inline typename GameObject::String& GameObject::Name() { return _name; }
    inline const typename GameObject::String& GameObject::GetName() const { return _name; }
    inline FieaGameEngine::Transform& GameObject::Transform() { return _transform.get(); }
    inline const FieaGameEngine::Transform& GameObject::GetTransform() const { return _transform.cget(); }
    inline Datum& GameObject::Children() { assert(_children != nullptr); return *_children; }
    inline const Datum& GameObject::GetChildren() const { assert(_children != nullptr); return *_children; }
    inline const GameObject& GameObject::GetChild(size_type index) const { return const_cast<GameObject*>(this)->ChildRef(index); }
    inline GameObject& GameObject::ChildRef(size_type index) {
        assert(_children != nullptr);
        Scope& child = _children->GetTableElement(index);
        assert(child.Is(GameObject::TypeIdClass()));
        return static_cast<GameObject&>(child);
    }
    inline Datum& GameObject::Actions() { assert(_actions != nullptr); return *_actions; }
    inline const Datum& GameObject::GetActions() const { assert(_actions != nullptr); return *_actions; }
    inline const Action& GameObject::GetAction(size_type index) const { return const_cast<GameObject*>(this)->ActionRef(index); }
    inline Action& GameObject::ActionRef(size_type index) {
        assert(_actions != nullptr);
        Scope& action = _actions->GetTableElement(index);
        assert(action.Is(Action::TypeIdClass()));
        return static_cast<Action&>(action);
    }
    inline typename GameObject::ScopeUniquePointer GameObject::Clone() const { return std::make_unique<GameObject>(*this); }

    inline void swap(GameObject& lhs, GameObject& rhs) { lhs.swap(rhs); }
}
