#pragma once
#include "Attributed.h"
#include "AttributedSignatureRegistry.h"
#include "Direction3D.h"
#include "EventSubscriber.h"

namespace FieaGameEngine {
    class Cube;

    class CubeOccupant final : public Attributed {
        friend class Cube;
        friend class ScopeAttribute<CubeOccupant>;
        friend std::unique_ptr<CubeOccupant> std::make_unique<CubeOccupant>();

        RTTI_DECLARATIONS(CubeOccupant, Attributed);

    public:
        enum OccupantType : Datum::Integer {
            TYPE_UNKNOWN = -1,
            TYPE_COLLECTABLE = 0,
            TYPE_GOAL,
            TYPE_HAZARD
        };

        static SignatureVector Signatures();

        const Direction3DInfo& GetOccupiedFace() const;
        OccupantType GetOccupantType() const;
        Datum::Integer GetValue() const;
        bool IsVisible() const;

        ScopeUniquePointer Clone() const override;

    private:
        CubeOccupant();

        static const key_type OCCUPANT_TYPE_KEY;
        static const key_type VALUE_KEY;
        static const key_type IS_VISIBLE_KEY;

        Direction3DInfo _occupiedFace;
        Datum::Integer _occupantType{-1};
        Datum::Integer _value{0};
        Datum::Integer _isVisible{1};

        std::shared_ptr<EventSubscriber> _subscriber;

    };

    FACTORY(CubeOccupant, Scope);
}

#include "CubeOccupant.inl"
