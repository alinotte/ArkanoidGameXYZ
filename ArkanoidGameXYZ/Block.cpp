#include "Block.h"

namespace Arkanoid
{
	Block::Block(const sf::Vector2f& position, const sf::Color& color)
	{
		shape.setSize({ BLOCK_WIDTH, BLOCK_HEIGHT });
		shape.setPosition(position);
		shape.setFillColor(color);
		shape.setOutlineThickness(1.f);
		shape.setOutlineColor(sf::Color::White);
	}

	void Block::Update(float deltaTime)
	{

	}

	void Block::Draw(sf::RenderWindow& window)
	{
		if (!isDestroyed)
		{
			window.draw(shape);
		}
	}

	sf::FloatRect Block::GetGlobalBounds() const
	{
		return isDestroyed ? sf::FloatRect() : shape.getGlobalBounds();
	}

	void Block::OnCollision()
	{
		isDestroyed = true;
	}
}