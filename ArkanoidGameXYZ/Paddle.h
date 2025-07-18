#pragma once
#include <SFML/Graphics.hpp>
#include "GameSettings.h"

namespace Arkanoid
{
	class Paddle
	{
	public:
		void Init(const sf::Vector2f& position);
		void Update(float deltaTime);
		void Draw(sf::RenderWindow& window);

		sf::Vector2f GetPosition() const { return shape.getPosition(); }
		sf::Vector2f GetSize() const { return shape.getSize(); }
		sf::FloatRect GetGlobalBounds() const { return shape.getGlobalBounds(); }
		float GetSpeed() const { return speed; }

		void SetPosition(const sf::Vector2f& position) { shape.setPosition(position); }
		void SetSpeed(float newSpeed) { speed = newSpeed; }


	private:
		void CheckBounds();

		sf::RectangleShape shape;
		float speed;
	};
}