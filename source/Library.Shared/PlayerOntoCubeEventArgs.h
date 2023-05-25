#pragma once
#include "IEventArgs.h"
#include "Direction3D.h"
#include "IntVector3D.h"

namespace FieaGameEngine {
    class PlayerOntoCubeEventArgs final : public IEventArgs {
        RTTI_DECLARATIONS(PlayerOntoCubeEventArgs, IEventArgs);

        using int_type = IntVector3D::int_type;

    public:
        PlayerOntoCubeEventArgs(Direction3D nextDirection, Direction3D nextCubeFace, int_type nextCubePositionX, int_type nextCubePositionY, int_type nextCubePositionZ);
        PlayerOntoCubeEventArgs(
            Direction3DInfo nextDirection,
            Direction3DInfo nextCubeFace,
            IntVector3D nextCubePosition,
            Direction3DInfo previousDirection,
            Direction3DInfo previousCubeFace,
            IntVector3D previousCubePosition
        );

        const Direction3DInfo& GetNextDirection() const;
        const Direction3DInfo& GetNextCubeFace() const;
        const IntVector3D& GetNextCubePosition() const;
        bool IsPreviousCubeValid() const;
        const Direction3DInfo& GetPreviousDirection() const;
        const Direction3DInfo& GetPreviousCubeFace() const;
        const IntVector3D& GetPreviousCubePosition() const;

        std::unique_ptr<IEventArgs> Clone() const override;

    private:
        Direction3DInfo _nextDirection;
        Direction3DInfo _nextCubeFace;
        IntVector3D _nextCubePosition;
        bool _isPreviousCubeValid;
        Direction3DInfo _previousDirection;
        Direction3DInfo _previousCubeFace;
        IntVector3D _previousCubePosition;

    };
}

#include "PlayerOntoCubeEventArgs.inl"
