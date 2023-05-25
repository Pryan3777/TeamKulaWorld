#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "Game.h"
#include "Player.h"
#include "Direction3D.h"
#include "PlayerOntoCubeEventArgs.h"

namespace FieaGameEngine
{
	class KeyboardComponent;
	class MouseComponent;
	class GamePadComponent;
	class FpsComponent;
	class Grid;
	class FirstPersonCamera;
}

namespace Rendering
{
	class KulaGameInstance;

	class RenderingGame final : public FieaGameEngine::Game
	{
	public:
		RenderingGame(std::function<void*()> getWindowCallback, std::function<void(SIZE&)> getRenderTargetSizeCallback);

		virtual void Initialize() override;
		virtual void Update(const FieaGameEngine::GameTime& gameTime) override;
		virtual void Draw(const FieaGameEngine::GameTime& gameTime) override;
		virtual void Shutdown() override;

		void ResetKulaGameInstance();

		bool RenderStartScreen(const FieaGameEngine::GameTime& gameTime);
		bool RenderEndScreen(const FieaGameEngine::GameTime& gameTime, size_t Score, size_t Time);
		void PlayMenuMusic();
		void PlayLevelMusic();
		void PlayScoreSfx();
		void SwivelUp();
		void SwivelDown();

		void Exit();
		std::shared_ptr<FieaGameEngine::ScoreComponent> mScoreComponent;
		std::unique_ptr<FieaGameEngine::Player> CurrentPlayer;

	private:
		enum MovementCase : std::uint8_t {
			ROLLING_UP = 0,
			ROLLING_AHEAD,
			ROLLING_AROUND,
			MOVEMENT_CASE_UNKNOWN
		};

		inline static const DirectX::XMVECTORF32 BackgroundColor{ DirectX::Colors::CornflowerBlue };
		void UpdateBall(const FieaGameEngine::GameTime& gameTime);
		std::pair<MovementCase, FieaGameEngine::PlayerOntoCubeEventArgs> ComputeMovementCase(const FieaGameEngine::Player& player);

		void ExecuteMovementCase1();

		std::shared_ptr<FieaGameEngine::KeyboardComponent> mKeyboard;
		std::shared_ptr<FieaGameEngine::MouseComponent> mMouse;
		std::shared_ptr<FieaGameEngine::GamePadComponent> mGamePad;
		std::shared_ptr<FieaGameEngine::Grid> mGrid;

		size_t KulaIndex;

		//Peyton stuff
		float acummulatedRotation{};
		DirectX::XMFLOAT3 cachedDirection{};
		DirectX::XMFLOAT3 cachedUp{};
		DirectX::XMMATRIX storedRotation{};
		bool isStartup{ true };
		bool isTurning{ false };
		bool isTurningLeft{ false };
		bool isTurningRight{ false };
		bool isTurningClockwise{ false };
		bool isTurningCounterClockwise{ false };
		bool isTurningUp{ false };
		bool isTurningDown{ false };
		bool isMovingForward{ false };
		bool canTurnClockwise{ true };
		bool canTurnCounterClockwise{ true };

		bool hasEnteredMenu{ false };
		Mix_Music* menuMusic{ nullptr };
		Mix_Music* levelMusic{ nullptr };

		Mix_Chunk* scoreSfx{ nullptr };
		//Arun stuff
		bool _isMoving{ false };
		bool _isTurning{ false };
		int _movementState = 0;
		MovementCase _currentMovementCase;
		DirectX::XMFLOAT3 _cachedPosition{};
		DirectX::XMFLOAT3 _targetPosition{};
		DirectX::XMFLOAT3 _startPosition{};
		DirectX::XMFLOAT3 _testTargetDirection{};
		DirectX::XMFLOAT3 _testTargetUp{};
		DirectX::XMFLOAT3 _midpoint{};
		float _moveInterpolation{0.f};
		float _moveDurationSeconds{0.1f};

		float _targetDistance{ 0 };
		float _distanceVec{ 0 };
		float _moveTolerance{ 0.5f };
		float _moveSpeed{ 0.01f };
		float _moveTime{ 1.f };
		float _rotationAngleTolerance{ 0.001f };
		float _rotationAngleIncrement{ 0.05f };
		float _accumulatedRotation{ 0.f };
		float _camOffsetBack{ -7.5f };
		float _camOffsetUp{ 2.f };
		DirectX::XMFLOAT3 _activeRotationAxis{};
		std::unique_ptr<FieaGameEngine::PlayerOntoCubeEventArgs> _movementArgs;

		int32_t levelSelected{ 1 };
		std::shared_ptr<FieaGameEngine::FirstPersonCamera> mCamera;
	};
}