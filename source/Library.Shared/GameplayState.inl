#pragma once
#include "GameplayState.h"

namespace FieaGameEngine {
    inline bool GameplayState::DestroySingleton() { bool isDestroyed = !!_singleton; _singleton.reset(); return isDestroyed; }

    inline GameplayState::GameplayState() { _clock.UpdateGameTime(_time); }

    inline Stack<Scope>& GameplayState::ActionArgumentStack() { return _actionArgumentStack; }
    inline EventQueue& GameplayState::Events() { return _events; }
    inline const GameClock& GameplayState::GetClock() const { return _clock; }
    inline const GameTime& GameplayState::GetTime() const { return _time; }

    inline const GameTime& GameplayState::ProgressTime() { _clock.UpdateGameTime(_time); _events.Update(_time); return _time; }
    inline const GameTime& GameplayState::ResetTime() { _clock.Reset(); _clock.UpdateGameTime(_time); return _time; }

    inline const ReversePolishEvaluator& GameplayState::RpnEvaluator() const { return _rpnEval; }
}
