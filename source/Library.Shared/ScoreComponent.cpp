#include "pch.h"
#include "ScoreComponent.h"
#include "Game.h"
#include "GameplayState.h"

using namespace std;
using namespace std::literals;
using namespace DirectX;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ScoreComponent)

	ScoreComponent::ScoreComponent(Game& game):
	DrawableGameComponent(game)
	{
	}

	void ScoreComponent::Initialize()
	{
		mSpriteBatch = make_unique<SpriteBatch>(mGame->Direct3DDeviceContext());
		mSpriteFont = make_unique<SpriteFont>(mGame->Direct3DDevice(), L"Content\\Fonts\\ScoreFont.spritefont");
		IsTimeOver = false;
		CurrentScore = 0;
		Reset();
	}

	void ScoreComponent::IncrementScore(const size_t amount)
	{
		CurrentScore+=amount;
	}

	void ScoreComponent::SetScore(const size_t newScore)
	{
		CurrentScore = newScore;
	}

	void ScoreComponent::Reset()
	{
		CurrentScore = 0;
		NewTimeLimit = StartTime;
		IsTimeOver = false;
	}


	void ScoreComponent::Draw(const GameTime&)
	{
		auto remainingTime = NewTimeLimit - GameplayState::Singleton().GetTime().TotalGameTimeSeconds().count();
		if (remainingTime <= 0)
		{
			IsTimeOver = true;

		}
		mSpriteBatch->Begin();

		wostringstream scoreLabel;
		scoreLabel << setprecision(4)  << L"Score: " << CurrentScore << L"\nTime: " << static_cast<size_t>(ceil(remainingTime));

		mSpriteFont->DrawString(mSpriteBatch.get(), scoreLabel.str().c_str(), mTextPosition);

		mSpriteBatch->End();
	}
}