#pragma once

#include <gsl\gsl>
#include <winrt\Windows.Foundation.h>
#include <d3d11.h>
#include "DrawableGameComponent.h"
#include "MatrixHelper.h"
#include "PointLight.h"
#include "Skybox.h"
#include "GenericMaterial.h"
#include "VectorHelper.h"
#include "MatrixHelper.h"
#include "BaseDrawableGameobject.h"
#include "GameTime.h"

namespace Rendering
{
	class GenericMaterial;

	class BallModel final : public FieaGameEngine::DrawableGameComponent, public FieaGameEngine::BaseDrawableGameobject 
	{
	public:
		BallModel(FieaGameEngine::Game& game, const std::shared_ptr<FieaGameEngine::Camera>& camera);
		BallModel(FieaGameEngine::Game& game, const std::shared_ptr<FieaGameEngine::Camera>& camera, float scale);
		BallModel(const BallModel&) = delete;
		BallModel(BallModel&&) = default;
		BallModel& operator=(const BallModel&) = default;
		BallModel& operator=(BallModel&&) = default;
		~BallModel();

		float AmbientLightIntensity() const;
		void SetAmbientLightIntensity(float intensity);

		float EnvironmentIntensity() const;
		void SetEnvironmentIntensity(float intensity);

		float ReflectionAmount() const;
		void SetReflectionAmount(float amount);

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
		void SetTransform(DirectX::XMFLOAT3 targetDirection, DirectX::XMFLOAT3 targetUp );

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
		std::shared_ptr<GenericMaterial> mMaterial;
		std::unique_ptr<FieaGameEngine::Skybox> mSkybox;
		winrt::com_ptr<ID3D11Buffer> mVertexBuffer;
		winrt::com_ptr<ID3D11Buffer> mIndexBuffer;
		std::uint32_t mIndexCount{ 0 };
		float mScale{ 2.f };
		bool mUpdateMaterial{ true };
		bool mUpdateWorldMatrix{ true };
	};
}