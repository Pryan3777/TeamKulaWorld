#include "pch.h"
#include "RenderingGame.h"
#include "GameException.h"
#include "KeyboardComponent.h"
#include "MouseComponent.h"
#include "GamePadComponent.h"
#include "FpsComponent.h"
#include "ScoreComponent.h"
#include "KulaGameInstance.h"
#include "FirstPersonCamera.h"
#include "Grid.h"
#include "SamplerStates.h"
#include "RasterizerStates.h"
#include "VectorHelper.h"
#include "ImGuiComponent.h"
#include "imgui_impl_dx11.h"
#include "UtilityWin32.h"
#include "dwrite.h"
#include "d2d1.h"
#include "Direction3D.h"
#include <limits>
#include <imgui.h>
#include "BasicTitle.h"
#include "FirstPersonCamera.h"
#include "GameplayState.h"
#include "Texture2DReader.h"
#include "BasicSprite.h"

using namespace std;
using namespace DirectX;
using namespace FieaGameEngine;

IMGUI_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Rendering
{
	RenderingGame::RenderingGame(std::function<void*()> getWindowCallback, std::function<void(SIZE&)> getRenderTargetSizeCallback) :
		Game(getWindowCallback, getRenderTargetSizeCallback)
	{
	}

	void RenderingGame::ResetKulaGameInstance()
	{

		static_cast<FirstPersonCamera*>(mCamera.get())->ClearCallbacks();

		mComponents[KulaIndex] = make_shared<KulaGameInstance>(*this, mCamera, levelSelected);
		mComponents[KulaIndex].get()->Initialize();

		mScoreComponent.reset();
		mComponents[KulaIndex+1].reset();

		mScoreComponent = make_shared<ScoreComponent>(*this);
		mComponents[KulaIndex + 1] = mScoreComponent;

		mScoreComponent->Initialize();
	}

	void RenderingGame::Initialize()
	{
		auto direct3DDevice = Direct3DDevice();
		SamplerStates::Initialize(direct3DDevice);
		RasterizerStates::Initialize(direct3DDevice);

		mKeyboard = make_shared<KeyboardComponent>(*this);
		mComponents.push_back(mKeyboard);
		mServices.AddService(KeyboardComponent::TypeIdClass(), mKeyboard.get());

		mMouse = make_shared<MouseComponent>(*this, MouseModes::Absolute);
		mComponents.push_back(mMouse);
		mServices.AddService(MouseComponent::TypeIdClass(), mMouse.get());

		mGamePad = make_shared<GamePadComponent>(*this);
		mComponents.push_back(mGamePad);
		mServices.AddService(GamePadComponent::TypeIdClass(), mGamePad.get());

		mCamera = make_shared<FirstPersonCamera>(*this);
		mComponents.push_back(mCamera);
		mServices.AddService(Camera::TypeIdClass(), mCamera.get());

		mGrid = make_shared<Grid>(*this, mCamera);
		mComponents.push_back(mGrid);

		auto newValue = make_shared<KulaGameInstance>(*this, mCamera, levelSelected);
		mComponents.push_back(std::move(newValue));
		KulaIndex = mComponents.size();

		mScoreComponent = make_shared<ScoreComponent>(*this);
		mScoreComponent.get()->SetVisible(true);
		mScoreComponent.get()->Initialize();
		mComponents.push_back(mScoreComponent);

		auto imGui = make_shared<ImGuiComponent>(*this);
		mComponents.push_back(imGui);
		mServices.AddService(ImGuiComponent::TypeIdClass(), imGui.get());
		auto imGuiWndProcHandler = make_shared<UtilityWin32::WndProcHandler>(ImGui_ImplWin32_WndProcHandler);
		UtilityWin32::AddWndProcHandler(imGuiWndProcHandler);

		Game::Initialize();

		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		
		mCamera->SetPosition(0.0f, 2.5f, 20.0f);

		levelMusic = Mix_LoadMUS("Content/Audio/BGM4.wav");
		menuMusic = Mix_LoadMUS("Content/Audio/BGM1.wav");

		scoreSfx = Mix_LoadWAV("Content/Audio/HIRO_00014.wav");

		PlayMenuMusic();
	}

	void RenderingGame::Update(const GameTime& gameTime)
	{
		if (mKeyboard->WasKeyPressedThisFrame(Keys::Escape) || mGamePad->WasButtonPressedThisFrame(GamePadButtons::Back))
		{
			Exit();
		}

		if (mMouse->WasButtonPressedThisFrame(MouseButtons::Left))
		{
			mMouse->SetMode(MouseModes::Relative);
		}

		if (mMouse->WasButtonReleasedThisFrame(MouseButtons::Left))
		{
			mMouse->SetMode(MouseModes::Absolute);
		}

		if (mKeyboard->WasKeyPressedThisFrame(Keys::G))
		{
			mGrid->SetVisible(!mGrid->Visible());
		}

		UpdateBall(gameTime);

		Game::Update(gameTime);
	}

	void RenderingGame::Draw(const GameTime &gameTime)
	{
		mDirect3DDeviceContext->ClearRenderTargetView(mRenderTargetView.get(), BackgroundColor.f);
		mDirect3DDeviceContext->ClearDepthStencilView(mDepthStencilView.get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		Game::Draw(gameTime);

		HRESULT hr = mSwapChain->Present(1, 0);

		// If the device was removed either by a disconnection or a driver upgrade, we must recreate all device resources.
		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			HandleDeviceLost();
		}
		else
		{
			ThrowIfFailed(hr, "IDXGISwapChain::Present() failed.");
		}
	}

	void RenderingGame::Shutdown()
	{
		mGrid = nullptr;
		mScoreComponent = nullptr;
		mComponents[KulaIndex] = nullptr;
		Mix_FreeMusic(menuMusic);
		Mix_FreeMusic(levelMusic);
		Mix_FreeChunk(scoreSfx);
		Mix_CloseAudio();
		SDL_TLSCleanup();
		Mix_Quit();
		RasterizerStates::Shutdown();
		SamplerStates::Shutdown();
		Game::Shutdown();
	}

	void RenderingGame::Exit()
	{
		PostQuitMessage(0);
	}
	

	bool RenderingGame::RenderStartScreen(const FieaGameEngine::GameTime& gameTime)
	{
		if (!hasEnteredMenu)
		{
			PlayMenuMusic();
			hasEnteredMenu = true;
		}
		mKeyboard->Update(gameTime);
		float ClearColor[4] = { 0.5f, 0.125f, 0.3f, 1.0f };
		mDirect3DDeviceContext->ClearRenderTargetView(mRenderTargetView.get(), ClearColor);
		
		DirectX::XMFLOAT2 bgPosition{ 70.0f, 0.0f };
		BasicSprite bgSprite(*this, bgPosition, L"Content\\Textures\\bg.png");
		bgSprite.Initialize();
		bgSprite.Draw(gameTime);

		DirectX::XMFLOAT2 titleTextPosition{ 230.0f, 50.0f };
		BasicTitle titleScreen(*this, titleTextPosition, "Kula World"s, true);
		titleScreen.Initialize();
		titleScreen.Draw(gameTime);

		DirectX::XMFLOAT2 teamTextPosition{ 320.0f, 150.0f };
		BasicTitle teamScreen(*this, teamTextPosition, "By Team A"s, false);
		teamScreen.Initialize();
		teamScreen.Draw(gameTime);

		/*DirectX::XMFLOAT2 pixelBallPosition{ 320.0f, 250.0f };
		BasicSprite ball(*this, pixelBallPosition, L"Content\\Textures\\ball.png");
		ball.Initialize();
		ball.Draw(gameTime);*/

		DirectX::XMFLOAT2 startTextPosition{ 200.0f, 600.0f };
		BasicTitle startButton(*this, startTextPosition, "Press Enter to begin"s, false);
		startButton.Initialize();
		startButton.Draw(gameTime);

		DirectX::XMFLOAT2 levelSelectTextPosition{ 200.0f, 675.0f };
		BasicTitle levelSelectTxt(*this, levelSelectTextPosition, "Level Selected: " + std::to_string(levelSelected), false);
		levelSelectTxt.Initialize();
		levelSelectTxt.Draw(gameTime);

		mSwapChain->Present(1, 0);
		
		if (mKeyboard->IsKeyDown(Keys::D1))
		{
			levelSelected = 1;
		}
		else if (mKeyboard->IsKeyDown(Keys::D2))
		{
			levelSelected = 2;
		}
		else if (mKeyboard->IsKeyDown(Keys::D3))
		{
			levelSelected = 3;
		}

		return mKeyboard->WasKeyPressedThisFrame(Keys::Enter);
	}

	bool RenderingGame::RenderEndScreen(const FieaGameEngine::GameTime& gameTime, size_t Score, size_t Time)
	{
		mKeyboard->Update(gameTime);
		float ClearColor[4] = { 0.5f, 0.125f, 0.3f, 1.0f };
		mDirect3DDeviceContext->ClearRenderTargetView(mRenderTargetView.get(), ClearColor);

		DirectX::XMFLOAT2 bgPosition{ 70.0f, 0.0f };
		BasicSprite bgSprite(*this, bgPosition, L"Content\\Textures\\bg.png");
		bgSprite.Initialize();
		bgSprite.Draw(gameTime);

		DirectX::XMFLOAT2 titleTextPosition{ 230.0f, 50.0f };
		BasicTitle titleScreen(*this, titleTextPosition, "Game Over!!"s, true);
		titleScreen.Initialize();
		titleScreen.Draw(gameTime);

		DirectX::XMFLOAT2 ScoreTextPosition{ 320.0f, 150.0f };
		BasicTitle scoreScreen(*this, ScoreTextPosition, "Score: "s + std::to_string(Score), false);
		scoreScreen.Initialize();
		scoreScreen.Draw(gameTime);

		/*DirectX::XMFLOAT2 pixelBallPosition{ 320.0f, 250.0f };
		BasicSprite ball(*this, pixelBallPosition, L"Content\\Textures\\ball.png");
		ball.Initialize();
		ball.Draw(gameTime);*/

		DirectX::XMFLOAT2 TimeTextPosition{ 320.0f, 675.0f };
		BasicTitle timeScreen(*this, TimeTextPosition, "Time Left: "s + std::to_string(Time), false);
		timeScreen.Initialize();
		timeScreen.Draw(gameTime);

		DirectX::XMFLOAT2 startTextPosition{ 180.0f, 600.0f };
		BasicTitle startButton(*this, startTextPosition, "Press Enter to Restart!"s, false);
		startButton.Initialize();
		startButton.Draw(gameTime);

		mSwapChain->Present(1, 0);
		return mKeyboard->WasKeyPressedThisFrame(Keys::Enter);
	}

	void RenderingGame::PlayMenuMusic()
	{
		Mix_PlayMusic(menuMusic, -1);
	}

	void RenderingGame::PlayLevelMusic()
	{
		Mix_PlayMusic(levelMusic, -1);
		hasEnteredMenu = false;
	}

	void RenderingGame::PlayScoreSfx()
	{
	}

	void RenderingGame::SwivelUp()
	{
		if (!isTurning)
		{
			FieaGameEngine::Player& player = static_cast<KulaGameInstance*>(mComponents[KulaIndex].get())->GetPlayer();
			storedRotation = XMMatrixRotationY(0.f) * XMMatrixRotationAxis(player.RightVector(), -XM_PIDIV2);
			cachedDirection = player.Direction();
			cachedUp = player.Up();
			isTurning = true;
			isTurningUp = true;
		}
	}

	void RenderingGame::SwivelDown()
	{
		if (!isTurning)
		{
			FieaGameEngine::Player& player = static_cast<KulaGameInstance*>(mComponents[KulaIndex].get())->GetPlayer();
			storedRotation = XMMatrixRotationY(0.0f) * XMMatrixRotationAxis(player.RightVector(), XM_PIDIV2);
			cachedDirection = player.Direction();
			cachedUp = player.Up();
			isTurning = true;
			isTurningDown = true;
		}
	}

	void RenderingGame::UpdateBall(const FieaGameEngine::GameTime& gameTime)
	{
		float elapsedTime = gameTime.ElapsedGameTimeSeconds().count();

		XMFLOAT3 movementAmount = Vector3Helper::Zero;
		const XMFLOAT3 BallRotationRate{ DirectX::XM_2PI, DirectX::XM_2PI, DirectX::XM_2PI };
		XMFLOAT3 rotationAmount = Vector3Helper::Zero;

		if (mKeyboard->IsKeyUp(Keys::A))
		{
			if (!canTurnClockwise)
			{
				canTurnClockwise = true;
			}
		}

		if (mKeyboard->IsKeyUp(Keys::D))
		{
			if (!canTurnCounterClockwise)
			{
				canTurnCounterClockwise = true;
			}
		}

		if (!_isMoving && mKeyboard->WasKeyPressedThisFrame(Keys::W) && !isTurning)
		{
			FieaGameEngine::Player& player = static_cast<KulaGameInstance*>(mComponents[KulaIndex].get())->GetPlayer();
			XMStoreFloat3(&_startPosition, player.PositionVector());
			auto [moveCase, movementArgs] = ComputeMovementCase(player);

			_currentMovementCase = moveCase;
			_movementArgs.reset();
			_movementArgs = std::make_unique<PlayerOntoCubeEventArgs>(std::move(movementArgs));

			auto& level = static_cast<KulaGameInstance*>(mComponents[KulaIndex].get())->GetLevel();
			const auto& nextCube = level.GetCube(_movementArgs->GetNextCubePosition());
			auto capture = _movementArgs->GetNextCubeFace().ToXMFLOAT3();
			XMVECTOR nextPositionOffset = XMLoadFloat3(&capture);
			nextPositionOffset *= Player::GetPlayerScale() + (Level::GetCubeSideLength() / 2.f);
			XMStoreFloat3(&_targetPosition, nextCube->PositionVector() + nextPositionOffset);
			capture = _movementArgs->GetPreviousDirection().ToXMFLOAT3(true);
			XMVECTOR midpointUnitVector = XMLoadFloat3(&capture);
			bool isDirectionUnchanged = (_movementArgs->GetPreviousDirection()) == (_movementArgs->GetNextDirection());
			XMStoreFloat3(
				&_midpoint,
				player.PositionVector() + (
					(isDirectionUnchanged ? 0.5f : 1.f)
					* midpointUnitVector
					* (XMLoadFloat3(&_targetPosition) - player.PositionVector())
				)
			);
			_isMoving = true;
			_moveInterpolation = 0.f;
		}
		
		if (isTurningUp)
		{
			float addedRotation = BallRotationRate.x * elapsedTime;
			acummulatedRotation += addedRotation;
			if (XM_PIDIV2 - acummulatedRotation <= 0.f)
			{
				FieaGameEngine::Player& player = static_cast<KulaGameInstance*>(mComponents[KulaIndex].get())->GetPlayer();
				player.ApplyRotation(storedRotation, cachedDirection, cachedUp);
				
				if (mCamera != nullptr)
				{
					mCamera->ApplyRotation(storedRotation, cachedDirection, cachedUp);
				}

				auto directionVec = player.DirectionVector();
				auto upVec = player.UpVector();
				auto position = player.PositionVector();
				upVec *= _camOffsetUp;
				directionVec *= _camOffsetBack;
				mCamera->SetPosition(position + directionVec + upVec);

				isTurningUp = false;
				isTurning = false;
				acummulatedRotation = 0.0f;
			}
			else
			{
				rotationAmount.y -= addedRotation;
			}
		}

		if (isTurningDown)
		{
			float addedRotation = BallRotationRate.x * elapsedTime;
			acummulatedRotation += addedRotation;
			if (XM_PIDIV2 - acummulatedRotation <= 0.f)
			{
				FieaGameEngine::Player& player = static_cast<KulaGameInstance*>(mComponents[KulaIndex].get())->GetPlayer();
				player.ApplyRotation(storedRotation, cachedDirection, cachedUp);

				if (mCamera != nullptr)
				{
					mCamera->ApplyRotation(storedRotation, cachedDirection, cachedUp);
				}

				auto directionVec = player.DirectionVector();
				auto upVec = player.UpVector();
				auto position = player.PositionVector();
				upVec *= _camOffsetUp;
				directionVec *= _camOffsetBack;
				mCamera->SetPosition(position + directionVec + upVec);

				isTurningDown = false;
				isTurning = false;
				acummulatedRotation = 0.0f;
			}
			else
			{
				rotationAmount.y += addedRotation;
			}
		}

		if (mKeyboard->WasKeyPressedThisFrame(Keys::D))
		{
			if (canTurnClockwise && !isTurning && !_isMoving)
			{
				FieaGameEngine::Player& player = static_cast<KulaGameInstance*>(mComponents[KulaIndex].get())->GetPlayer();
				storedRotation = XMMatrixRotationY(rotationAmount.x) * XMMatrixRotationAxis(player.UpVector(), -XM_PIDIV2);
				cachedDirection = player.Direction();
				cachedUp = player.Up();
				canTurnClockwise = false;
				isTurning = true;
				isTurningClockwise = true;
			}
		}

		if (mKeyboard->WasKeyPressedThisFrame(Keys::A))
		{
			if (canTurnCounterClockwise && !isTurning && !_isMoving)
			{
				FieaGameEngine::Player& player = static_cast<KulaGameInstance*>(mComponents[KulaIndex].get())->GetPlayer();
				storedRotation = XMMatrixRotationY(rotationAmount.x) * XMMatrixRotationAxis(player.UpVector(), XM_PIDIV2);
				cachedDirection = player.Direction();
				cachedUp = player.Up();
				canTurnCounterClockwise = false;
				isTurning = true;
				isTurningCounterClockwise = true;
			}
		}

		if (isTurningClockwise)
		{
			float addedRotation = BallRotationRate.x * elapsedTime;
			acummulatedRotation += addedRotation;
			if (XM_PIDIV2 - acummulatedRotation <= 0.f)
			{
				FieaGameEngine::Player& player = static_cast<KulaGameInstance*>(mComponents[KulaIndex].get())->GetPlayer();
				player.ApplyRotation(storedRotation, cachedDirection, cachedUp);

				if (mCamera != nullptr)
				{
					mCamera->ApplyRotation(storedRotation, cachedDirection, cachedUp);
				}

				auto directionVec = player.DirectionVector();
				auto upVec = player.UpVector();
				auto position = player.PositionVector();
				upVec *= _camOffsetUp;
				directionVec *= _camOffsetBack;
				mCamera->SetPosition(position + directionVec + upVec);

				player.TurnRight();

				canTurnClockwise = false;
				isTurningClockwise = false;
				isTurning = false;
				acummulatedRotation = 0.0f;
			}
			else
			{
				rotationAmount.z -= addedRotation;
			}
		}

		if (isTurningCounterClockwise)
		{
			float addedRotation = BallRotationRate.x * elapsedTime;
			acummulatedRotation += addedRotation;
			if (XM_PIDIV2 - acummulatedRotation <= 0.f)
			{
				FieaGameEngine::Player& player = static_cast<KulaGameInstance*>(mComponents[KulaIndex].get())->GetPlayer();
				player.ApplyRotation(storedRotation, cachedDirection, cachedUp);

				if (mCamera != nullptr)
				{
					mCamera->ApplyRotation(storedRotation, cachedDirection, cachedUp);
				}

				auto directionVec = player.DirectionVector();
				auto upVec = player.UpVector();
				auto position = player.PositionVector();
				upVec *= _camOffsetUp;
				directionVec *= _camOffsetBack;
				mCamera->SetPosition(position + directionVec + upVec);

				player.TurnLeft();

				canTurnCounterClockwise = false;
				isTurningCounterClockwise = false;
				isTurning = false;
				acummulatedRotation = 0.0f;
			}
			else
			{
				rotationAmount.z += addedRotation;
			}
		}

		const float BallMovementRate = 10.0f;
		if (isStartup)
		{
			FieaGameEngine::Player& player = static_cast<KulaGameInstance*>(mComponents[KulaIndex].get())->GetPlayer();
			XMMATRIX RotationMatrix = XMMatrixRotationY(rotationAmount.x) * XMMatrixRotationAxis(player.RightVector(), rotationAmount.y);
			player.ApplyRotation(RotationMatrix);
			mCamera->ApplyRotation(RotationMatrix, player.Direction(), player.Up());

			XMVECTOR movement = XMLoadFloat3(&movementAmount) * BallMovementRate * elapsedTime;

			auto directionVec = player.DirectionVector();
			auto upVec = player.UpVector();
			player.SetPosition(player.PositionVector() + movement);
			auto position = player.PositionVector();
			upVec *= _camOffsetUp;
			directionVec *= _camOffsetBack;

			if (mCamera != nullptr)
			{
				mCamera->SetPosition(position + directionVec + upVec);
			}
			isStartup = false;
		}

		if (_isMoving && !_isTurning)
		{
			FieaGameEngine::Player& player = static_cast<KulaGameInstance*>(mComponents[KulaIndex].get())->GetPlayer();

			if (_moveInterpolation < 1.f) {
				_moveInterpolation += (gameTime.ElapsedGameTimeSeconds().count() / _moveDurationSeconds);

				if (_moveInterpolation > 0.99f) {
					player.SetPosition(_midpoint);
					_moveInterpolation = 1.f;

					if (_currentMovementCase == ROLLING_UP) {
						SwivelDown();
					} else if (_currentMovementCase == ROLLING_AROUND) {
						SwivelUp();
					}
				} else {
					auto position = XMLoadFloat3(&_startPosition);
					auto destination = XMLoadFloat3(&_midpoint);
					player.SetPosition(position + (_moveInterpolation * (destination - position)));
				}
			} else {
				_moveInterpolation += (gameTime.ElapsedGameTimeSeconds().count() / _moveDurationSeconds);

				if (_moveInterpolation > 1.99f) {
					player.SetPosition(_targetPosition);
					_moveInterpolation = 2.f;
					_isMoving = false;
					_currentMovementCase = MOVEMENT_CASE_UNKNOWN;
					player.SetCoordinate(_movementArgs->GetNextCubePosition());
					player.SetDirection(_movementArgs->GetNextDirection().GetDirection());
					player.SetFace(_movementArgs->GetNextCubeFace().GetDirection());
					Event::Publish(std::move(_movementArgs));
				} else {
					auto position = XMLoadFloat3(&_midpoint);
					auto destination = XMLoadFloat3(&_targetPosition);
					player.SetPosition(position + ((_moveInterpolation - 1.f) * (destination - position)));
				}
			}
		}

		if (rotationAmount.x != 0.0f || rotationAmount.y != 0.0f || rotationAmount.z != 0.0f)
		{
			FieaGameEngine::Player& player = static_cast<KulaGameInstance*>(mComponents[KulaIndex].get())->GetPlayer();
			XMMATRIX RotationMatrix = XMMatrixRotationY(rotationAmount.x) * XMMatrixRotationAxis(player.RightVector(), rotationAmount.y) * XMMatrixRotationAxis(player.UpVector(), rotationAmount.z);
			player.ApplyRotation(RotationMatrix);
			mCamera->ApplyRotation(RotationMatrix);
		}

		FieaGameEngine::Player& player = static_cast<KulaGameInstance*>(mComponents[KulaIndex].get())->GetPlayer();

		auto directionVec = player.DirectionVector();
		auto upVec = player.UpVector();
		auto position = player.PositionVector();
		upVec *= _camOffsetUp;
		directionVec *= _camOffsetBack;

		if (mCamera!= nullptr)
		{
			mCamera->SetPosition(position + directionVec + upVec);
		}
	}

	std::pair<typename RenderingGame::MovementCase, FieaGameEngine::PlayerOntoCubeEventArgs> RenderingGame::ComputeMovementCase(const FieaGameEngine::Player& player) {
		auto& level = static_cast<KulaGameInstance*>(mComponents[KulaIndex].get())->GetLevel();

		const auto& previousPosition = player.GetCoordinate();
		const auto& previousDirection = player.GetDirection();
		const auto& previousFace = player.GetFace();

		auto* previousCube = level.GetCube(previousPosition);
		assert(previousCube);

		FieaGameEngine::IntVector3D nextPosition = previousPosition + previousDirection.ToIntVector3D() + previousFace.ToIntVector3D();
		auto* nextCube = level.GetCube(nextPosition);

		// Assume the cube directly in front of us exists.
		FieaGameEngine::Direction3DInfo nextDirection = previousFace; // Assuming the above, we're turning "up", so now we're facing in the same direction as our previous normal.
		FieaGameEngine::Direction3DInfo nextFace = previousDirection.Invert(); // Assuming the above, the face we're on is the opposite of the direction we used to be facing.
		MovementCase movementCase = ROLLING_UP;

		if (nextCube == nullptr) {
			// There is no cube in front of us. Is there a cube below where we were moving?
			nextPosition = nextPosition - previousFace.ToIntVector3D();
			nextCube = level.GetCube(nextPosition);

			if (nextCube == nullptr) {
				// There is neither a cube in front of us nor is there a cube below us. Therefore, we should turn on our current cube.
				nextCube = previousCube;
				nextPosition = previousPosition;
				nextDirection = previousFace.Invert(); // We're turning "down", so we're facing the opposite direction of our previous normal.
				nextFace = previousDirection; // Now we're on the face which has the same directional value as the direction we were facing.
				movementCase = ROLLING_AROUND;
			} else {
				// There IS a cube below us!
				nextDirection = previousDirection; // No change in direction, we're moving "forward".
				nextFace = previousFace; // No change in face, it's a different cube but we'll be on the same face on that different cube.
				movementCase = ROLLING_AHEAD;
			}
		}

		return std::make_pair(
			movementCase,
			FieaGameEngine::PlayerOntoCubeEventArgs{
				nextDirection,
				nextFace,
				nextPosition,
				previousDirection,
				previousFace,
				previousPosition
			}
		);
	}

	void RenderingGame::ExecuteMovementCase1()
	{
		FieaGameEngine::Player& player = static_cast<KulaGameInstance*>(mComponents[KulaIndex].get())->GetPlayer();

		switch (_movementState)
		{
		case 1:
			XMVECTOR newPos = player.PositionVector() + player.DirectionVector() * _moveSpeed;
			player.SetPosition(newPos);

			XMVECTOR distanceVec = XMLoadFloat3(&_targetPosition) - newPos;
			XMStoreFloat(&_distanceVec, XMVector3Length(distanceVec));
			if (_distanceVec < 0.1f)
			{
				player.SetPosition(_targetPosition);
				_movementState = 2;
			}

			break;
		case 2:
			_movementState = 3;

			break;

		case 3:


		default:
			break;
		}
	}
}