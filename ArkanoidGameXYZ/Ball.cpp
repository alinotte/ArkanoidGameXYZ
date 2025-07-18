#include "Ball.h"
#include "GameSettings.h"
#include <cmath>

namespace Arkanoid
{
	void Ball::Init(const sf::Vector2f& position)
	{
		shape.setRadius(BALL_RADIUS);
		shape.setOrigin(BALL_RADIUS, BALL_RADIUS);
		shape.setPosition(position);
		shape.setFillColor(sf::Color::White);

		NormalizeVelocity();
	}

	void Ball::Update(float deltaTime)
	{
		if (!isStuck)
		{
			shape.move(velocity * deltaTime);
		}
	}

	void Ball::NormalizeVelocity()
	{
		float length = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
		if (length > 0) {
			velocity = velocity / length * speed;
		}
	}

	void Ball::Draw(sf::RenderWindow& window)
	{
		window.draw(shape);
	}
}