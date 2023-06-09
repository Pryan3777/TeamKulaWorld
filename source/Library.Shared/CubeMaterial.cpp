#include "pch.h"
#include "CubeMaterial.h"
#include "Game.h"
#include "GameException.h"
#include "VertexDeclarations.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture2D.h"
#include "TextureCube.h"

using namespace std;
using namespace std::string_literals;
using namespace gsl;
using namespace winrt;
using namespace DirectX;
using namespace FieaGameEngine;

namespace Rendering
{
	RTTI_DEFINITIONS(CubeMaterial)

		CubeMaterial::CubeMaterial(Game& game, shared_ptr<Texture2D> colorMap, shared_ptr<TextureCube> environmentMap) :
		Material(game), mColorMap(move(colorMap)), mEnvironmentMap(move(environmentMap))
	{
	}

	com_ptr<ID3D11SamplerState> CubeMaterial::SamplerState() const
	{
		return mSamplerState;
	}

	void CubeMaterial::SetSamplerState(com_ptr<ID3D11SamplerState> samplerState)
	{
		assert(samplerState != nullptr);
		mSamplerState = samplerState;
		Material::SetSamplerState(ShaderStages::PS, mSamplerState.get());
	}

	shared_ptr<Texture2D> CubeMaterial::ColorMap() const
	{
		return mColorMap;
	}

	void CubeMaterial::SetColorMap(shared_ptr<Texture2D> texture)
	{
		assert(texture != nullptr);
		mColorMap = move(texture);
		ResetPixelShaderResources();
	}

	shared_ptr<TextureCube> CubeMaterial::EnvironmentMap() const
	{
		return mEnvironmentMap;
	}

	void CubeMaterial::SetEnvironmentMap(shared_ptr<TextureCube> texture)
	{
		assert(texture != nullptr);
		mEnvironmentMap = move(texture);
		ResetPixelShaderResources();
	}

	const XMFLOAT4& CubeMaterial::AmbientColor() const
	{
		return mPixelCBufferPerFrameData.AmbientColor;
	}

	void CubeMaterial::SetAmbientColor(const XMFLOAT4& color)
	{
		mPixelCBufferPerFrameData.AmbientColor = color;
		mPixelCBufferPerFrameDataDirty = true;
	}

	const XMFLOAT4& CubeMaterial::EnvironmentColor() const
	{
		return mPixelCBufferPerFrameData.EnvironmentColor;
	}

	void CubeMaterial::SetEnvironmentColor(const XMFLOAT4& color)
	{
		mPixelCBufferPerFrameData.EnvironmentColor = color;
		mPixelCBufferPerFrameDataDirty = true;
	}

	const float CubeMaterial::ReflectionAmount() const
	{
		return mPixelCBufferPerObjectData.ReflectionAmount;
	}

	void CubeMaterial::SetReflectionAmount(float amount)
	{
		mPixelCBufferPerObjectData.ReflectionAmount = amount;
		mGame->Direct3DDeviceContext()->UpdateSubresource(mPixelCBufferPerObject.get(), 0, nullptr, &mPixelCBufferPerObjectData, 0, 0);
	}

	uint32_t CubeMaterial::VertexSize() const
	{
		return sizeof(VertexPositionTextureNormal);
	}

	void CubeMaterial::Initialize()
	{
		Material::Initialize();

		auto& content = mGame->Content();
		auto vertexShader = content.Load<VertexShader>(L"Shaders\\GenericMaterialVS.cso"s);
		SetShader(vertexShader);

		auto pixelShader = content.Load<PixelShader>(L"Shaders\\GenericMaterialPS.cso");
		SetShader(pixelShader);

		auto direct3DDevice = mGame->Direct3DDevice();
		vertexShader->CreateInputLayout<VertexPositionTextureNormal>(direct3DDevice);
		SetInputLayout(vertexShader->InputLayout());

		D3D11_BUFFER_DESC constantBufferDesc{ 0 };
		constantBufferDesc.ByteWidth = sizeof(VertexCBufferPerFrame);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mVertexCBufferPerFrame.put()), "ID3D11Device::CreateBuffer() failed.");
		AddConstantBuffer(ShaderStages::VS, mVertexCBufferPerFrame.get());

		constantBufferDesc.ByteWidth = sizeof(VertexCBufferPerObject);
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mVertexCBufferPerObject.put()), "ID3D11Device::CreateBuffer() failed.");
		AddConstantBuffer(ShaderStages::VS, mVertexCBufferPerObject.get());

		constantBufferDesc.ByteWidth = sizeof(PixelCBufferPerFrame);
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mPixelCBufferPerFrame.put()), "ID3D11Device::CreateBuffer() failed.");
		AddConstantBuffer(ShaderStages::PS, mPixelCBufferPerFrame.get());

		constantBufferDesc.ByteWidth = sizeof(PixelCBufferPerObject);
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mPixelCBufferPerObject.put()), "ID3D11Device::CreateBuffer() failed.");
		AddConstantBuffer(ShaderStages::PS, mPixelCBufferPerObject.get());

		auto direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->UpdateSubresource(mVertexCBufferPerFrame.get(), 0, nullptr, &mVertexCBufferPerFrameData, 0, 0);
		direct3DDeviceContext->UpdateSubresource(mVertexCBufferPerObject.get(), 0, nullptr, &mVertexCBufferPerObjectData, 0, 0);
		direct3DDeviceContext->UpdateSubresource(mPixelCBufferPerFrame.get(), 0, nullptr, &mPixelCBufferPerFrameData, 0, 0);
		direct3DDeviceContext->UpdateSubresource(mPixelCBufferPerObject.get(), 0, nullptr, &mPixelCBufferPerObjectData, 0, 0);

		ResetPixelShaderResources();
		AddSamplerState(ShaderStages::PS, mSamplerState.get());
	}

	void CubeMaterial::UpdateCameraPosition(const DirectX::XMFLOAT3& position)
	{
		mVertexCBufferPerFrameData.CameraPosition = position;
		mGame->Direct3DDeviceContext()->UpdateSubresource(mVertexCBufferPerFrame.get(), 0, nullptr, &mVertexCBufferPerFrameData, 0, 0);
	}

	void CubeMaterial::UpdateTransforms(FXMMATRIX worldViewProjectionMatrix, CXMMATRIX worldMatrix)
	{
		XMStoreFloat4x4(&mVertexCBufferPerObjectData.WorldViewProjection, worldViewProjectionMatrix);
		XMStoreFloat4x4(&mVertexCBufferPerObjectData.World, worldMatrix);
		mGame->Direct3DDeviceContext()->UpdateSubresource(mVertexCBufferPerObject.get(), 0, nullptr, &mVertexCBufferPerObjectData, 0, 0);
	}

	void CubeMaterial::BeginDraw()
	{
		Material::BeginDraw();

		if (mPixelCBufferPerFrameDataDirty)
		{
			mGame->Direct3DDeviceContext()->UpdateSubresource(mPixelCBufferPerFrame.get(), 0, nullptr, &mPixelCBufferPerFrameData, 0, 0);
			mPixelCBufferPerFrameDataDirty = false;
		}
	}

	void CubeMaterial::ResetPixelShaderResources()
	{
		Material::ClearShaderResources(ShaderStages::PS);
		ID3D11ShaderResourceView* shaderResources[] = { mColorMap->ShaderResourceView().get(), mEnvironmentMap->ShaderResourceView().get() };
		Material::AddShaderResources(ShaderStages::PS, shaderResources);
	}
}