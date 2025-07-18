#pragma once
#include <SFML/Graphics.hpp>
#include "Sprite.h"
#include "GameSettings.h"
#include <unordered_map>

namespace Arkanoid
{
	enum class GameOptions: std::uint8_t
	{
		InfiniteApples = 1 << 0,
		WithAcceleration = 1 << 1,

		Default = InfiniteApples | WithAcceleration,
		Empty = 0
	};

	enum class GameStateType
	{
		None = 0,
		MainMenu,
		Playing,
		GameOver,
		ExitDialog,
		Records,
	};

	class GameState
	{
	public:
		GameState(GameStateType type = GameStateType::None, void* data = nullptr, bool isExclusivelyVisible = false)
			: type(type), data(data), isExclusivelyVisible(isExclusivelyVisible) {
		}

		GameStateType type = GameStateType::None;
		void* data = nullptr;
		bool isExclusivelyVisible = false;
	};

	enum class GameStateChangeType
	{
		None,
		Push,
		Pop,
		Switch
	};

	class Game 
	{
	public:
		void Init();
		void HandleWindowEvents(sf::RenderWindow& window);
		bool Update(float timeDelta); // Return false if game should be closed
		
		void Draw(sf::RenderWindow& window);
		void Shutdown();

		void PushGameState(GameStateType stateType, bool isExclusivelyVisible);
		void PopGameState();
		void SwitchGameState(GameStateType newState);

		GameOptions GetOptions() const { return options; }
		void SetOptions(GameOptions newOptions) { options = newOptions; }

		const std::unordered_map<std::string, int>& GetRecordsTable() const { return recordsTable; }
		void SetRecordsTable(const std::unordered_map<std::string, int>& newRecords) { recordsTable = newRecords; }
		void AddRecord(const std::string& name, int score)
		{
			if (recordsTable.find(name) != recordsTable.end()) {
				recordsTable[name] = std::max(recordsTable[name], score);
			}
			else {
				recordsTable[name] = score;
			}
		}

		GameStateType GetCurrentStateType() const {
			return !gameStateStack.empty() ? gameStateStack.back().type : GameStateType::None;
		}

		bool IsStateChangePending() const {
			return gameStateChangeType != GameStateChangeType::None;
		}

		GameStateChangeType GetPendingStateChangeType() const {
			return gameStateChangeType;
		}

	private:
		void InitGameState(GameState& state);
		void ShutdownGameState(GameState& state);
		void HandleWindowEventGameState(GameState& state, sf::Event& event);
		void UpdateGameState(GameState& state, float timeDelta);
		void DrawGameState(GameState& state, sf::RenderWindow& window);

		std::vector<GameState> gameStateStack;
		GameStateChangeType gameStateChangeType = GameStateChangeType::None;
		GameStateType pendingGameStateType = GameStateType::None;
		bool pendingGameStateIsExclusivelyVisible = false;

		GameOptions options = GameOptions::Default;
		std::unordered_map<std::string, int> recordsTable;
	};
}
