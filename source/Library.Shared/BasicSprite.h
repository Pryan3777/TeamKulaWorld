#pragma once
#include "DrawableGameComponent.h"
#include <DirectXMath.h>
#include <DirectXTK\SpriteBatch.h>
#include <chrono>
#include <cmath>

namespace FieaGameEngine
{
	class BasicSprite final : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(BasicSprite, DrawableGameComponent)

	public:
		BasicSprite(Game& game, DirectX::XMFLOAT2 position, const wchar_t* filename);
		BasicSprite(const BasicSprite&) = delete;
		BasicSprite(BasicSprite&&) = delete;
		BasicSprite& operator=(const BasicSprite&) = delete;
		BasicSprite& operator=(BasicSprite&&) = delete;
		~BasicSprite() = default;

		virtual void Initialize() override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
		DirectX::XMFLOAT2 spritePosition{ 400.0f, 20.0f };
		winrt::com_ptr<ID3D11ShaderResourceView> mShaderResourceView;
		const wchar_t* assetPath;
	};
}
