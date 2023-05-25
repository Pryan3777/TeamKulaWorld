#pragma once
#include "DrawableGameComponent.h"
#include <DirectXMath.h>
#include <DirectXTK\SpriteBatch.h>
#include <chrono>
#include <cmath>

namespace FieaGameEngine
{
	class BasicTitle final : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(BasicTitle, DrawableGameComponent)

	public:
		BasicTitle(Game& game, DirectX::XMFLOAT2 position, std::string message, bool TextFontFlag);
		BasicTitle(const BasicTitle&) = delete;
		BasicTitle(BasicTitle&&) = delete;
		BasicTitle& operator=(const BasicTitle&) = delete;
		BasicTitle& operator=(BasicTitle&&) = delete;
		~BasicTitle() = default;

		virtual void Initialize() override;
		virtual void Draw(const GameTime& gameTime) override;
		std::string TargetMessage = "Kula World";

	private:
		std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
		std::unique_ptr<DirectX::SpriteFont> mSpriteFont;
		DirectX::XMFLOAT2 mTextPosition{ 400.0f, 20.0f };

		float StartTime = 70.0f;
		size_t CurrentScore = 0;
		bool textFontflag = false;
	};
}