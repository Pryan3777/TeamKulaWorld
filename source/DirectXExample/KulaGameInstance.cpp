#include "pch.h"
#include "KulaGameInstance.h"
#include "FirstPersonCamera.h"
#include "VertexDeclarations.h"
#include "RenderingGame.h"
#include "Game.h"
#include "GameException.h"
#include "Model.h"
#include "Mesh.h"
#include "Texture2D.h"
#include "CubeModel.h"
#include "TextureCube.h"
#include "Direction3D.h"
#include <ScopeParseWrapper.h>
#include <JsonParseCoordinator.h>
#include <ScopeJsonKeyTokenTransmuter.h>
#include <AllScopeJsonParseHelper.h>

using namespace std;
using namespace std::string_literals;
using namespace gsl;
using namespace FieaGameEngine;
using namespace DirectX;

namespace Rendering
{
	KulaGameInstance::KulaGameInstance(Game& game, const shared_ptr<Camera> camera, int32_t selectedLevel) :
		DrawableGameComponent(game, camera), levelSelected(selectedLevel)
	{
	}

	KulaGameInstance::~KulaGameInstance()
	{
	}

	void KulaGameInstance::Initialize()
	{
		//Testing Deserializing JSON data to produce a Level
		mLevel.reset();
		mLevel = std::make_shared<Level>();
		auto wrapper = std::make_shared<ScopeParseWrapper>(mLevel);
		auto coordinator = JsonParseCoordinator(wrapper);

		coordinator.PushBackHelper(std::make_unique<ScopeJsonParse::AllScopeJsonParseHelper>());
		coordinator.PushBackTransmuter(std::make_unique<ScopeJsonParse::ScopeJsonKeyTokenTransmuter>());

		switch (levelSelected)
		{
		case 1:
			CubeModel::SetTextureFile(L"Textures\\CubeTexture_1.png");
			coordinator.DeserializeIntoWrapperFromFile(R"(Files\Level1.json)"s);
			mSkybox = std::unique_ptr<Skybox>(new Skybox(*mGame, mCamera, L"Textures\\SKYboxHands.dds"s, 500.0f));
			break;
		case 2:
			CubeModel::SetTextureFile(L"Textures\\CubeTexture_3.png");
			coordinator.DeserializeIntoWrapperFromFile(R"(Files\Level2.json)"s);
			mSkybox = std::unique_ptr<Skybox>(new Skybox(*mGame, mCamera, L"Textures\\OBIWANPAULskybox.dds"s, 500.0f));
			break;
		case 3:
			CubeModel::SetTextureFile(L"Textures\\CubeTexture_2.png");
			coordinator.DeserializeIntoWrapperFromFile(R"(Files\Level3.json)"s);
			mSkybox = std::unique_ptr<Skybox>(new Skybox(*mGame, mCamera, L"Textures\\PaulInMountainds.dds"s, 500.0f));
			break;
		}

		Level* level = mLevel.get();
		level->Initialize(*mGame, mCamera, mDrawableObjects);

		auto updateMaterialFunc = [this]() { mUpdateMaterial = true; };
		mCamera->AddViewMatrixUpdatedCallback(updateMaterialFunc);
		mCamera->AddProjectionMatrixUpdatedCallback(updateMaterialFunc);

		mSkybox->Initialize();
		mGame->As<RenderingGame>()->PlayLevelMusic();
	}

	void KulaGameInstance::Draw(const GameTime& gameTime)
	{
		mSkybox->Draw(gameTime);

		for (auto& object : mDrawableObjects)
		{
			object->DrawableUpdate(gameTime);
		}

		sort(mDrawableObjects.begin(), mDrawableObjects.end(), CompareDistance(mCamera));

		for (auto& object : mDrawableObjects)
		{
			object->DrawableDraw(gameTime);
		}
	}

	FieaGameEngine::Player& KulaGameInstance::GetPlayer()
	{
		return mLevel->GetPlayer();
	}

	FieaGameEngine::Level& KulaGameInstance::GetLevel()
	{
		return *mLevel;
	}

	CompareDistance::CompareDistance(std::shared_ptr<Camera> camera) :
		mCamera { move(camera) }
	{
	}
	bool CompareDistance::operator()(BaseDrawableGameobject* a, BaseDrawableGameobject* b) const
	{
		auto aPos = a->GetPositionVector();
		auto bPos = b->GetPositionVector();
		auto camPos = mCamera->PositionVector();
		
		auto aDiff = XMVectorSubtract(camPos, aPos);
		auto aLength = XMVector3Length(aDiff);
		float aDistance{};
		XMStoreFloat(&aDistance, aLength);

		auto bDiff = XMVectorSubtract(camPos, bPos);
		auto bLength = XMVector3Length(bDiff);
		float bDistance{};
		XMStoreFloat(&bDistance, bLength);

		return aDistance > bDistance;
	}
}