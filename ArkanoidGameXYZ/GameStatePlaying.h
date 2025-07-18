#pragma once
#include "SFML/Graphics.hpp"
#include "Ball.h"
#include "Paddle.h"
#include <vector>

namespace Arkanoid
{
	class Game;

	class GameStatePlaying
	{
	public:
		void Init(Game& game);
		void Shutdown(Game& game);
		void HandleWindowEvent(Game& game, const sf::Event& event);
		void Update(Game& game, float timeDelta);
		void Draw(Game& game, sf::RenderWindow& window);

	private:
		void CheckCollisions(Game& game);
		void ResetBall();
		void ResetPaddle();

		// Resources
		sf::Font font;

		// Game data
		Ball ball;
		Paddle paddle;
		int score = 0;
		int lives = 3;

		// UI data
		sf::Text scoreText;
		sf::Text inputHintText;
		sf::RectangleShape background;
	};
}
