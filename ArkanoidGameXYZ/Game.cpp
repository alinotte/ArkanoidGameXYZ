#include "Game.h"
#include <assert.h>
#include <algorithm>
#include "GameStatePlaying.h"
#include "GameStateGameOver.h"
#include "GameStatePauseMenu.h"
#include "GameStateMainMenu.h"
#include "GameStateRecords.h"
#include "GameStateWin.h"

namespace Arkanoid
{
	void Game::Init()
	{
		// Generate fake records table
		recordsTable =
		{
			{"John", 100 / 2},
			{"Jane", 100 / 3 },
			{"Alice", 100 / 4 },
			{"Bob", 100 / 5 },
			{"Clementine", 100 / 5 },
		};

		gameStateChangeType = GameStateChangeType::None;
		pendingGameStateType = GameStateType::None;
		pendingGameStateIsExclusivelyVisible = false;
		SwitchGameState(GameStateType::MainMenu);
	}

	void Game::HandleWindowEvents(sf::RenderWindow& window)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Close window if close button or Escape key pressed
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (gameStateStack.size() > 0)
			{
				HandleWindowEventGameState(gameStateStack.back(), event);
			}
		}
	}

	bool Game::Update(float timeDelta)
	{
		if (gameStateChangeType == GameStateChangeType::Switch)
		{
			// Shutdown all game states
			while (gameStateStack.size() > 0)
			{
				ShutdownGameState(gameStateStack.back());
				gameStateStack.pop_back();
			}
		}
		else if (gameStateChangeType == GameStateChangeType::Pop)
		{
			// Shutdown only current game state
			if (gameStateStack.size() > 0)
			{
				ShutdownGameState(gameStateStack.back());
				gameStateStack.pop_back();
			}
		}

		// Initialize new game state if needed
		if (pendingGameStateType != GameStateType::None)
		{
			gameStateStack.push_back({ pendingGameStateType, nullptr, pendingGameStateIsExclusivelyVisible });
			InitGameState(gameStateStack.back());
		}

		gameStateChangeType = GameStateChangeType::None;
		pendingGameStateType = GameStateType::None;
		pendingGameStateIsExclusivelyVisible = false;

		if (gameStateStack.size() > 0)
		{
			UpdateGameState(gameStateStack.back(), timeDelta);
			return true;
		}

		return false;
	}

	void Game::Draw(sf::RenderWindow& window)
	{
		if (gameStateStack.size() > 0)
		{
			std::vector<GameState*> visibleGameStates;
			for (auto it = gameStateStack.rbegin(); it != gameStateStack.rend(); ++it)
			{
				visibleGameStates.push_back(&(*it));
				if (it->isExclusivelyVisible)
				{
					break;
				}
			}

			for (auto it = visibleGameStates.rbegin(); it != visibleGameStates.rend(); ++it)
			{
				DrawGameState(**it, window);
			}
		}
	}

	void Game::Shutdown()
	{
		// Shutdown all game states
		while (gameStateStack.size() > 0)
		{
			ShutdownGameState(gameStateStack.back());
			gameStateStack.pop_back();
		}

		gameStateChangeType = GameStateChangeType::None;
		pendingGameStateType = GameStateType::None;
		pendingGameStateIsExclusivelyVisible = false;
	}

	void Game::PushGameState(GameStateType stateType, bool isExclusivelyVisible)
	{
		pendingGameStateType = stateType;
		pendingGameStateIsExclusivelyVisible = isExclusivelyVisible;
		gameStateChangeType = GameStateChangeType::Push;
	}

	void Game::PopGameState()
	{
		pendingGameStateType = GameStateType::None;
		pendingGameStateIsExclusivelyVisible = false;
		gameStateChangeType = GameStateChangeType::Pop;
	}

	void Game::SwitchGameState(GameStateType newState)
	{
		pendingGameStateType = newState;
		pendingGameStateIsExclusivelyVisible = false;
		gameStateChangeType = GameStateChangeType::Switch;
	}

	void Game::InitGameState(GameState& state)
	{
		switch (state.type)
		{
		case GameStateType::MainMenu:
		{
			state.data = new GameStateMainMenu();
			((GameStateMainMenu*)state.data)->Init(*this);
			break;
		}
		case GameStateType::Playing:
		{
			state.data = new GameStatePlaying();
			((GameStatePlaying*)state.data)->Init(*this);
			break;
		}
		case GameStateType::GameOver:
		{
			state.data = new GameStateGameOver();
			((GameStateGameOver*)state.data)->Init(*this);
			break;
		}
		case GameStateType::ExitDialog:
		{
			state.data = new GameStatePauseMenu();
			((GameStatePauseMenu*)state.data)->Init(*this);
			break;
		}
		case GameStateType::Records:
		{
			state.data = new GameStateRecords();
			((GameStateRecords*)state.data)->Init(*this);
			break;
		}
		case GameStateType::Win:
		{
			state.data = new GameStateWin();
			((GameStateWin*)state.data)->Init(*this);
			break;
		}
		default:
			assert(false); // We want to know if we forgot to implement new game state
			break;
		}
	}

	void Game::ShutdownGameState(GameState& state)
	{
		switch (state.type)
		{
		case GameStateType::MainMenu:
		{
			((GameStateMainMenu*)state.data)->Shutdown(*this);
			delete (GameStateMainMenu*)state.data;
			break;
		}
		case GameStateType::Playing:
		{
			((GameStatePlaying*)state.data)->Shutdown(*this);
			delete (GameStatePlaying*)state.data;
			break;
		}
		case GameStateType::GameOver:
		{
			((GameStateGameOver*)state.data)->Shutdown(*this);
			delete (GameStateGameOver*)state.data;
			break;
		}
		case GameStateType::ExitDialog:
		{
			((GameStatePauseMenu*)state.data)->Shutdown(*this);
			delete (GameStatePauseMenu*)state.data;
			break;
		}
		case GameStateType::Records:
		{
			((GameStateRecords*)state.data)->Shutdown(*this);
			delete (GameStateRecords*)state.data;
			break;
		}
		case GameStateType::Win:
		{
			((GameStateWin*)state.data)->Shutdown(*this);
			delete ((GameStateWin*)state.data);
			break;
		}
		default:
			assert(false); // We want to know if we forgot to implement new game state
			break;
		}

		state.data = nullptr;
	}

	void Game::HandleWindowEventGameState(GameState& state, sf::Event& event)
	{
		switch (state.type)
		{
		case GameStateType::MainMenu:
		{
			((GameStateMainMenu*)state.data)->HandleWindowEvent(*this, event);
			break;
		}
		case GameStateType::Playing:
		{
			((GameStatePlaying*)state.data)->HandleWindowEvent(*this, event);
			break;
		}
		case GameStateType::GameOver:
		{
			((GameStateGameOver*)state.data)->HandleWindowEvent(*this, event);
			break;
		}
		case GameStateType::ExitDialog:
		{
			((GameStatePauseMenu*)state.data)->HandleWindowEvent(*this, event);
			break;
		}
		case GameStateType::Records:
		{
			((GameStateRecords*)state.data)->HandleWindowEvent(*this, event);
			break;
		}
		case GameStateType::Win:
		{
			((GameStateWin*)state.data)->HandleWindowEvent(*this, event);
			break;
		}
		default:
			assert(false); // We want to know if we forgot to implement new game state
			break;
		}
	}

	void Game::UpdateGameState(GameState& state, float timeDelta)
	{
		switch (state.type)
		{
		case GameStateType::MainMenu:
		{
			((GameStateMainMenu*)state.data)->Update(*this, timeDelta);
			break;
		}
		case GameStateType::Playing:
		{
			((GameStatePlaying*)state.data)->Update(*this, timeDelta);
			break;
		}
		case GameStateType::GameOver:
		{
			((GameStateGameOver*)state.data)->Update(*this, timeDelta);
			break;
		}
		case GameStateType::ExitDialog:
		{
			((GameStatePauseMenu*)state.data)->Update(*this, timeDelta);
			break;
		}
		case GameStateType::Records:
		{
			((GameStateRecords*)state.data)->Update(*this, timeDelta);
			break;
		}
		case GameStateType::Win:
			((GameStateWin*)state.data)->Update(*this, timeDelta);
			break;
		default:
			assert(false); // We want to know if we forgot to implement new game state
			break;
		}
	}

	void Game::DrawGameState(GameState& state, sf::RenderWindow& window)
	{
		switch (state.type)
		{
		case GameStateType::MainMenu:
		{
			((GameStateMainMenu*)state.data)->Draw(*this, window);
			break;
		}
		case GameStateType::Playing:
		{
			((GameStatePlaying*)state.data)->Draw(*this, window);
			break;
		}
		case GameStateType::GameOver:
		{
			((GameStateGameOver*)state.data)->Draw(*this, window);
			break;
		}
		case GameStateType::ExitDialog:
		{
			((GameStatePauseMenu*)state.data)->Draw(*this, window);
			break;
		}
		case GameStateType::Records:
		{
			((GameStateRecords*)state.data)->Draw(*this, window);
			break;
		}
		case GameStateType::Win:
		{
			((GameStateWin*)state.data)->Draw(*this, window);
			break;
		}
		default:
			assert(false); // We want to know if we forgot to implement new game state
			break;
		}
	}

}
