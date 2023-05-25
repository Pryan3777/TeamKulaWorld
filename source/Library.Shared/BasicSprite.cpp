#include "pch.h"
#include "BasicSprite.h"
#include "Game.h"
#include "GameException.h"

using namespace std;
using namespace std::literals;
using namespace DirectX;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(BasicSprite)

		BasicSprite::BasicSprite(Game& game, DirectX::XMFLOAT2 position, const wchar_t* filename) :
		DrawableGameComponent(game), spritePosition(position), assetPath(filename)
	{
	}

	void BasicSprite::Initialize()
	{

		mSpriteBatch = make_unique<SpriteBatch>(mGame->Direct3DDeviceContext());
		ThrowIfFailed(CreateWICTextureFromFile(mGame->Direct3DDevice(), assetPath, nullptr, mShaderResourceView.put()), "CreateWICTextureFromFile() failed.");
	}

	void BasicSprite::Draw(const GameTime&)
	{

		mSpriteBatch->Begin();
		mSpriteBatch->Draw(mShaderResourceView.get(), spritePosition, nullptr, Colors::White, 0.f);
		mSpriteBatch->End();
	}
}
