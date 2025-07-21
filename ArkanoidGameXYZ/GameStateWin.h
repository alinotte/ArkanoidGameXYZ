#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

namespace Arkanoid
{
	class Game;

	class GameStateWin
	{
	public:
		void Init(Game& game);
		void Shutdown(Game& game);
		void HandleWindowEvent(Game& game, const sf::Event& event);
		void Update(Game& game, float deltaTime);
		void Draw(Game& game, sf::RenderWindow& window);

	private:
		sf::Font font;
		sf::Text winText;
		sf::Text hintText;
		sf::Text scoreText;
		std::vector<sf::Text> recordsTableTexts;
		sf::RectangleShape background;
		int finalScore = 0;
	};
}