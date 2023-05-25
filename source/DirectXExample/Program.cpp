#include "pch.h"
#include "GameException.h"
#include "UtilityWin32.h"
#include "RenderingGame.h"
#include "GameplayState.h"
#include "KulaHelperFunctions.h"
#include "ScoreIncrementEventArgs.h"
#include "ClockEventArgs.h"
#include "Event.h"
#include "Cube.h"
#pragma once

using namespace FieaGameEngine;
using namespace Rendering;
using namespace std;
using namespace std::string_literals;

Player& DeserializePlayer(std::shared_ptr<Player> root, std::shared_ptr<ScopeParseWrapper> wrapper, JsonParseCoordinator& coordinator)
{

	coordinator.PushBackHelper(std::make_unique<ScopeJsonParse::AllScopeJsonParseHelper>());
	coordinator.PushBackTransmuter(std::make_unique<ScopeJsonParse::ScopeJsonKeyTokenTransmuter>());
	coordinator.DeserializeIntoWrapperFromFile(R"(Files\GameData.json)"s);
	return static_cast<Player&>(root->At("Scene"s).FrontTable());
}
void LoadLevel(RenderingGame& game)
{
	GameplayState::Singleton().ResetTime();
	game.LevelSelected = true;
	game.ResetKulaGameInstance();
}
void CheckGameLoss(RenderingGame& game, EventQueue& Queue)
{
	Queue;
	if (game.mScoreComponent->IsTimeOver)
	{
		game.mScoreComponent->Draw(GameplayState::Singleton().GetTime());
		if (game.mScoreComponent->IsTimeOver)
		{
			Queue.Enqueue<FieaGameEngine::ClockEventArgs>(0L, GameplayState::Singleton().GetClock().StartTime(), true);
			game.InEndScreen = true;
		}
	}
}

void BasicGameLoop(RenderingGame& game, Player& CurrentPlayer, EventQueue& Queue)
{
	game.mScoreComponent.get()->SetScore(CurrentPlayer["Score"].GetIntegerElement());
	//This is a test for the reaction system.
	if (GameplayState::Singleton().GetTime().TotalGameTimeSeconds().count() > 5.0f && GameplayState::Singleton().GetTime().TotalGameTimeSeconds().count() < 6.0f)
	{
		Queue.Enqueue<LibraryDesktopTests::ScoreIncrementEventArgs>(0L, GameplayState::Singleton().GetClock().StartTime(), 2);
	}
	//If game isnt paused, we progress the clock
	if (game.LevelSelected)
	{
		game.Run();
		GameplayState::Singleton().ProgressTime();
	}
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int showCommand)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	ThrowIfFailed(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED), "Error initializing COM.");

	current_path(UtilityWin32::ExecutableDirectory());

	const wstring windowClassName = L"RenderingClass"s;
	const wstring windowTitle = L"Kula World"s;

	const SIZE RenderTargetSize = { 1024, 768 };
	HWND windowHandle;
	WNDCLASSEX window;

	UtilityWin32::InitializeWindow(window, windowHandle, instance, windowClassName, windowTitle, RenderTargetSize, showCommand);

	auto getRenderTargetSize = [&RenderTargetSize](SIZE& renderTargetSize)
	{
		renderTargetSize = RenderTargetSize;
	};

	auto getWindow = [&]() -> void*
	{
		return reinterpret_cast<void*>(windowHandle);
	};

	//Creates and registers necessary objects and types
	GameSetup();

	RenderingGame game(getWindow, getRenderTargetSize);
	game.UpdateRenderTargetSize();
	game.Initialize();

	auto root = std::make_shared<Player>();
	auto wrapper = std::make_shared<ScopeParseWrapper>(root);
	auto coordinator = JsonParseCoordinator(wrapper);
	Player& CurrentPlayer = DeserializePlayer(root, wrapper, coordinator);//Deserializing player data

	EventQueue Queue{};//Initializing Queue
	EventSubscribeInitialize(CurrentPlayer, Queue);	//This sets up the unique reactions

	MSG message{ 0 };
	try
	{
		while (message.message != WM_QUIT)
		{
			if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
			else if(!game.InMenu && !game.InEndScreen) //This triggers ingame actions
			{
				if (!game.LevelSelected)
				{
					LoadLevel(game);//Loads the level into the game itself if not already loaded
				}
				if (CurrentPlayer.PausedGame)//If the player has left the menu but the game is paused, we enqueue a new event to unpause the clock
				{
					Queue.Enqueue<FieaGameEngine::ClockEventArgs>(0L, GameplayState::Singleton().GetClock().StartTime(), false);
				}
				BasicGameLoop(game,CurrentPlayer, Queue); //Basic, ingame behavior, letting the player mvoe around and continuing clock tick
				CheckGameLoss(game, Queue); //Checks to see if the timer hit 0
			}
			else if (game.InEndScreen) //End Screen behavior
			{
				game.InEndScreen = !game.RenderEndScreen(GameplayState::Singleton().GetTime(), CurrentPlayer["Score"].GetIntegerElement(), 0);
				game.InMenu = true;
			}
			else if(game.InMenu) //Level Select/Start Screen behavior
			{
				CurrentPlayer["Score"].SetElement(0);
				game.LevelSelected = false;
				game.InMenu = !game.RenderStartScreen(GameplayState::Singleton().GetTime());
			}
		}
	}
	catch (GameException ex)
	{
		MessageBox(windowHandle, ex.whatw().c_str(), windowTitle.c_str(), MB_ABORTRETRYIGNORE);
	}

	game.Shutdown();
	//Frees and destroys unnecessary objects
	GameFree();

	UnregisterClass(windowClassName.c_str(), window.hInstance);
	CoUninitialize();

	return static_cast<int>(message.wParam);
}
