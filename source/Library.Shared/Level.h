#pragma once
#include "Attributed.h"
#include "ProxyModel.h"
#include "Cube.h"
#include "Player.h"
#include "BaseDrawableGameobject.h"
#include "Direction3D.h"
namespace FieaGameEngine
{
	class Game;
	class Camera;
	using SignatureVector = Vector<Attributed::Signature>;
	class Level final : public Attributed
	{
		RTTI_DECLARATIONS(Level, Attributed);
	public:
		Level();
		explicit Level(String name);

		void Initialize(Game& game, std::shared_ptr<Camera> camera, Vector<BaseDrawableGameobject*>& drawVector);

		Cube* GetCube(const IntVector3D v);
		Cube* GetCube(const std::tuple<int, int, int>& coordinate);
		Cube* GetCube(const int x, const int y, const int z);

		Player& GetPlayer();

		static SignatureVector Signatures();

		[[nodiscard]] virtual ScopeUniquePointer Clone() const override;
		static float GetCubeSideLength();

	private:

		void RotateRight(Camera*);
		void RotateLeft(Camera*);
		void RotateDown(Camera*);
		void CameraFixup(Camera*);

		std::size_t GetAmountToRotate(Direction3D face, Direction3D targetForward);

		Datum* _cubesDatum;
		Scope* _cubes;
		Datum* _playerDatum;
		Player* _player;
		int _mapXExtent;
		int _mapYExtent;
		int _mapZExtent;
	};
	FACTORY(Level, Scope);
}

