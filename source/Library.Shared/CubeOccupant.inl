#pragma once
#include "CubeOccupant.h"

namespace FieaGameEngine {
    inline const Direction3DInfo& CubeOccupant::GetOccupiedFace() const { return _occupiedFace; }
    inline typename CubeOccupant::OccupantType CubeOccupant::GetOccupantType() const { return static_cast<OccupantType>(_occupantType); }
    inline typename Datum::Integer CubeOccupant::GetValue() const { return _value; }
    inline bool CubeOccupant::IsVisible() const { return _isVisible != 0; }
    inline typename CubeOccupant::ScopeUniquePointer CubeOccupant::Clone() const { return std::make_unique<CubeOccupant>(*this); }
}
