#pragma once
#include "DrawableGameComponent.h"
#include <DirectXMath.h>
#include <DirectXTK\SpriteBatch.h>
#include <chrono>
#include <cmath>

namespace FieaGameEngine
{
	class ScoreComponent final : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(ScoreComponent, DrawableGameComponent)

	public:
		ScoreComponent(Game& game);
		ScoreComponent(const ScoreComponent&) = delete;
		ScoreComponent(ScoreComponent&&) = delete;
		ScoreComponent& operator=(const ScoreComponent&) = delete;
		ScoreComponent& operator=(ScoreComponent&&) = delete;
		~ScoreComponent() = default;

		virtual void Initialize() override;
		virtual void Draw(const GameTime& gameTime) override;
		
		void IncrementScore(const size_t amount);
		void SetScore(const size_t newScore);

		void Reset();
		bool IsTimeOver{ false };
	private:
		std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
		std::unique_ptr<DirectX::SpriteFont> mSpriteFont;
		DirectX::XMFLOAT2 mTextPosition{ 400.0f, 20.0f };

		float StartTime = 30.f; 
		float NewTimeLimit = 30.f; 
		size_t CurrentScore = 0;
	};
}