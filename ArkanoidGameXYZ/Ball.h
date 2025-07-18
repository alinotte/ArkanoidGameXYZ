#pragma once
#include <SFML/Graphics.hpp>
#include "GameSettings.h"

namespace Arkanoid
{
	class Ball
	{
	public:
		void Init(const sf::Vector2f& position);
		void Update(float deltaTime);
		void Draw(sf::RenderWindow& window);

		void StickToPaddle(const sf::Vector2f& paddlePosition)
		{
			SetPosition({paddlePosition.x + PADDLE_WIDTH / 2.0f, paddlePosition.y - BALL_RADIUS});
			isStuck = true;
		}

		bool IsStuckToPaddle() const { return isStuck; }
		void SetStuckToPaddle(bool stuck) { isStuck = stuck; }

		sf::Vector2f GetPosition() const { return shape.getPosition(); }
		sf::Vector2f GetVelocity() const { return velocity; }
		sf::FloatRect GetGlobalBounds() const { return shape.getGlobalBounds(); }
		float GetSpeed() const { return speed; }

		void SetPosition(const sf::Vector2f& position) { shape.setPosition(position); }
		void SetVelocity(const sf::Vector2f& newVelocity)
		{ 
			velocity = newVelocity;
			NormalizeVelocity();
		}
		void SetSpeed(float newSpeed) { speed = newSpeed; }

	private:
		void NormalizeVelocity();

		sf::CircleShape shape;
		sf::Vector2f velocity;
		float speed = 0;
		bool isStuck = true;
	};
}
