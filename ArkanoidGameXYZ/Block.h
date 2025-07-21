#pragma once
#include "GameObject.h"
#include "GameSettings.h"

namespace Arkanoid
{
	class Block : public GameObject
	{
	public:
		Block(const sf::Vector2f& position, const sf::Color& color = sf::Color::Red);

		void Update(float deltaTime) override;
		void Draw(sf::RenderWindow& window) override;
		sf::FloatRect GetGlobalBounds() const override;
		void OnCollision() override;

		bool IsDestroyed() const { return isDestroyed; }
		void SetDestroyed(bool destroyed) { isDestroyed = destroyed; }

	private:
		sf::RectangleShape shape;
		bool isDestroyed = false;
	};
}