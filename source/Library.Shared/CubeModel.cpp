#include "pch.h"
#include "CubeModel.h"
#include "FirstPersonCamera.h"
#include "VertexDeclarations.h"
#include "Game.h"
#include "GameException.h"
#include "Model.h"
#include "Mesh.h"
#include "GenericMaterial.h"
#include "Texture2D.h"
#include "TextureCube.h"

using namespace std;
using namespace std::string_literals;
using namespace gsl;
using namespace FieaGameEngine;
using namespace DirectX;

namespace Rendering
{
	CubeModel::CubeModel(Game& game, const shared_ptr<Camera>& camera) :
		DrawableGameComponent(game, camera)
	{
	}

	CubeModel::CubeModel(FieaGameEngine::Game& game, const std::shared_ptr<FieaGameEngine::Camera>& camera, float scale) :
		DrawableGameComponent(game, camera), mScale{ scale }
	{
	}

	CubeModel::~CubeModel()
	{
	}

	float CubeModel::AmbientLightIntensity() const
	{
		return mMaterial->AmbientColor().x;
	}

	void CubeModel::SetAmbientLightIntensity(float intensity)
	{
		mMaterial->SetAmbientColor(XMFLOAT4(intensity, intensity, intensity, 1.0f));
	}

	float CubeModel::EnvironmentIntensity() const
	{
		return mMaterial->EnvironmentColor().x;
	}

	void CubeModel::SetEnvironmentIntensity(float intensity)
	{
		mMaterial->SetEnvironmentColor(XMFLOAT4(intensity, intensity, intensity, 1.0f));
	}

	float CubeModel::ReflectionAmount() const
	{
		return mMaterial->ReflectionAmount();
	}

	void CubeModel::SetReflectionAmount(float amount)
	{
		mMaterial->SetReflectionAmount(amount);
	}

	const DirectX::XMFLOAT3& CubeModel::Position() const
	{
		return mPosition;
	}

	const DirectX::XMFLOAT3& CubeModel::Direction() const
	{
		return mDirection;
	}

	const DirectX::XMFLOAT3& CubeModel::Up() const
	{
		return mUp;
	}

	const DirectX::XMFLOAT3& CubeModel::Right() const
	{
		return mRight;
	}

	DirectX::XMVECTOR CubeModel::PositionVector() const
	{
		return XMLoadFloat3(&mPosition);
	}

	DirectX::XMVECTOR CubeModel::DirectionVector() const
	{
		return XMLoadFloat3(&mDirection);
	}

	DirectX::XMVECTOR CubeModel::UpVector() const
	{
		return XMLoadFloat3(&mUp);
	}

	DirectX::XMVECTOR CubeModel::RightVector() const
	{
		return XMLoadFloat3(&mRight);
	}

	DirectX::XMVECTOR CubeModel::GetPositionVector()
	{
		return PositionVector();
	}

	void CubeModel::SetPosition(float x, float y, float z)
	{
		XMVECTOR position = XMVectorSet(x, y, z, 1.0f);
		SetPosition(position);
	}

	void CubeModel::SetPosition(DirectX::FXMVECTOR position)
	{
		XMStoreFloat3(&mPosition, position);
		mUpdateWorldMatrix = true;
	}

	void CubeModel::SetPosition(const DirectX::XMFLOAT3& position)
	{
		mPosition = position;
		mUpdateWorldMatrix = true;
	}

	void CubeModel::DrawableUpdate(const GameTime& gameTime)
	{
		Update(gameTime);
	}

	void CubeModel::DrawableDraw(const GameTime& gameTime)
	{
		Draw(gameTime);
	}

	void CubeModel::ApplyRotation(DirectX::CXMMATRIX transform, DirectX::XMFLOAT3 direction, DirectX::XMFLOAT3 up)
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

	void CubeModel::ApplyRotation(DirectX::CXMMATRIX transform)
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

	void CubeModel::ApplyRotation(const DirectX::XMFLOAT4X4& transform)
	{
		XMMATRIX transformMatrix = XMLoadFloat4x4(&transform);
		ApplyRotation(transformMatrix);
	}

	void CubeModel::SetTextureFile(const std::wstring& file)
	{
		filepath = file;
	}

	const std::wstring& CubeModel::GetFilepath()
	{
		return filepath;
	}

	void CubeModel::Initialize()
	{
		auto direct3DDevice = mGame->Direct3DDevice();
		const auto model = mGame->Content().Load<Model>(L"Models\\Cube.model"s);
		Mesh* mesh = model->Meshes().at(0).get();
		VertexPositionTextureNormal::CreateVertexBuffer(direct3DDevice, *mesh, not_null<ID3D11Buffer**>(mVertexBuffer.put()));
		mesh->CreateIndexBuffer(*direct3DDevice, not_null<ID3D11Buffer**>(mIndexBuffer.put()));
		mIndexCount = narrow<uint32_t>(mesh->Indices().size());

		auto colorMap = mGame->Content().Load<Texture2D>(CubeModel::GetFilepath());
		auto environmentMap = mGame->Content().Load<TextureCube>(L"Textures\\DesertCubemap.dds"s);
		mMaterial = make_shared<CubeMaterial>(*mGame, colorMap, environmentMap);
		mMaterial->Initialize();

		auto updateMaterialFunc = [this]() { mUpdateMaterial = true; };
		mCamera->AddViewMatrixUpdatedCallback(updateMaterialFunc);
		mCamera->AddProjectionMatrixUpdatedCallback(updateMaterialFunc);

		auto firstPersonCamera = mCamera->As<FirstPersonCamera>();
		if (firstPersonCamera != nullptr)
		{
			firstPersonCamera->AddPositionUpdatedCallback([this]() {
				mMaterial->UpdateCameraPosition(mCamera->Position());
				});
		}
	}

	void CubeModel::Draw(const GameTime&)
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

		if (mUpdateMaterial)
		{
			const XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
			const XMMATRIX wvp = XMMatrixTranspose(worldMatrix * mCamera->ViewProjectionMatrix());
			mMaterial->UpdateTransforms(wvp, XMMatrixTranspose(worldMatrix));
			mUpdateMaterial = false;
		}

		mMaterial->DrawIndexed(not_null<ID3D11Buffer*>(mVertexBuffer.get()), not_null<ID3D11Buffer*>(mIndexBuffer.get()), mIndexCount);
	}
}