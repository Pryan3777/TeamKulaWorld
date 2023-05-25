#pragma once

#include <gsl\gsl>
#include <winrt\Windows.Foundation.h>
#include <d3d11.h>
#include "DrawableGameComponent.h"
#include "MatrixHelper.h"
#include "PointLight.h"
#include "ProxyModel.h"
#include "Skybox.h"
#include "Vector.h"
#include "Camera.h"
#include "Cube.h"
#include "Level.h"
#include "Player.h"
#include "BaseDrawableGameobject.h"


namespace Rendering
{
	class EnvironmentMappingMaterial;
	struct CompareDistance
	{
		std::shared_ptr<FieaGameEngine::Camera> mCamera;
		CompareDistance(std::shared_ptr<FieaGameEngine::Camera> camera);
		CompareDistance(const CompareDistance&) = default;
		CompareDistance& operator=(const CompareDistance&) = default;
		CompareDistance(CompareDistance&&) noexcept = default;
		CompareDistance& operator=(CompareDistance&&) noexcept = default;
		~CompareDistance() = default;
		bool operator()(FieaGameEngine::BaseDrawableGameobject* a, FieaGameEngine::BaseDrawableGameobject* b) const;
	};

	class KulaGameInstance final : public FieaGameEngine::DrawableGameComponent
	{
	public:
		KulaGameInstance(FieaGameEngine::Game& game, const std::shared_ptr<FieaGameEngine::Camera> camera, int32_t selectedLevel = 1);
		KulaGameInstance(const KulaGameInstance&) = delete;
		KulaGameInstance(KulaGameInstance&&) = default;
		KulaGameInstance& operator=(const KulaGameInstance&) = default;		
		KulaGameInstance& operator=(KulaGameInstance&&) = default;
		~KulaGameInstance();
		virtual void Initialize() override;
		virtual void Draw(const FieaGameEngine::GameTime& gameTime) override;
		FieaGameEngine::Player& GetPlayer();
		FieaGameEngine::Level& GetLevel();

		DirectX::XMFLOAT4X4 mWorldMatrix{ FieaGameEngine::MatrixHelper::Identity };
		FieaGameEngine::Vector<FieaGameEngine::BaseDrawableGameobject*> mDrawableObjects{};
	private:
		std::shared_ptr<FieaGameEngine::Level> mLevel;
		inline static const float RotationRate{ DirectX::XM_PI };

		std::shared_ptr<EnvironmentMappingMaterial> mMaterial;
		std::unique_ptr<FieaGameEngine::Skybox> mSkybox;
		
		winrt::com_ptr<ID3D11Buffer> mVertexBuffer;
		winrt::com_ptr<ID3D11Buffer> mIndexBuffer;
		std::uint32_t mIndexCount{ 0 };
		bool mUpdateMaterial{ true };

		int32_t levelSelected{ 1 };
	};
}