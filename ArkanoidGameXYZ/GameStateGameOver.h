#pragma once
#include "SFML/Graphics.hpp"

namespace Arkanoid
{
	class Game;

	class GameStateGameOver
	{
	public:
		// Returns pointer to the allocated data
		void Init(Game& game);
		void Shutdown(Game& game);
		void HandleWindowEvent(Game& game, const sf::Event& event);
		void Update(Game& game, float timeDelta);
		void Draw(Game& game, sf::RenderWindow& window);

	private:
		// Resources
		sf::Font font;

		float timeSinceGameOver = 0.f;

		// UI data
		sf::RectangleShape background;
		sf::Text gameOverText;
		sf::Text hintText;
		std::vector<sf::Text> recordsTableTexts;
	};
}
