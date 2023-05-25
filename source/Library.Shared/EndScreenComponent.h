#pragma once
#include "DrawableGameComponent.h"
#include <DirectXMath.h>
#include <DirectXTK\SpriteBatch.h>
#include <chrono>
#include <cmath>

namespace FieaGameEngine
{
	class EndScreenComponent final : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(EndScreenComponent, DrawableGameComponent)

	public:
		EndScreenComponent(Game& game);//, Datum& score);
		EndScreenComponent(const EndScreenComponent&) = delete;
		EndScreenComponent(EndScreenComponent&&) = delete;
		EndScreenComponent& operator=(const EndScreenComponent&) = delete;
		EndScreenComponent& operator=(EndScreenComponent&&) = delete;
		~EndScreenComponent() = default;

		//	DirectX::XMFLOAT2& TextPosition();

		virtual void Initialize() override;
		//virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
		std::unique_ptr<DirectX::SpriteFont> mSpriteFont;
		DirectX::XMFLOAT2 mTextPosition{ 350.0f, 400.0f };

	};
}