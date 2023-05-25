#pragma once
#include "CubeOccupantInteractedEventArgs.h"

namespace FieaGameEngine {
    inline CubeOccupantInteractedEventArgs::CubeOccupantInteractedEventArgs(OccupantType type, Datum::Integer value) : _occupantType{type}, _value{value} {}
    inline typename CubeOccupantInteractedEventArgs::OccupantType CubeOccupantInteractedEventArgs::GetOccupantType() const { return _occupantType; }
    inline typename Datum::Integer CubeOccupantInteractedEventArgs::GetValue() const { return _value; }
    inline std::unique_ptr<IEventArgs> CubeOccupantInteractedEventArgs::Clone() const { return std::make_unique<CubeOccupantInteractedEventArgs>(*this); }
}
