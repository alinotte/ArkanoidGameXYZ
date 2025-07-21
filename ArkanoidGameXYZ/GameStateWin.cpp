#include "GameStateWin.h"
#include "Game.h"
#include "Text.h"
#include <assert.h>
#include <sstream>
#include <map>

namespace Arkanoid
{
	void GameStateWin::Init(Game& game)
	{
		assert(font.loadFromFile(FONTS_PATH + "Roboto-Regular.ttf"));

		finalScore = game.GetRecordsTable().at(PLAYER_NAME);

		background.setFillColor(sf::Color(0, 0, 0, 200));

		winText.setFont(font);
		winText.setCharacterSize(48);
		winText.setFillColor(sf::Color::Green);
		winText.setString("YOU WIN!");

		std::stringstream finalScoreStream;
		finalScoreStream << "Your score: " << finalScore;
		scoreText.setFont(font);
		scoreText.setCharacterSize(36);
		scoreText.setFillColor(sf::Color::Yellow);
		scoreText.setString(finalScoreStream.str());

		recordsTableTexts.reserve(MAX_RECORDS_TABLE_SIZE);

		std::multimap<int, std::string> sortedRecordsTable;
		int playerScore = game.GetRecordsTable().at(PLAYER_NAME);
		for (const auto& item : game.GetRecordsTable())
		{
			sortedRecordsTable.insert(std::make_pair(item.second, item.first));
		}

		bool isPlayerInTop = false;

		auto it = sortedRecordsTable.rbegin();
		for (int i = 0; i < MAX_RECORDS_TABLE_SIZE && it != sortedRecordsTable.rend(); ++i, ++it) // Note, we can do several actions in for action block
		{
			recordsTableTexts.emplace_back(); // Create text in place
			sf::Text& text = recordsTableTexts.back();

			// We can use streams for writing into string and reading from it
			std::stringstream sstream;
			sstream << i + 1 << ". " << it->second << ": " << it->first;
			text.setString(sstream.str());
			text.setFont(font);
			text.setCharacterSize(24);
			if (it->second == PLAYER_NAME)
			{
				text.setFillColor(sf::Color::Green);
				isPlayerInTop = true;
			}
			else
			{
				text.setFillColor(sf::Color::White);
			}
		}

		// If snake is not in table, replace last element with him
		if (!isPlayerInTop)
		{
			sf::Text& text = recordsTableTexts.back();
			std::stringstream sstream;
			sstream << MAX_RECORDS_TABLE_SIZE << ". " << PLAYER_NAME << ": " << playerScore;
			text.setString(sstream.str());
			text.setFillColor(sf::Color::Green);
		}

		hintText.setFont(font);
		hintText.setCharacterSize(24);
		hintText.setFillColor(sf::Color::White);
		hintText.setString("Play again? (ENTER - YES, ESC - NO)");
	}

	void GameStateWin::Shutdown(Game& game)
	{

	}

	void GameStateWin::HandleWindowEvent(Game& game, const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Enter)
			{
				game.SwitchGameState(GameStateType::Playing);
			}
			else if(event.key.code == sf::Keyboard::Escape)
			{
				game.SwitchGameState(GameStateType::MainMenu);
			}
		}
	}

	void GameStateWin::Update(Game& game, float deltaTime)
	{
		static float blinkTimer = 0;
		blinkTimer += deltaTime;
		if (blinkTimer > 0.5f)
		{
			blinkTimer = 0;
			winText.setFillColor(winText.getFillColor() == sf::Color::Green ?
				sf::Color(100, 255, 100) : sf::Color::Green);
		}
	}

	void GameStateWin::Draw(Game& game, sf::RenderWindow& window)
	{
		sf::Vector2f viewSize = window.getView().getSize();

		background.setSize(viewSize);
		window.draw(background);

		winText.setOrigin(GetTextOrigin(winText, { 0.5f, 1.f }));
		winText.setPosition(viewSize.x / 2.f, viewSize.y / 2.f - 120.f);
		window.draw(winText);

		scoreText.setOrigin(GetTextOrigin(scoreText, { 0.5f, 0.5f }));
		scoreText.setPosition(viewSize.x / 2.f, viewSize.y / 2.f - 60.f);
		window.draw(scoreText);

		// We need to create new vector here as DrawItemsList needs vector of pointers
		std::vector<sf::Text*> textsList;
		textsList.reserve(recordsTableTexts.size());
		for (auto& text : recordsTableTexts)
		{
			textsList.push_back(&text);
		}

		sf::Vector2f tablePosition = { viewSize.x / 2, viewSize.y / 2.f };
		DrawTextList(window, textsList, 10.f, Orientation::Vertical, Alignment::Min, tablePosition, { 0.5f, 0.f });

		hintText.setOrigin(GetTextOrigin(hintText, { 0.5f, 1.f }));
		hintText.setPosition(viewSize.x / 2.f, viewSize.y - 50.f);
		window.draw(hintText);
	}
}