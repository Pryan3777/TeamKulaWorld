#include "pch.h"
#include "Level.h"
#include "Cube.h"

using namespace std::literals::string_literals;
using namespace DirectX;

#define CUBE_MODEL_PATH "Models\\Cube.model"
#define ONE_CUBE_UNIT 5.f
namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Level);

	Level::Level() : Level("Level"s) {}

	Level::Level(String name) :
		Attributed{Level::TypeIdClass()},
		_cubesDatum{ &(At("Cubes"s)) },
		_playerDatum{&(At("Player"s))}
	{}

	void Level::Initialize(Game& game, std::shared_ptr<Camera> camera, Vector<BaseDrawableGameobject*>& drawVector)
	{

		_cubes = &_cubesDatum->GetTableElement();
		assert(_cubes != nullptr);
		for (Datum::size_type i = 0; i < _cubes->Size(); ++i)
		{
			Datum::Table& cubeScope = _cubes->At(i).GetTableElement();
			assert(cubeScope.Is(Cube::TypeIdClass()));
			Cube& cube = reinterpret_cast<Cube&>(cubeScope);

			cube.Initialize(game, camera, ONE_CUBE_UNIT);

			const glm::vec4& position = cube.GetTransform().GetLocalPosition();
			cube.SetPosition(position.x * ONE_CUBE_UNIT, position.y * ONE_CUBE_UNIT, position.z * ONE_CUBE_UNIT);
			auto a = Colors::AntiqueWhite;
			const XMFLOAT4 b{ a[0], a[1],a[2],a[3] };

			drawVector.EmplaceBack(&cube.GetModel());
		}

		Scope* playerScope = &_playerDatum->GetTableElement();
		assert(playerScope != nullptr && playerScope->Is(Player::TypeIdClass()));
		_player = reinterpret_cast<Player*>(playerScope);
		
		_player->Initialize(game, camera);


		int startingCubeID = _player->GetStartingCube();
		std::string id = std::to_string(startingCubeID);
		auto it = _cubes->Find(id);

		assert(it != _cubes->end());
		Scope& cubeScope = it->second.GetTableElement();
		assert(cubeScope.Is(Cube::TypeIdClass()));
		Cube& startingCube = reinterpret_cast<Cube&>(cubeScope);

		const glm::vec4& cubePosition = startingCube.GetTransform().GetLocalPosition();
		_player->SetCoordinate(IntVector3D{ static_cast<int>(cubePosition.x), static_cast<int>(cubePosition.y), static_cast<int>(cubePosition.z) });
		
		switch (_player->GetStartingFace())
		{
		case Direction3D::Up:
			break;
		case Direction3D::Right:
			RotateRight(camera.get());
			RotateDown(camera.get());
			RotateLeft(camera.get());
			CameraFixup(camera.get());
			break;
		case Direction3D::Left:
			RotateLeft(camera.get());
			RotateDown(camera.get());
			RotateRight(camera.get());
			CameraFixup(camera.get());
			break;
		case Direction3D::Backward:
			RotateRight(camera.get());
			RotateRight(camera.get());
			RotateDown(camera.get());
			CameraFixup(camera.get());
			break;
		case Direction3D::Forward:
			RotateDown(camera.get());
			CameraFixup(camera.get());
			break;
		case Direction3D::Down:
			RotateDown(camera.get());
			RotateDown(camera.get());
			RotateRight(camera.get());
			RotateRight(camera.get());
			CameraFixup(camera.get());
			break;
		}

		for (std::size_t i{ 0 }; i < GetAmountToRotate(_player->GetStartingFace(), _player->GetStartingDirection()); ++i)
		{
			RotateRight(camera.get());
		}
		CameraFixup(camera.get());
		XMMATRIX rotationMat = XMMatrixRotationAxis(camera->UpVector(), XM_PIDIV2);
		camera->ApplyRotation(rotationMat, camera->Direction(), camera->Up());
		
		auto verticalOffset = Direction3DInfo::Direction3DToFloatVector4D(_player->GetStartingFace()) * ((ONE_CUBE_UNIT / 2) + Player::GetPlayerScale());
		auto sum = cubePosition * ONE_CUBE_UNIT;
		sum += verticalOffset;
		_player->SetPosition(sum.x, sum.y, sum.z);
		drawVector.EmplaceBack(&_player->GetModel());
	}

	Cube* Level::GetCube(const IntVector3D v)
	{
		return GetCube(v.x(), v.y(), v.z());
	}

	Cube* Level::GetCube(const std::tuple<int, int, int>& coordinate)
	{
		return GetCube(std::get<0>(coordinate), std::get<1>(coordinate), std::get<2>(coordinate));
	}

	Cube* Level::GetCube(int x, int y, int z)
	{
		Cube* cubeToReturn = nullptr;
		assert(_cubes != nullptr);
		
		int uniqueId = z + (y * _mapZExtent) + (x * _mapZExtent * _mapYExtent);
		std::string test = std::to_string(uniqueId);
		auto it = _cubes->Find(test);

		if (it != _cubes->end())
		{
			Scope* cube = &(it->second.GetTableElement());
			assert(cube->Is(Cube::TypeIdClass()));
			cubeToReturn = reinterpret_cast<Cube*>(cube);
		}
		return cubeToReturn;
	}

	Player& Level::GetPlayer()
	{
		assert(_player != nullptr);
		return *_player;
	}

	SignatureVector Level::Signatures() { return SignatureVector{
		Signature{"MapXExtent"s, DatumType::Integer, true, size_type(1), offsetof(Level, _mapXExtent)},
		Signature{"MapYExtent"s, DatumType::Integer, true, size_type(1), offsetof(Level, _mapYExtent)},
		Signature{"MapZExtent"s, DatumType::Integer, true, size_type(1), offsetof(Level, _mapZExtent)},
		Signature{"Player"s, DatumType::InternalTable, false, size_type(0), std::size_t(0)},
		Signature{"Cubes"s, DatumType::InternalTable, false, size_type(0), std::size_t(0)}
	}; }

	inline typename Level::ScopeUniquePointer Level::Clone() const { return std::make_unique<Level>(*this); }
	
	float Level::GetCubeSideLength()
	{
		return ONE_CUBE_UNIT;
	}
	void Level::RotateRight(Camera* camera)
	{
		auto rotationMatrix = XMMatrixRotationY(0.f) * XMMatrixRotationAxis(_player->UpVector(), -XM_PIDIV2);
		_player->ApplyRotation(rotationMatrix, _player->Direction(), _player->Up());
		camera->ApplyRotation(rotationMatrix, _player->Direction(), _player->Up());
	}
	void Level::RotateLeft(Camera* camera)
	{
		auto rotationMatrix = XMMatrixRotationY(0.f) * XMMatrixRotationAxis(_player->UpVector(), XM_PIDIV2);
		_player->ApplyRotation(rotationMatrix, _player->Direction(), _player->Up());
		camera->ApplyRotation(rotationMatrix, _player->Direction(), _player->Up());
	}
	void Level::RotateDown(Camera* camera)
	{
		auto rotationMatrix = XMMatrixRotationY(0.f) * XMMatrixRotationAxis(_player->RightVector(), -XM_PIDIV2);
		_player->ApplyRotation(rotationMatrix, _player->Direction(), _player->Up());
		camera->ApplyRotation(rotationMatrix, _player->Direction(), _player->Up());
	}
	void Level::CameraFixup(Camera* camera)
	{
		auto directionVec = _player->DirectionVector();
		auto upVec = _player->UpVector();
		auto position = _player->PositionVector();
		upVec *= 2.f;
		directionVec *= -7.5f;
		camera->SetPosition(position + directionVec + upVec);
	}
	std::size_t Level::GetAmountToRotate(Direction3D face, Direction3D targetForward)
	{
		Direction3D startingForward{Direction3D::Forward};
		if (face == Direction3D::Backward || face == Direction3D::Forward)
		{
			startingForward = Direction3D::Down;
		}

		auto forwardVector = Direction3DInfo::Direction3DToIntVector3D(startingForward);
		auto targetVector = Direction3DInfo::Direction3DToIntVector3D(targetForward);
		auto result = (forwardVector.x() * targetVector.x()) + (forwardVector.y() * targetVector.y()) + (forwardVector.z() * targetVector.z());

		if (result >= 0.9f)
		{
			return 0;
		}
		else if (result <= -0.9f)
		{
			return 2;
		}

		Direction3DInfo forwardInfo{ Direction3D::Forward };
		auto crossResult = forwardInfo.Cross(targetForward);

		std::size_t amount = crossResult.IsNegative() ? 3 : 1;

		return amount;
	}
}
