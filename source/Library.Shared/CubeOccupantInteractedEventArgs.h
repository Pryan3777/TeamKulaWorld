#pragma once
#include "CubeOccupant.h"
#include "IEventArgs.h"

namespace FieaGameEngine {
    class CubeOccupantInteractedEventArgs : public IEventArgs {
        RTTI_DECLARATIONS(CubeOccupantInteractedEventArgs, IEventArgs);

    public:
        using OccupantType = CubeOccupant::OccupantType;

        CubeOccupantInteractedEventArgs() = default;
        CubeOccupantInteractedEventArgs(OccupantType type, Datum::Integer value);

        OccupantType GetOccupantType() const;
        Datum::Integer GetValue() const;

        std::unique_ptr<IEventArgs> Clone() const override;

    private:
        CubeOccupant::OccupantType _occupantType{CubeOccupant::TYPE_UNKNOWN};
        Datum::Integer _value{0};

    };
}

#include "CubeOccupantInteractedEventArgs.inl"
