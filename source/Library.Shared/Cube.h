#pragma once
#include "GameObject.h"
#include "ProxyModel.h"
#include "Game.h"
#include "Camera.h"
#include "CubeModel.h"
#include "CubeOccupant.h"

namespace FieaGameEngine {
    class Cube final : public GameObject {
        RTTI_DECLARATIONS(Cube, GameObject);

    public:
        static SignatureVector Signatures();

        Cube();
        explicit Cube(const String& name);
        Cube(const Cube&);
        Cube(Cube&&) noexcept;
        Cube& operator=(const Cube&);
        Cube& operator=(Cube&&) noexcept;
        ~Cube() = default;

        void Initialize(Game& game, const std::shared_ptr<Camera>& camera, float scale = 1.0f);
        void Draw(const GameTime& gameTime);

        void UpdateSelf(const GameTime& gameTime) override;
        Rendering::CubeModel& GetModel();

        bool IsAtPosition(const IntVector3D&) const;
        bool IsAtPosition(int x, int y, int z) const;

        ScopeUniquePointer Clone() const override;

#pragma region Wrappers for Model Functions
        inline const DirectX::XMFLOAT3& Position() const { return _cubeModel->Position(); };

        inline const DirectX::XMFLOAT3& Direction() const { return _cubeModel->Direction(); };
        inline const DirectX::XMFLOAT3& Up() const { return _cubeModel->Up(); };
        inline const DirectX::XMFLOAT3& Right() const { return _cubeModel->Right(); };

        inline DirectX::XMVECTOR PositionVector() const { return _cubeModel->PositionVector(); };
        inline DirectX::XMVECTOR DirectionVector() const { return _cubeModel->DirectionVector(); };
        inline DirectX::XMVECTOR UpVector() const { return _cubeModel->UpVector(); };
        inline DirectX::XMVECTOR RightVector() const { return _cubeModel->RightVector(); };

        inline void SetPosition(float x, float y, float z) { _cubeModel->SetPosition(x, y, z); };
        inline void SetPosition(DirectX::FXMVECTOR position) { _cubeModel->SetPosition(position); };
        inline void SetPosition(const DirectX::XMFLOAT3& position) { _cubeModel->SetPosition(position); };

        inline void ApplyRotation(DirectX::CXMMATRIX transform) { _cubeModel->ApplyRotation(transform); };
        inline void ApplyRotation(DirectX::CXMMATRIX transform, DirectX::XMFLOAT3 direction, DirectX::XMFLOAT3 up) { _cubeModel->ApplyRotation(transform, direction, up); };
        inline void ApplyRotation(const DirectX::XMFLOAT4X4& transform) { _cubeModel->ApplyRotation(transform); };
#pragma endregion

    private:
        static const String DEFAULT_NAME;
        static const key_type RIGHT_KEY;
        static const key_type UP_KEY;
        static const key_type FORWARD_KEY;
        static const key_type LEFT_KEY;
        static const key_type DOWN_KEY;
        static const key_type BACKWARD_KEY;

        std::unique_ptr<Rendering::CubeModel> _cubeModel;

        ScopeAttribute<CubeOccupant> _rightOccupant;
        ScopeAttribute<CubeOccupant> _upOccupant;
        ScopeAttribute<CubeOccupant> _forwardOccupant;
        ScopeAttribute<CubeOccupant> _leftOccupant;
        ScopeAttribute<CubeOccupant> _downOccupant;
        ScopeAttribute<CubeOccupant> _backwardOccupant;

    };

    FACTORY(Cube, Scope);
}

#include "Cube.inl"
