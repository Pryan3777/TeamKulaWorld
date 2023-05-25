#include "pch.h"
#include "Cube.h"
#include <cassert>

using namespace std::literals::string_literals;

namespace FieaGameEngine {
    RTTI_DEFINITIONS(Cube);

    const typename Cube::String Cube::DEFAULT_NAME = "Cube"s;
    const typename Cube::key_type Cube::RIGHT_KEY = "Right"s;
    const typename Cube::key_type Cube::UP_KEY = "Up"s;
    const typename Cube::key_type Cube::FORWARD_KEY = "Forward"s;
    const typename Cube::key_type Cube::LEFT_KEY = "Left"s;
    const typename Cube::key_type Cube::DOWN_KEY = "Down"s;
    const typename Cube::key_type Cube::BACKWARD_KEY = "Backward"s;

    SignatureVector Cube::Signatures() {
        return SignatureVector{
            Signature{RIGHT_KEY, DatumType::ExternalTable, true, size_type(1), offsetof(Cube, _rightOccupant)},
            Signature{UP_KEY, DatumType::ExternalTable, true, size_type(1), offsetof(Cube, _upOccupant)},
            Signature{FORWARD_KEY, DatumType::ExternalTable, true, size_type(1), offsetof(Cube, _forwardOccupant)},
            Signature{LEFT_KEY, DatumType::ExternalTable, true, size_type(1), offsetof(Cube, _leftOccupant)},
            Signature{DOWN_KEY, DatumType::ExternalTable, true, size_type(1), offsetof(Cube, _downOccupant)},
            Signature{BACKWARD_KEY, DatumType::ExternalTable, true, size_type(1), offsetof(Cube, _backwardOccupant)}
        };
    }

#pragma region Copy and Move
    Cube::Cube(const Cube& other)
        : GameObject{other}
        , _rightOccupant{other._rightOccupant.get()}
        , _upOccupant{other._upOccupant.get()}
        , _forwardOccupant{other._forwardOccupant.get()}
        , _leftOccupant{other._leftOccupant.get()}
        , _downOccupant{other._downOccupant.get()}
        , _backwardOccupant{other._backwardOccupant.get()}
    {
        _rightOccupant.get()._parent = this;
        _upOccupant.get()._parent = this;
        _forwardOccupant.get()._parent = this;
        _leftOccupant.get()._parent = this;
        _downOccupant.get()._parent = this;
        _backwardOccupant.get()._parent = this;
    }

    Cube::Cube(Cube&& other) noexcept
        : GameObject{std::move(other)}
        , _rightOccupant{std::move(other._rightOccupant.get())}
        , _upOccupant{std::move(other._upOccupant.get())}
        , _forwardOccupant{std::move(other._forwardOccupant.get())}
        , _leftOccupant{std::move(other._leftOccupant.get())}
        , _downOccupant{std::move(other._downOccupant.get())}
        , _backwardOccupant{std::move(other._backwardOccupant.get())}
    {
        _rightOccupant.get()._parent = this;
        _upOccupant.get()._parent = this;
        _forwardOccupant.get()._parent = this;
        _leftOccupant.get()._parent = this;
        _downOccupant.get()._parent = this;
        _backwardOccupant.get()._parent = this;
    }

    Cube& Cube::operator=(const Cube& other) {
        if (this != &other) {
            GameObject::operator=(other);
            _rightOccupant.get() = other._rightOccupant.get();
            _rightOccupant.get()._parent = this;
            _upOccupant.get() = other._upOccupant.get();
            _upOccupant.get()._parent = this;
            _forwardOccupant.get() = other._forwardOccupant.get();
            _forwardOccupant.get()._parent = this;
            _leftOccupant.get() = other._leftOccupant.get();
            _leftOccupant.get()._parent = this;
            _downOccupant.get() = other._downOccupant.get();
            _downOccupant.get()._parent = this;
            _backwardOccupant.get() = other._backwardOccupant.get();
            _backwardOccupant.get()._parent = this;
        }

        return *this;
    }

    Cube& Cube::operator=(Cube&& other) noexcept {
        if (this != &other) {
            GameObject::operator=(std::move(other));
            _rightOccupant.get() = std::move(other._rightOccupant.get());
            _rightOccupant.get()._parent = this;
            _upOccupant.get() = std::move(other._upOccupant.get());
            _upOccupant.get()._parent = this;
            _forwardOccupant.get() = std::move(other._forwardOccupant.get());
            _forwardOccupant.get()._parent = this;
            _leftOccupant.get() = std::move(other._leftOccupant.get());
            _leftOccupant.get()._parent = this;
            _downOccupant.get() = std::move(other._downOccupant.get());
            _downOccupant.get()._parent = this;
            _backwardOccupant.get() = std::move(other._backwardOccupant.get());
            _backwardOccupant.get()._parent = this;
        }

        return *this;
    }

    Cube::Cube(const String& name) : GameObject{Cube::TypeIdClass(), name} {
        _rightOccupant.get()._parent = this;
        _rightOccupant.get()._occupiedFace = Direction3D::Right;
        _upOccupant.get()._parent = this;
        _upOccupant.get()._occupiedFace = Direction3D::Up;
        _forwardOccupant.get()._parent = this;
        _forwardOccupant.get()._occupiedFace = Direction3D::Forward;
        _leftOccupant.get()._parent = this;
        _leftOccupant.get()._occupiedFace = Direction3D::Left;
        _downOccupant.get()._parent = this;
        _downOccupant.get()._occupiedFace = Direction3D::Down;
        _backwardOccupant.get()._parent = this;
        _backwardOccupant.get()._occupiedFace = Direction3D::Backward;
    }
#pragma endregion

    bool Cube::IsAtPosition(int x, int y, int z) const {
        assert(_cubeModel);
        const auto& currentPosition = Position();
        auto tolerance = 0.4f;
        return (std::abs(x - currentPosition.x) < tolerance)
            && (std::abs(y - currentPosition.y) < tolerance)
            && (std::abs(z - currentPosition.z) < tolerance);
    }
}
