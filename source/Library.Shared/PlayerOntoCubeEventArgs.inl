#pragma once
#include "PlayerOntoCubeEventArgs.h"
#include <cassert>

namespace FieaGameEngine {
    inline PlayerOntoCubeEventArgs::PlayerOntoCubeEventArgs(Direction3D nextDirection, Direction3D nextCubeFace, int_type nextCubePositionX, int_type nextCubePositionY, int_type nextCubePositionZ)
        : _nextDirection{nextDirection}
        , _nextCubeFace{nextCubeFace}
        , _nextCubePosition{nextCubePositionX, nextCubePositionY, nextCubePositionZ}
        , _isPreviousCubeValid{false}
        , _previousDirection{nextDirection}
        , _previousCubeFace{nextCubeFace}
        , _previousCubePosition{nextCubePositionX, nextCubePositionY, nextCubePositionZ} {}

    inline PlayerOntoCubeEventArgs::PlayerOntoCubeEventArgs(
        Direction3DInfo nextDirection,
        Direction3DInfo nextCubeFace,
        IntVector3D nextCubePosition,
        Direction3DInfo previousDirection,
        Direction3DInfo previousCubeFace,
        IntVector3D previousCubePosition
    )   : _nextDirection{std::move(nextDirection)}
        , _nextCubeFace{std::move(nextCubeFace)}
        , _nextCubePosition{std::move(nextCubePosition)}
        , _isPreviousCubeValid{true}
        , _previousDirection{std::move(previousDirection)}
        , _previousCubeFace{std::move(previousCubeFace)}
        , _previousCubePosition{std::move(previousCubePosition)} {}

    inline const Direction3DInfo& PlayerOntoCubeEventArgs::GetNextDirection() const { return _nextDirection; }
    inline const Direction3DInfo& PlayerOntoCubeEventArgs::GetNextCubeFace() const { return _nextCubeFace; }
    inline const IntVector3D& PlayerOntoCubeEventArgs::GetNextCubePosition() const { return _nextCubePosition; }
    inline bool PlayerOntoCubeEventArgs::IsPreviousCubeValid() const { return _isPreviousCubeValid; }
    inline const Direction3DInfo& PlayerOntoCubeEventArgs::GetPreviousDirection() const { assert(_isPreviousCubeValid); return _previousDirection; }
    inline const Direction3DInfo& PlayerOntoCubeEventArgs::GetPreviousCubeFace() const { assert(_isPreviousCubeValid); return _previousCubeFace; }
    inline const IntVector3D& PlayerOntoCubeEventArgs::GetPreviousCubePosition() const { assert(_isPreviousCubeValid); return _previousCubePosition; }

    inline std::unique_ptr<IEventArgs> PlayerOntoCubeEventArgs::Clone() const { return std::make_unique<PlayerOntoCubeEventArgs>(*this); }
}
