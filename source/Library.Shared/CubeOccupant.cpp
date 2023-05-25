#include "pch.h"
#include "Cube.h"
#include "CubeOccupant.h"
#include "Event.h"
#include "CubeOccupantInteractedEventArgs.h"
#include "PlayerOntoCubeEventArgs.h"
#include "GameplayState.h"
#include <cassert>

using namespace std::literals::string_literals;

namespace FieaGameEngine {
    RTTI_DEFINITIONS(CubeOccupant);

    const typename CubeOccupant::key_type CubeOccupant::OCCUPANT_TYPE_KEY = "OccupantType"s;
    const typename CubeOccupant::key_type CubeOccupant::VALUE_KEY = "Value"s;
    const typename CubeOccupant::key_type CubeOccupant::IS_VISIBLE_KEY = "IsVisible"s;

    SignatureVector CubeOccupant::Signatures() {
        return SignatureVector{
            Signature{OCCUPANT_TYPE_KEY, DatumType::Integer, true, size_type(1), offsetof(CubeOccupant, _occupantType)},
            Signature{VALUE_KEY, DatumType::Integer, true, size_type(1), offsetof(CubeOccupant, _value)},
            Signature{IS_VISIBLE_KEY, DatumType::Integer, true, size_type(1), offsetof(CubeOccupant, _isVisible), true}
        };
    }

    CubeOccupant::CubeOccupant() : Attributed{CubeOccupant::TypeIdClass()}, _occupiedFace{Direction3D::Right}, _subscriber{std::make_shared<EventSubscriber>([this](const IEventArgs& iea){
        // THIS CODE BLOCK IS NOT THE CONSTRUCTOR! This defines the EventSubscriber::Notify behavior for _subscriber.
        if (!IsVisible() && (_occupantType != TYPE_UNKNOWN)) {
            return;
        }
        assert(_parent);
        assert(_parent->Is(Cube::TypeIdClass()));
        auto* cube = reinterpret_cast<Cube*>(_parent);
        assert(iea.Is(PlayerOntoCubeEventArgs::TypeIdClass()));
        const auto& args = static_cast<const PlayerOntoCubeEventArgs&>(iea);
        if ((args.GetNextCubeFace() == _occupiedFace) && cube->IsAtPosition(args.GetNextCubePosition())) {
            Event interaction{std::make_unique<CubeOccupantInteractedEventArgs>(static_cast<OccupantType>(_occupantType), _value)};
            interaction.Publish();
            if (_occupantType == TYPE_COLLECTABLE) {
                _isVisible = 0;
            }
        }
    })} {
        // THIS code block is the constructor.
        Event::Subscribe<PlayerOntoCubeEventArgs>(_subscriber);
    }
}
