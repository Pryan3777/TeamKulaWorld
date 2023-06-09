#include "pch.h"
#include "ProxyModel.h"
#include "Game.h"
#include "GameException.h"
#include "Utility.h"
#include "Camera.h"
#include "VertexDeclarations.h"
#include "RasterizerStates.h"
#include "Model.h"
#include "Mesh.h"
#include "Texture2D.h"
#include "TextureCube.h"
#include "GenericMaterial.h"
#include "FirstPersonCamera.h"

using namespace std;
using namespace gsl;
using namespace DirectX;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ProxyModel)

	ProxyModel::ProxyModel(Game& game, const shared_ptr<Camera>& camera, const std::string& modelFileName, float scale) :
		DrawableGameComponent(game, camera),
		mModelFileName(modelFileName), mScale(scale),
		mMaterial(*mGame)
	{
	}

	const XMFLOAT3& ProxyModel::Position() const
	{
		return mPosition;
	}

	const XMFLOAT3& ProxyModel::Direction() const
	{
		return mDirection;
	}

	const XMFLOAT3& ProxyModel::Up() const
	{
		return mUp;
	}

	const XMFLOAT3& ProxyModel::Right() const
	{
		return mRight;
	}

	XMVECTOR ProxyModel::PositionVector() const
	{
		return XMLoadFloat3(&mPosition);
	}

	XMVECTOR ProxyModel::DirectionVector() const
	{
		return XMLoadFloat3(&mDirection);
	}

	XMVECTOR ProxyModel::UpVector() const
	{
		return XMLoadFloat3(&mUp);
	}

	XMVECTOR ProxyModel::RightVector() const
	{
		return XMLoadFloat3(&mRight);
	}

	bool& ProxyModel::DisplayWireframe()
	{
		return mDisplayWireframe;
	}

	void ProxyModel::SetPosition(float x, float y, float z)
	{
		XMVECTOR position = XMVectorSet(x, y, z, 1.0f);
		SetPosition(position);
	}

	void ProxyModel::SetPosition(FXMVECTOR position)
	{
		XMStoreFloat3(&mPosition, position);
		mUpdateWorldMatrix = true;
	}

	void ProxyModel::SetPosition(const XMFLOAT3& position)
	{
		mPosition = position;
		mUpdateWorldMatrix = true;
	}

	void ProxyModel::ApplyRotation(CXMMATRIX transform)
	{
		XMVECTOR direction = XMLoadFloat3(&mDirection);
		XMVECTOR up = XMLoadFloat3(&mUp);

		direction = XMVector3TransformNormal(direction, transform);
		direction = XMVector3Normalize(direction);

		up = XMVector3TransformNormal(up, transform);
		up = XMVector3Normalize(up);

		XMVECTOR right = XMVector3Cross(direction, up);
		up = XMVector3Cross(right, direction);

		XMStoreFloat3(&mDirection, direction);
		XMStoreFloat3(&mUp, up);
		XMStoreFloat3(&mRight, right);
		
		mUpdateWorldMatrix = true;
	}

	void ProxyModel::ApplyRotation(DirectX::CXMMATRIX transform, DirectX::XMFLOAT3 direction, DirectX::XMFLOAT3 up)
	{
		XMVECTOR directionVec = XMLoadFloat3(&direction);
		XMVECTOR upVec = XMLoadFloat3(&up);

		directionVec = XMVector3TransformNormal(directionVec, transform);
		directionVec = XMVector3Normalize(directionVec);

		upVec = XMVector3TransformNormal(upVec, transform);
		upVec = XMVector3Normalize(upVec);

		XMVECTOR rightVec = XMVector3Cross(directionVec, upVec);
		upVec = XMVector3Cross(rightVec, directionVec);

		XMStoreFloat3(&mDirection, directionVec);
		XMStoreFloat3(&mUp, upVec);
		XMStoreFloat3(&mRight, rightVec);

		mUpdateWorldMatrix = true;
	}

	void ProxyModel::ApplyRotation(const XMFLOAT4X4& transform)
	{
		XMMATRIX transformMatrix = XMLoadFloat4x4(&transform);
		ApplyRotation(transformMatrix);
	}

	void ProxyModel::SetColor(const DirectX::XMFLOAT4& color)
	{
		mMaterial.SetSurfaceColor(color);
	}

	void ProxyModel::Initialize()
	{
		const auto model = mGame->Content().Load<Model>(Utility::ToWideString(mModelFileName));
		Mesh* mesh = model->Meshes().at(0).get();
		VertexPosition::CreateVertexBuffer(mGame->Direct3DDevice(), *mesh, not_null<ID3D11Buffer**>(mVertexBuffer.put()));
		mesh->CreateIndexBuffer(*mGame->Direct3DDevice(), not_null<ID3D11Buffer**>(mIndexBuffer.put()));
		mIndexCount = narrow<uint32_t>(mesh->Indices().size());

		//auto colorMap = mGame->Content().Load<Texture2D>(L"Textures\\diffuse.png"s);
		//auto environmentMap = mGame->Content().Load<TextureCube>(L"Textures\\DesertCubemap.dds"s);
		//mCustomMaterial = make_shared<Rendering::GenericMaterial>(*mGame, colorMap, environmentMap);
		//mCustomMaterial->Initialize();

		mMaterial.Initialize();
		mMaterial.SetSurfaceColor(Colors::Moccasin.f);

		auto updateMaterialFunc = [this]() { mUpdateWorldMatrix = true; };
		mCamera->AddViewMatrixUpdatedCallback(updateMaterialFunc);
		mCamera->AddProjectionMatrixUpdatedCallback(updateMaterialFunc);

		//mCamera->As<FieaGameEngine::FirstPersonCamera>()->AddPositionUpdatedCallback([this]() {
		//	mCustomMaterial->UpdateCameraPosition(mCamera->Position());
		//	});
	}

	void ProxyModel::Update(const GameTime&)
	{
		if (mUpdateWorldMatrix)
		{
			XMMATRIX worldMatrix = XMMatrixIdentity();
			MatrixHelper::SetForward(worldMatrix, mDirection);
			MatrixHelper::SetUp(worldMatrix, mUp);
			MatrixHelper::SetRight(worldMatrix, mRight);
			MatrixHelper::SetTranslation(worldMatrix, mPosition);

			XMStoreFloat4x4(&mWorldMatrix, XMMatrixScaling(mScale, mScale, mScale) * worldMatrix);
			mUpdateWorldMatrix = false;
			mUpdateMaterial = true;
		}
	}

	void ProxyModel::Draw(const GameTime&)
	{
		if (mUpdateMaterial)
		{
			const XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
			const XMMATRIX wvp = XMMatrixTranspose(worldMatrix * mCamera->ViewProjectionMatrix());
			mMaterial.UpdateTransform(wvp);
			//mCustomMaterial->UpdateTransforms(wvp, XMMatrixTranspose(worldMatrix));
			mUpdateMaterial = false;
		}

		if (mDisplayWireframe)
		{
			mGame->Direct3DDeviceContext()->RSSetState(RasterizerStates::Wireframe.get());
			mMaterial.DrawIndexed(not_null<ID3D11Buffer*>(mVertexBuffer.get()), not_null<ID3D11Buffer*>(mIndexBuffer.get()), mIndexCount);
			mGame->Direct3DDeviceContext()->RSSetState(nullptr);
		}
		else
		{
			mMaterial.DrawIndexed(not_null<ID3D11Buffer*>(mVertexBuffer.get()), not_null<ID3D11Buffer*>(mIndexBuffer.get()), mIndexCount);
			//mCustomMaterial->DrawIndexed(not_null<ID3D11Buffer*>(mVertexBuffer.get()), not_null<ID3D11Buffer*>(mIndexBuffer.get()), mIndexCount);
		}
	}
}