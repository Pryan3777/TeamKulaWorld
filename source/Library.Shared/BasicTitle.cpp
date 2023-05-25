#include "pch.h"
#include "BasicTitle.h"
#include "Game.h"

using namespace std;
using namespace std::literals;
using namespace DirectX;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(BasicTitle)

		BasicTitle::BasicTitle(Game& game, DirectX::XMFLOAT2 position, std::string message, bool TextFontFlag) :
		DrawableGameComponent(game), mTextPosition(position), TargetMessage(message), textFontflag(TextFontFlag)
	{
	}

	void BasicTitle::Initialize()
	{

		mSpriteBatch = make_unique<SpriteBatch>(mGame->Direct3DDeviceContext());
		if (textFontflag)
		{
			mSpriteFont = make_unique<SpriteFont>(mGame->Direct3DDevice(), L"Content\\Fonts\\RailwayMedium_70.spritefont");
		}
		else
		{
			mSpriteFont = make_unique<SpriteFont>(mGame->Direct3DDevice(), L"Content\\Fonts\\RailwayMedium_50.spritefont");
		}
		//mSpriteFont = make_unique<SpriteFont>(mGame->Direct3DDevice(), L"Content\\Fonts\\ScoreFont.spritefont");
		//mSpriteFont = make_unique<SpriteFont>(mGame->Direct3DDevice(), L"Content\\Fonts\\Raleway.spritefont");
	}

	void BasicTitle::Draw(const GameTime&)
	{
		mSpriteBatch->Begin();
		mSpriteFont->DrawString(mSpriteBatch.get(), TargetMessage.c_str(), mTextPosition);
		mSpriteBatch->End();
	}
}