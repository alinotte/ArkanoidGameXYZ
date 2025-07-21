#pragma once
#include <SFML/Graphics.hpp>

namespace Arkanoid
{
	class GameObject
	{
	public:
		virtual ~GameObject() = default;
		virtual void Update(float deltaTime) = 0;
		virtual void Draw(sf::RenderWindow& window) = 0;
		virtual sf::FloatRect GetGlobalBounds() const = 0;
		virtual void OnCollision() {}
	};
}