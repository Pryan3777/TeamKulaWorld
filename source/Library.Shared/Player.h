#pragma once
#include "EventSubscriber.h"
#include "GameObject.h"
#include "ProxyModel.h"
#include "Direction3D.h"
#include "BallModel.h"
#include <DirectXMath.h>
#include <tuple>
namespace FieaGameEngine
{
	class Game;
	class Camera;
	class Player : public GameObject
	{
		RTTI_DECLARATIONS(Player, FieaGameEngine::GameObject);
	public:
		Datum::Integer Score = 0;
		Player();
		inline explicit Player(String name) : GameObject{ Player::TypeIdClass(), name } {}

		static SignatureVector Signatures();

		void Initialize(Game& game, const std::shared_ptr<Camera>& camera);

		virtual void UpdateSelf(const GameTime& gameTime) override;

		static float GetPlayerScale();

		std::shared_ptr<EventSubscriber> EventListener;
		bool PausedGame = true;

		inline Direction3D GetStartingDirection() const { return static_cast<Direction3D>(_startingDirection); }
		inline Direction3D GetStartingFace() const { return static_cast<Direction3D>(_startingFace); }
		inline int GetStartingCube() const { return _startingCubeID; }
		inline FieaGameEngine::Direction3DInfo GetDirection() const { return _currentDirection; }
		inline FieaGameEngine::Direction3DInfo GetFace() const { return _currentFace; }
		inline FieaGameEngine::IntVector3D GetCoordinate() const { return _currentCoordinate; }
		inline void SetDirection(Direction3D v) {_currentDirection.SetDirection(v); };
		inline void SetFace(Direction3D f) { _currentFace.SetDirection(f); };
		inline void SetCoordinate(IntVector3D t) { _currentCoordinate = t; }
		inline void TurnLeft(){ _currentDirection = CrossProduct(_currentDirection, _currentFace).GetDirection(); }
		inline void TurnRight() { _currentDirection = CrossProduct(_currentFace, _currentDirection).GetDirection(); }
		
		void SetMovementCase(int moveCase) { _movementCase = moveCase; }
		void SetTargetTransform(IntVector3D targetCoordinate, Direction3D targetDirection, Direction3D targetFace);
		void SetTargetDirection(Direction3D targetDirection);
		void UpdateToTargetTransform();
		void UpdateToTargetDirection();
		void Reconcile();

		// Wrappers for Model Functions
		inline const DirectX::XMFLOAT3& Direction() const { return _playerModel->Direction(); };
		inline const DirectX::XMFLOAT3& Up() const { return _playerModel->Up(); };
		inline const DirectX::XMFLOAT3& Right() const { return _playerModel->Right(); };
		inline DirectX::XMVECTOR PositionVector() const { return _playerModel->PositionVector(); };
		inline DirectX::XMVECTOR DirectionVector() const { return _playerModel->DirectionVector(); };
		inline DirectX::XMVECTOR UpVector() const { return _playerModel->UpVector(); };
		inline DirectX::XMVECTOR RightVector() const { return _playerModel->RightVector(); };
		inline void SetPosition(float x, float y, float z) { _playerModel->SetPosition(x, y, z); };
		inline void SetPosition(DirectX::FXMVECTOR position) { _playerModel->SetPosition(position); };
		inline void SetPosition(const DirectX::XMFLOAT3& position) { _playerModel->SetPosition(position); };
		inline void SetTransform(DirectX::XMFLOAT3 targetDirection, DirectX::XMFLOAT3 targetUp) { _playerModel->SetTransform(targetDirection, targetUp); };
		inline void ApplyRotation(DirectX::CXMMATRIX transform) { _playerModel->ApplyRotation(transform); };
		inline void ApplyRotation(DirectX::CXMMATRIX transform, DirectX::XMFLOAT3 direction, DirectX::XMFLOAT3 up) { _playerModel->ApplyRotation(transform, direction, up); };
		inline void ApplyRotation(const DirectX::XMFLOAT4X4& transform) { _playerModel->ApplyRotation(transform); };

		DirectX::FXMVECTOR OffsetPositionVector() const;
		void SetOffsetPositionVector(DirectX::FXMVECTOR position);
		void SetOffsetPosition(DirectX::XMFLOAT3 position);

		inline Rendering::BallModel& GetModel() { return *_playerModel.get(); };
	private:
		std::unique_ptr<Rendering::BallModel> _playerModel;
		FieaGameEngine::Direction3DInfo _currentDirection{Direction3D::Up};
		FieaGameEngine::Direction3DInfo _currentFace{Direction3D::Forward};
		IntVector3D _currentCoordinate;

		int _movementCase = 0;
		IntVector3D _targetCoordinate;
		FieaGameEngine::Direction3DInfo _targetDirection{ Direction3D::Up };
		FieaGameEngine::Direction3DInfo _targetFace{ Direction3D::Forward };

		bool _isMoving;
		bool _isTurning;
		Datum::Integer _startingDirection;
		Datum::Integer _startingFace;
		Datum::Integer _startingCubeID;
	};
	FACTORY(Player, FieaGameEngine::Scope);
}

