#include "pch.h"
#include "CoinModel.h"
#include "FirstPersonCamera.h"
#include "VertexDeclarations.h"
#include "Game.h"
#include "GameException.h"
#include "Model.h"
#include "Mesh.h"
#include "NormalMappingMaterial.h"
#include "Texture2D.h"
#include "TextureCube.h"

using namespace std;
using namespace std::string_literals;
using namespace gsl;
using namespace FieaGameEngine;
using namespace DirectX;

namespace Rendering
{
	CoinModel::CoinModel(Game& game, const shared_ptr<Camera>& camera) :
		DrawableGameComponent(game, camera)
	{
	}

	CoinModel::CoinModel(FieaGameEngine::Game& game, const std::shared_ptr<FieaGameEngine::Camera>& camera, float scale) :
		DrawableGameComponent(game, camera), mScale{ scale }
	{
	}

	CoinModel::~CoinModel()
	{
	}

	const DirectX::XMFLOAT3& CoinModel::Position() const
	{
		return mPosition;
	}

	const DirectX::XMFLOAT3& CoinModel::Direction() const
	{
		return mDirection;
	}

	const DirectX::XMFLOAT3& CoinModel::Up() const
	{
		return mUp;
	}

	const DirectX::XMFLOAT3& CoinModel::Right() const
	{
		return mRight;
	}

	DirectX::XMVECTOR CoinModel::PositionVector() const
	{
		return XMLoadFloat3(&mPosition);
	}

	DirectX::XMVECTOR CoinModel::DirectionVector() const
	{
		return XMLoadFloat3(&mDirection);
	}

	DirectX::XMVECTOR CoinModel::UpVector() const
	{
		return XMLoadFloat3(&mUp);
	}

	DirectX::XMVECTOR CoinModel::RightVector() const
	{
		return XMLoadFloat3(&mRight);
	}

	DirectX::XMVECTOR CoinModel::GetPositionVector()
	{
		return PositionVector();
	}

	void CoinModel::SetPosition(float x, float y, float z)
	{
		XMVECTOR position = XMVectorSet(x, y, z, 1.0f);
		SetPosition(position);
	}

	void CoinModel::SetPosition(DirectX::FXMVECTOR position)
	{
		XMStoreFloat3(&mPosition, position);
		mUpdateWorldMatrix = true;
	}

	void CoinModel::SetPosition(const DirectX::XMFLOAT3& position)
	{
		mPosition = position;
		mUpdateWorldMatrix = true;
	}

	void CoinModel::ApplyRotation(DirectX::CXMMATRIX transform, DirectX::XMFLOAT3 direction, DirectX::XMFLOAT3 up)
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

	void CoinModel::ApplyRotation(DirectX::CXMMATRIX transform)
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

	void CoinModel::ApplyRotation(const DirectX::XMFLOAT4X4& transform)
	{
		DirectX::XMMATRIX transformMatrix = XMLoadFloat4x4(&transform);
		ApplyRotation(transformMatrix);
	}

	void CoinModel::DrawableUpdate(const GameTime& gameTime)
	{
		Update(gameTime);
	}

	void CoinModel::DrawableDraw(const GameTime& gameTime)
	{
		Draw(gameTime);
	}

	void CoinModel::Initialize()
	{
		auto direct3DDevice = mGame->Direct3DDevice();
		const auto model = mGame->Content().Load<Model>(L"Models\\Coin.model"s);
		Mesh* mesh = model->Meshes().at(0).get();
		VertexPositionTextureNormal::CreateVertexBuffer(direct3DDevice, *mesh, not_null<ID3D11Buffer**>(mVertexBuffer.put()));
		mesh->CreateIndexBuffer(*direct3DDevice, not_null<ID3D11Buffer**>(mIndexBuffer.put()));
		mIndexCount = narrow<uint32_t>(mesh->Indices().size());


		auto colorMap = mGame->Content().Load<Texture2D>(L"Textures\\Coin_Base.png"s);
		auto normalMap = mGame->Content().Load<Texture2D>(L"Textures\\Coin_Normal.png"s);
		mMaterial = make_shared<NormalMappingMaterial>(*mGame, colorMap, normalMap);
		mMaterial->Initialize();

		auto updateMaterialFunc = [this]() { mUpdateMaterial = true; };
		mCamera->AddViewMatrixUpdatedCallback(updateMaterialFunc);
		mCamera->AddProjectionMatrixUpdatedCallback(updateMaterialFunc);
	}

	void CoinModel::Draw(const GameTime&)
	{

		if (mUpdateWorldMatrix)
		{
			DirectX::XMMATRIX worldMatrix = XMMatrixIdentity();
			MatrixHelper::SetForward(worldMatrix, mDirection);
			MatrixHelper::SetUp(worldMatrix, mUp);
			MatrixHelper::SetRight(worldMatrix, mRight);
			MatrixHelper::SetTranslation(worldMatrix, mPosition);

			XMStoreFloat4x4(&mWorldMatrix, XMMatrixScaling(mScale, mScale, mScale) * worldMatrix);
			mUpdateWorldMatrix = false;
			mUpdateMaterial = true;
		}

		if (mUpdateMaterial)
		{
			const DirectX::XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
			const DirectX::XMMATRIX wvp = XMMatrixTranspose(worldMatrix * mCamera->ViewProjectionMatrix());
			mMaterial->UpdateTransforms(wvp, XMMatrixTranspose(worldMatrix));
			mUpdateMaterial = false;
		}

		mMaterial->DrawIndexed(not_null<ID3D11Buffer*>(mVertexBuffer.get()), not_null<ID3D11Buffer*>(mIndexBuffer.get()), mIndexCount);
	}
}