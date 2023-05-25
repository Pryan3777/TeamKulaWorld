#pragma once
#include "Cube.h"

namespace FieaGameEngine {
    inline Cube::Cube() : Cube{DEFAULT_NAME} {}

    inline bool Cube::IsAtPosition(const IntVector3D& position) const { return IsAtPosition(position.x(), position.y(), position.z()); }

    inline void Cube::Initialize(Game& game, const std::shared_ptr<Camera>& camera, float scale) {
        _cubeModel = std::make_unique<Rendering::CubeModel>(game, camera, scale);
        _cubeModel->Initialize();
    }
    inline void Cube::Draw(const GameTime& gameTime) { _cubeModel->Draw(gameTime); }

    inline void Cube::UpdateSelf(const GameTime& gameTime) { _cubeModel->Update(gameTime); }
    inline typename Rendering::CubeModel& Cube::GetModel() { return *_cubeModel.get(); }

    inline typename Cube::ScopeUniquePointer Cube::Clone() const { return std::make_unique<Cube>(*this); }
}
