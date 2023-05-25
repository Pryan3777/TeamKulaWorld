#include "pch.h"
#include "AttributedSignatureRegistry.h"
#include "GameplayState.h"
#include "Player.h"
#include "Cube.h"
#include <ScopeParseWrapper.h>
#include <JsonParseCoordinator.h>
#include <ScopeJsonKeyTokenTransmuter.h>
#include <AllScopeJsonParseHelper.h>
#include "ClockEventArgs.h"
#include "ScoreIncrementEventArgs.h"
#include "Level.h"
#pragma once

namespace FieaGameEngine {
	void GameFree()
	{
		ScopeFactory::Unregister();
		TransformFactory::Unregister();
		GameObjectFactory::Unregister();
		TransformFactory::Unregister();
		CubeFactory::Unregister();
		LevelFactory::Unregister();
		CubeOccupantFactory::Unregister();

		AttributedSignatureRegistry::UnregisterSignatures<Transform>();
		AttributedSignatureRegistry::UnregisterSignatures<GameObject>();
		AttributedSignatureRegistry::UnregisterSignatures<Player>();
		AttributedSignatureRegistry::UnregisterSignatures<Cube>();
		AttributedSignatureRegistry::UnregisterSignatures<Level>();
		AttributedSignatureRegistry::UnregisterSignatures<CubeOccupant>();

		GameplayState::DestroySingleton();
		AttributedSignatureRegistry::DestroySingleton();
	}

	void GameSetup()
	{
		GameplayState::CreateSingleton();
		AttributedSignatureRegistry::CreateSingleton();

		AttributedSignatureRegistry::RegisterSignatures<Transform>();
		AttributedSignatureRegistry::RegisterSignatures<GameObject>();
		AttributedSignatureRegistry::RegisterSignatures<Player, GameObject>();
		AttributedSignatureRegistry::RegisterSignatures<Cube, GameObject>();
		AttributedSignatureRegistry::RegisterSignatures<Level>();
		AttributedSignatureRegistry::RegisterSignatures<CubeOccupant>();

		ScopeFactory::Register();
		TransformFactory::Register();
		GameObjectFactory::Register();
		PlayerFactory::Register();
		CubeFactory::Register();
		LevelFactory::Register();
		CubeOccupantFactory::Register();
	}

	void EventSubscribeInitialize(Player& CurrentPlayer, EventQueue& Queue)
	{
		CurrentPlayer.EventListener = std::make_shared<EventSubscriber>([&CurrentPlayer](const IEventArgs& args) {
			const LibraryDesktopTests::ScoreIncrementEventArgs* converted = args.As<LibraryDesktopTests::ScoreIncrementEventArgs>();
			if (converted != nullptr) {
				CurrentPlayer.At("Score").SetElement(CurrentPlayer.At("Score").GetIntegerElement() + static_cast<int>(converted->Increment));
			}
			else {
				const FieaGameEngine::ClockEventArgs* timerArgs = args.As<FieaGameEngine::ClockEventArgs>();
				if (timerArgs != nullptr)
				{
					CurrentPlayer.PausedGame = timerArgs->IsPaused;
				}
			}
			});
		//The subscriber component subscribes to the event type
		Event::Subscribe<LibraryDesktopTests::ScoreIncrementEventArgs>(CurrentPlayer.EventListener);
		Event::Subscribe<FieaGameEngine::ClockEventArgs>(CurrentPlayer.EventListener);
		Queue.Enqueue<FieaGameEngine::ClockEventArgs>(0L, GameplayState::Singleton().GetClock().StartTime(), true);
	}
}