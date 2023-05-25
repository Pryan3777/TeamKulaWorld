#include "pch.h"
#include "EndScreenComponent.h"
#include "Game.h"

using namespace std;
using namespace std::literals;
using namespace DirectX;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EndScreenComponent)

		EndScreenComponent::EndScreenComponent(Game& game) :
		DrawableGameComponent(game)
	{
	}
	
	void EndScreenComponent::Initialize()
	{
		mSpriteBatch = make_unique<SpriteBatch>(mGame->Direct3DDeviceContext());
		mSpriteFont = make_unique<SpriteFont>(mGame->Direct3DDevice(), L"Content\\Fonts\\ScoreFont.spritefont");
	}


	void EndScreenComponent::Draw([[maybe_unused]]const GameTime& gameTime)
	{
		mSpriteBatch->Begin();

		wostringstream scoreLabel;
		scoreLabel << setprecision(4) << L"Game Over! ";
		mSpriteFont->DrawString(mSpriteBatch.get(), scoreLabel.str().c_str(), mTextPosition);

		mSpriteBatch->End();
	}
}