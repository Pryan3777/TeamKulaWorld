#pragma once

#include <DirectXColors.h>
#include "Material.h"
#include "VectorHelper.h"
#include "MatrixHelper.h"
#include "SamplerStates.h"

namespace FieaGameEngine
{
	class Texture2D;
	class TextureCube;
}

namespace Rendering
{
	class EnvironmentMappingMaterial : public FieaGameEngine::Material
	{
		RTTI_DECLARATIONS(EnvironmentMappingMaterial, FieaGameEngine::Material)

	public:
		EnvironmentMappingMaterial(FieaGameEngine::Game& game, std::shared_ptr<FieaGameEngine::Texture2D> colormap, std::shared_ptr<FieaGameEngine::TextureCube> environmentMap);
		EnvironmentMappingMaterial(const EnvironmentMappingMaterial&) = default;
		EnvironmentMappingMaterial& operator=(const EnvironmentMappingMaterial&) = default;
		EnvironmentMappingMaterial(EnvironmentMappingMaterial&&) = default;
		EnvironmentMappingMaterial& operator=(EnvironmentMappingMaterial&&) = default;
		virtual ~EnvironmentMappingMaterial() = default;

		winrt::com_ptr<ID3D11SamplerState> SamplerState() const;
		void SetSamplerState(winrt::com_ptr<ID3D11SamplerState> samplerState);

		std::shared_ptr<FieaGameEngine::Texture2D> ColorMap() const;
		void SetColorMap(std::shared_ptr<FieaGameEngine::Texture2D> texture);

		std::shared_ptr<FieaGameEngine::TextureCube> EnvironmentMap() const;
		void SetEnvironmentMap(std::shared_ptr<FieaGameEngine::TextureCube> texture);

		const DirectX::XMFLOAT4& AmbientColor() const;
		void SetAmbientColor(const DirectX::XMFLOAT4& color);

		const DirectX::XMFLOAT4& EnvironmentColor() const;
		void SetEnvironmentColor(const DirectX::XMFLOAT4& color);

		const float ReflectionAmount() const;
		void SetReflectionAmount(const float amount);

		virtual std::uint32_t VertexSize() const override;
		virtual void Initialize() override;

		void UpdateCameraPosition(const DirectX::XMFLOAT3& position);
		void UpdateTransforms(DirectX::FXMMATRIX worldViewProjectionMatrix, DirectX::CXMMATRIX worldMatrix);
		
	private:
		struct VertexCBufferPerFrame
		{
			DirectX::XMFLOAT3 CameraPosition{ FieaGameEngine::Vector3Helper::Zero };
			float Padding;
		};

		struct VertexCBufferPerObject
		{
			DirectX::XMFLOAT4X4 WorldViewProjection{ FieaGameEngine::MatrixHelper::Identity };
			DirectX::XMFLOAT4X4 World{ FieaGameEngine::MatrixHelper::Identity };
		};

		struct PixelCBufferPerFrame
		{
			DirectX::XMFLOAT4 AmbientColor{ DirectX::Colors::White };
			DirectX::XMFLOAT4 EnvironmentColor{ DirectX::Colors::White };
		};

		struct PixelCBufferPerObject
		{
			float ReflectionAmount{ 0.9f };
			float Padding[3];
		};

		virtual void BeginDraw() override;

		void ResetPixelShaderResources();

		winrt::com_ptr<ID3D11Buffer> mVertexCBufferPerFrame;
		winrt::com_ptr<ID3D11Buffer> mVertexCBufferPerObject;
		winrt::com_ptr<ID3D11Buffer> mPixelCBufferPerFrame;
		winrt::com_ptr<ID3D11Buffer> mPixelCBufferPerObject;
		VertexCBufferPerFrame mVertexCBufferPerFrameData;
		VertexCBufferPerObject mVertexCBufferPerObjectData;
		PixelCBufferPerFrame mPixelCBufferPerFrameData;
		PixelCBufferPerObject mPixelCBufferPerObjectData;		
		std::shared_ptr<FieaGameEngine::Texture2D> mColorMap;
		std::shared_ptr<FieaGameEngine::TextureCube> mEnvironmentMap;
		winrt::com_ptr<ID3D11SamplerState> mSamplerState{ FieaGameEngine::SamplerStates::TrilinearClamp };
		bool mPixelCBufferPerFrameDataDirty{ true };
		bool mPixelCBufferPerObjectDataDirty{ true };
	};
}