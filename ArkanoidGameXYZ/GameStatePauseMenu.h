#pragma once
#include <SFML/Graphics.hpp>
#include "Menu.h"

namespace Arkanoid
{
	class Game;

	struct GameStatePauseMenu
	{
	public:
		void Init(Game& game);
		void Shutdown(Game& game);
		void HandleWindowEvent(Game& game, const sf::Event& event);
		void Update(Game& game, float timeDelta);
		void Draw(Game& game, sf::RenderWindow& window);

	private:
		sf::Font font;

		sf::RectangleShape background;
		sf::Text titleText;

		Menu menu;
		MenuItem resumeItem;
		MenuItem exitItem;
	};
}
