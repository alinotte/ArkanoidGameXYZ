#include "GameStatePlaying.h"
#include "Game.h"
#include "Text.h"
#include <assert.h>
#include <sstream>
#include <algorithm>

namespace Arkanoid
{
	void GameStatePlaying::Init(Game& game)
	{	
		// Init game resources (terminate if error)
		assert(font.loadFromFile(FONTS_PATH + "Roboto-Regular.ttf"));

		// Init background
		background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
		background.setPosition(0.f, 0.f);
		background.setFillColor(sf::Color(0, 200, 0));

		paddle.Init(sf::Vector2f(SCREEN_WIDTH / 2.f - PADDLE_WIDTH / 2.f,
			SCREEN_HEIGHT - PADDLE_HEIGHT - PADDLE_BOTTOM_MARGIN));
		paddle.SetSpeed(PADDLE_SPEED);

		ball.Init(sf::Vector2f(paddle.GetPosition().x + PADDLE_WIDTH / 2,
			paddle.GetPosition().y - BALL_RADIUS));
		ball.SetSpeed(BALL_INITIAL_SPEED);
		ball.SetStuckToPaddle(true);

		// reset game state
		score = 0;
		lives = 3;
		blocks.clear();

		// init blocks
		const float startX = (SCREEN_WIDTH - (COLUMNS * (BLOCK_WIDTH + BLOCK_COL_SPACING) - BLOCK_COL_SPACING)) / 2.f;
		const float startY = 50.f;
		
		for (int j = 0; j < ROWS; ++j)
		{
			for (int i = 0; i < COLUMNS; ++i)
			{
				sf::Vector2f position(
					startX + i * (BLOCK_WIDTH + BLOCK_COL_SPACING),
					startY + j * (BLOCK_HEIGHT + BLOCK_ROW_SPACING)
				);

				sf::Color color;
				if (j == 0) color = sf::Color::Red;
				else if (j == 1) color = sf::Color::Green;
				else color = sf::Color::Blue;

				blocks.emplace_back(position, color);
			}
		}

		// init UI
		scoreText.setFont(font);
		scoreText.setCharacterSize(24);
		scoreText.setFillColor(sf::Color::White);
		scoreText.setString("Score: " + std::to_string(score));
		scoreText.setOrigin(GetTextOrigin(scoreText, { 0.f, 0.f }));

		inputHintText.setFont(font);
		inputHintText.setCharacterSize(24);
		inputHintText.setFillColor(sf::Color::White);
		inputHintText.setString("Use arrow keys to move, SPACE to launch, ESC to pause");
		inputHintText.setOrigin(GetTextOrigin(inputHintText, { 1.f, 0.f }));
	}

	void GameStatePlaying::Shutdown(Game& game)
	{
		// We dont need to free resources here, because they will be freed automatically
	}

	void GameStatePlaying::HandleWindowEvent(Game& game, const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				game.PushGameState(GameStateType::ExitDialog, false);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && ball.IsStuckToPaddle())
			{
				ball.SetStuckToPaddle(false);
				ball.SetVelocity({ 0.5f, -0.5f });
			}
		}
	}

	void GameStatePlaying::CheckCollisions(Game& game)
	{
		sf::Vector2f ballVelocity = ball.GetVelocity();

		// ball with window bounds
		if (ball.GetPosition().x - BALL_RADIUS <= 0 ||
			ball.GetPosition().x + BALL_RADIUS >= SCREEN_WIDTH)
		{
			ballVelocity.x = -ballVelocity.x;
			ball.SetVelocity(ballVelocity);
		}

		if (ball.GetPosition().y - BALL_RADIUS <= 0)
		{
			ballVelocity.y = -ballVelocity.y;
			ball.SetVelocity(ballVelocity);
		}

		// ball lost
		if (ball.GetPosition().y + BALL_RADIUS >= SCREEN_HEIGHT)
		{
			lives--;
			if (lives <= 0)
			{
				// game over
				game.AddRecord(PLAYER_NAME, score);
				game.PushGameState(GameStateType::GameOver, false);
			}
			else
			{
				ResetBall();
				ResetPaddle();
			}
		}

		// ball with paddle
		if (ball.GetGlobalBounds().intersects(paddle.GetGlobalBounds()))
		{
			// Add some directional influence based on where ball hits paddle
			float hitPosition = (ball.GetPosition().x - (paddle.GetPosition().x + PADDLE_WIDTH / 2.f)) / (PADDLE_WIDTH / 2.f);
			float angle = hitPosition * MAX_BOUNCE_ANGLE;

			ball.SetVelocity({ std::sin(angle) * ball.GetSpeed(), -std::abs(std::cos(angle) * ball.GetSpeed()) });
		}

		// ball with blocks
		for (auto& block : blocks)
		{
			if (!block.IsDestroyed() && ball.GetGlobalBounds().intersects(block.GetGlobalBounds()))
			{
				block.OnCollision();
				score += 10;

				sf::Vector2f ballVelocity = ball.GetVelocity();
				ballVelocity.y = -ballVelocity.y;
				ball.SetVelocity(ballVelocity);

				break;
			}
		}

		
	}

	void GameStatePlaying::ResetBall()
	{
		ball.SetPosition(sf::Vector2f((paddle.GetPosition().x + PADDLE_WIDTH / 2.f),
			paddle.GetPosition().y - BALL_RADIUS));
		ball.SetStuckToPaddle(true);
	}

	void GameStatePlaying::ResetPaddle()
	{
		paddle.SetPosition(sf::Vector2f(SCREEN_WIDTH / 2.f - PADDLE_WIDTH / 2.f,
			SCREEN_HEIGHT - PADDLE_HEIGHT - PADDLE_BOTTOM_MARGIN));
	}

	void GameStatePlaying::Update(Game& game, float timeDelta)
	{
		paddle.Update(timeDelta);
		if (ball.IsStuckToPaddle())
		{
			ball.StickToPaddle(paddle.GetPosition());
		}
		else
		{
			ball.Update(timeDelta);
			CheckCollisions(game);
		}

		bool allBlocksDestroyed = true;
		for (const auto& block : blocks)
		{
			if (!block.IsDestroyed())
			{
				allBlocksDestroyed = false;
				break;
			}
		}

		if (allBlocksDestroyed)
		{
			game.AddRecord(PLAYER_NAME, score);
			game.SwitchGameState(GameStateType::Win);
		}
	}

	void GameStatePlaying::Draw(Game& game, sf::RenderWindow& window)
	{
		// Draw background
		window.draw(background);

		paddle.Draw(window);
		ball.Draw(window);
		for (auto& block : blocks)
		{
			block.Draw(window);
		}

		scoreText.setString("Score: " + std::to_string(score));
		scoreText.setOrigin(GetTextOrigin(scoreText, { 0.f, 0.f }));
		scoreText.setPosition(10.f, 10.f);
		window.draw(scoreText);

		sf::Vector2f viewSize = window.getView().getSize();
		inputHintText.setPosition(viewSize.x - 10.f, 10.f);
		window.draw(inputHintText);
	}
}
