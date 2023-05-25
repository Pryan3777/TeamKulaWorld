#pragma once

#include <gsl\gsl>
#include <winrt\Windows.Foundation.h>
#include <d3d11.h>
#include "DrawableGameComponent.h"
#include "MatrixHelper.h"
#include "PointLight.h"
#include "Skybox.h"
#include "NormalMappingMaterial.h"
#include "VectorHelper.h"
#include "MatrixHelper.h"
#include "BaseDrawableGameobject.h"
#include "GameTime.h"

namespace Rendering
{
	class NormalMappingMaterial;

	class CoinModel final : public FieaGameEngine::DrawableGameComponent, public FieaGameEngine::BaseDrawableGameobject
	{
	public:
		CoinModel(FieaGameEngine::Game& game, const std::shared_ptr<FieaGameEngine::Camera>& camera);
		CoinModel(FieaGameEngine::Game& game, const std::shared_ptr<FieaGameEngine::Camera>& camera, float scale);
		CoinModel(const CoinModel&) = delete;
		CoinModel(CoinModel&&) = default;
		CoinModel& operator=(const CoinModel&) = default;
		CoinModel& operator=(CoinModel&&) = default;
		~CoinModel();

		const DirectX::XMFLOAT3& Position() const;
		const DirectX::XMFLOAT3& Direction() const;
		const DirectX::XMFLOAT3& Up() const;
		const DirectX::XMFLOAT3& Right() const;

		DirectX::XMVECTOR PositionVector() const;
		DirectX::XMVECTOR DirectionVector() const;
		DirectX::XMVECTOR UpVector() const;
		DirectX::XMVECTOR RightVector() const;
		DirectX::XMVECTOR GetPositionVector() override;
		void SetPosition(float x, float y, float z);
		void SetPosition(DirectX::FXMVECTOR position);
		void SetPosition(const DirectX::XMFLOAT3& position);

		void ApplyRotation(DirectX::CXMMATRIX transform);
		void ApplyRotation(DirectX::CXMMATRIX transform, DirectX::XMFLOAT3 direction, DirectX::XMFLOAT3 up);
		void ApplyRotation(const DirectX::XMFLOAT4X4& transform);
		virtual void DrawableUpdate(const FieaGameEngine::GameTime&) override;
		virtual void DrawableDraw(const FieaGameEngine::GameTime&) override;
		virtual void Initialize() override;
		virtual void Draw(const FieaGameEngine::GameTime& gameTime) override;
		DirectX::XMFLOAT4X4 mWorldMatrix{ FieaGameEngine::MatrixHelper::Identity };

	private:
		inline static const float RotationRate{ DirectX::XM_PI };

		DirectX::XMFLOAT3 mPosition{ FieaGameEngine::Vector3Helper::Zero };
		DirectX::XMFLOAT3 mDirection{ FieaGameEngine::Vector3Helper::Forward };
		DirectX::XMFLOAT3 mUp{ FieaGameEngine::Vector3Helper::Up };
		DirectX::XMFLOAT3 mRight{ FieaGameEngine::Vector3Helper::Right };
		std::shared_ptr<NormalMappingMaterial> mMaterial;
		winrt::com_ptr<ID3D11Buffer> mVertexBuffer;
		winrt::com_ptr<ID3D11Buffer> mIndexBuffer;
		std::uint32_t mIndexCount{ 0 };
		float mScale{ 2.f };
		bool mUpdateMaterial{ true };
		bool mUpdateWorldMatrix{ true };
	};
}
