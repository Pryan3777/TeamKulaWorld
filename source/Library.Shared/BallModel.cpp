#include "pch.h"
#include "BallModel.h"
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
	BallModel::BallModel(Game& game, const shared_ptr<Camera>& camera) :
		DrawableGameComponent(game, camera)
	{
	}

	BallModel::BallModel(FieaGameEngine::Game& game, const std::shared_ptr<FieaGameEngine::Camera>& camera, float scale) :
		DrawableGameComponent(game, camera), mScale{ scale }
	{
	}

	BallModel::~BallModel()
	{
	}

	float BallModel::AmbientLightIntensity() const
	{
		return mMaterial->AmbientColor().x;
	}

	void BallModel::SetAmbientLightIntensity(float intensity)
	{
		mMaterial->SetAmbientColor(DirectX::XMFLOAT4(intensity, intensity, intensity, 1.0f));
	}

	float BallModel::EnvironmentIntensity() const
	{
		return mMaterial->EnvironmentColor().x;
	}

	void BallModel::SetEnvironmentIntensity(float intensity)
	{
		mMaterial->SetEnvironmentColor(DirectX::XMFLOAT4(intensity, intensity, intensity, 1.0f));
	}

	float BallModel::ReflectionAmount() const
	{
		return mMaterial->ReflectionAmount();
	}

	void BallModel::SetReflectionAmount(float amount)
	{
		mMaterial->SetReflectionAmount(amount);
	}

	const DirectX::XMFLOAT3& BallModel::Position() const
	{
		return mPosition;
	}

	const DirectX::XMFLOAT3& BallModel::Direction() const
	{
		return mDirection;
	}

	const DirectX::XMFLOAT3& BallModel::Up() const
	{
		return mUp;
	}

	const DirectX::XMFLOAT3& BallModel::Right() const
	{
		return mRight;
	}

	DirectX::XMVECTOR BallModel::PositionVector() const
	{
		return XMLoadFloat3(&mPosition);
	}

	DirectX::XMVECTOR BallModel::DirectionVector() const
	{
		return XMLoadFloat3(&mDirection);
	}

	DirectX::XMVECTOR BallModel::UpVector() const
	{
		return XMLoadFloat3(&mUp);
	}

	DirectX::XMVECTOR BallModel::RightVector() const
	{
		return XMLoadFloat3(&mRight);
	}

	DirectX::XMVECTOR BallModel::GetPositionVector()
	{
		return PositionVector();
	}

	void BallModel::SetPosition(float x, float y, float z)
	{
		XMVECTOR position = XMVectorSet(x, y, z, 1.0f);
		SetPosition(position);
	}

	void BallModel::SetPosition(DirectX::FXMVECTOR position)
	{
		XMStoreFloat3(&mPosition, position);
		mUpdateWorldMatrix = true;
	}

	void BallModel::SetPosition(const DirectX::XMFLOAT3& position)
	{
		mPosition = position;
		mUpdateWorldMatrix = true;
	}

	void BallModel::SetTransform(DirectX::XMFLOAT3 targetDirection, DirectX::XMFLOAT3 targetUp )
	{
		XMVECTOR directionVec = XMLoadFloat3(&targetDirection);
		XMVECTOR upVec = XMLoadFloat3(&targetUp);
		XMVECTOR rightVec = XMVector3Cross(directionVec, upVec);

		XMStoreFloat3(&mDirection, directionVec);
		XMStoreFloat3(&mUp, upVec);
		XMStoreFloat3(&mRight, rightVec);

		mUpdateWorldMatrix = true;
	}

	void BallModel::ApplyRotation(DirectX::CXMMATRIX transform, DirectX::XMFLOAT3 direction, DirectX::XMFLOAT3 up)
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

	void BallModel::ApplyRotation(DirectX::CXMMATRIX transform)
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

	void BallModel::ApplyRotation(const DirectX::XMFLOAT4X4& transform)
	{
		DirectX::XMMATRIX transformMatrix = XMLoadFloat4x4(&transform);
		ApplyRotation(transformMatrix);
	}

	void BallModel::DrawableUpdate(const GameTime& gameTime)
	{
		Update(gameTime);
	}

	void BallModel::DrawableDraw(const GameTime& gameTime)
	{
		Draw(gameTime);
	}

	void BallModel::Initialize()
	{
		auto direct3DDevice = mGame->Direct3DDevice();
		const auto model = mGame->Content().Load<Model>(L"Models\\beach_ball.model"s);
		Mesh* mesh = model->Meshes().at(0).get();
		VertexPositionTextureNormal::CreateVertexBuffer(direct3DDevice, *mesh, not_null<ID3D11Buffer**>(mVertexBuffer.put()));
		mesh->CreateIndexBuffer(*direct3DDevice, not_null<ID3D11Buffer**>(mIndexBuffer.put()));
		mIndexCount = narrow<uint32_t>(mesh->Indices().size());


		auto colorMap = mGame->Content().Load<Texture2D>(L"Textures\\diffuse.png"s);
		auto environmentMap = mGame->Content().Load<TextureCube>(L"Textures\\DesertCubemapPAUL.dds"s);
		mMaterial = make_shared<GenericMaterial>(*mGame, colorMap, environmentMap);
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

	void BallModel::Draw(const GameTime&)
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