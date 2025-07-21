#include "Paddle.h"
#include "GameSettings.h"
#include <SFML/Window/Keyboard.hpp>

namespace Arkanoid
{
	void Paddle::Init(const sf::Vector2f& position)
	{
		shape.setSize({ PADDLE_WIDTH, PADDLE_HEIGHT });
		shape.setPosition(position);
		shape.setFillColor(sf::Color::White);
	}

	void Paddle::CheckBounds()
	{
		sf::FloatRect paddleBounds = shape.getGlobalBounds();
		if (paddleBounds.left < 0)
		{
			shape.setPosition(0, shape.getPosition().y);
		}
		else if (shape.getPosition().x + paddleBounds.width > SCREEN_WIDTH)
		{
			shape.setPosition(SCREEN_WIDTH - paddleBounds.width,
				shape.getPosition().y);
		}
	}

	void Paddle::Update(float deltaTime)
	{
		float direction = 0.f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			direction -= 1.f;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			direction += 1.f;
		}

		shape.move(direction * speed * deltaTime, 0.f);

		CheckBounds();
	}

	void Paddle::Draw(sf::RenderWindow& window)
	{
		window.draw(shape);
	}
}