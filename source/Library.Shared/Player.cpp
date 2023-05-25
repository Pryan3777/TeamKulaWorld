#include "pch.h"
#include "Player.h"
#include "Game.h"
#include "Camera.h"
#include "ProxyModel.h"
#include <DirectXMathVector.inl>

using namespace std::literals::string_literals;
using namespace DirectX;

#define PLAYER_MODEL_PATH "Models\\beach_ball.model"
#define PLAYER_SCALE 1.f
namespace FieaGameEngine {
    RTTI_DEFINITIONS(Player);
    Player::Player() : Player{ "Player"s } {}
    
    SignatureVector Player::Signatures() { return SignatureVector{ 
        Signature{ "Score"s, DatumType::Integer, true, size_type(1), offsetof(Player, Score) },
        Signature{ "PlayerDirection"s, DatumType::Integer, true, size_type(1), offsetof(Player, _startingDirection) },
        Signature{ "CubeFace"s, DatumType::Integer, true, size_type(1), offsetof(Player, _startingFace) },
        Signature{ "CubeID"s, DatumType::Integer, true, size_type(1), offsetof(Player, _startingCubeID) }
    };
    }

    void Player::Initialize(Game& game, const std::shared_ptr<Camera>& camera)
    {
        _currentDirection = static_cast<Direction3D>(_startingDirection);
        _currentFace = static_cast<Direction3D>(_startingFace);
        _playerModel = std::make_unique<Rendering::BallModel>(game, camera, PLAYER_SCALE);
        _playerModel->Initialize();
    }

    void Player::UpdateSelf(const GameTime&)
    {
        At("Score"s).SetElement(At("Score"s).GetIntegerElement() + 1);
    }

    float Player::GetPlayerScale()
    {
        return PLAYER_SCALE;
    }

    void Player::SetTargetTransform(IntVector3D targetCoordinate, Direction3D targetDirection, Direction3D targetFace)
    {
        _targetCoordinate = targetCoordinate;
        _targetDirection = targetDirection;
        _targetFace = targetFace;
    }

    void Player::SetTargetDirection(Direction3D targetDirection)
    {
        _targetDirection = targetDirection;
    }

    void Player::UpdateToTargetTransform()
    {
        _currentCoordinate = _targetCoordinate;
        _currentDirection = _targetDirection;
        _currentFace = _targetFace;
    }

    void Player::UpdateToTargetDirection()
    {
        _currentDirection = _targetDirection;
    }

    void Player::Reconcile()
    {
        auto directionVEC = _currentDirection.ToXMFLOAT3();
        auto upVEC = _currentFace.ToXMFLOAT3();
        //auto positionVEC = _currentCoordinate.ConvertToXMFLOAT3();

        SetTransform(directionVEC, upVEC);
        //SetPosition(positionVEC);
    }

    FXMVECTOR Player::OffsetPositionVector() const {
        auto up = XMFLOAT3{0.f, PLAYER_SCALE, 0.f};
        XMVECTOR itReallyIsUpTrustMe = XMLoadFloat3(&up);
        return _playerModel->PositionVector() - itReallyIsUpTrustMe;
    }

    void Player::SetOffsetPositionVector(FXMVECTOR position) {
        auto up = XMFLOAT3{ 0.f, PLAYER_SCALE, 0.f };
        _playerModel->SetPosition(position + XMLoadFloat3(&up));
    }

    void Player::SetOffsetPosition(XMFLOAT3 position) {
        SetOffsetPositionVector(XMLoadFloat3(&position));
    }
}
