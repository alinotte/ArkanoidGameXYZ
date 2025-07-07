#pragma once
#include "SFML/Graphics.hpp"
#include "Text.h"
#include <list>

namespace SnakeGame
{
	class MenuItem
	{
	public:
		void SetText(const sf::Text& newText) { text = newText; }
		sf::Text& GetText() { return text; }

		void SetHintText(const sf::Text& newHintText) { hintText = newHintText; }
		sf::Text& GetHintText() { return hintText; }

		void SetChildrenOrientation(Orientation orientation) { childrenOrientation = orientation; }
		Orientation GetChildrenOrientation() const { return childrenOrientation; }

		void SetChildrenAlignment(Alignment alignment) { childrenAlignment = alignment; }
		Alignment GetChildrenAlignment() const { return childrenAlignment; }

		void SetChildrenSpacing(float spacing) { childrenSpacing = spacing; }
		float GetChildrenSpacing() const { return childrenSpacing; }

		void SetSelectedColor(const sf::Color& color) { selectedColor = color; }
		sf::Color GetSelectedColor() const { return selectedColor; }

		void SetDeselectedColor(const sf::Color& color) { deselectedColor = color; }
		sf::Color GetDeselectedColor() const { return deselectedColor; }

		void SetEnabled(bool enabled) { isEnabled = enabled; }
		bool IsEnabled() const { return isEnabled; }

		const std::vector<MenuItem*>& GetChildren() const { return children; }
		void AddChild(MenuItem* child) {
			child->parent = this;
			children.push_back(child);
		}

		void SetParent(MenuItem* parentItem) { parent = parentItem; }
		MenuItem* GetParent() const { return parent; }

	private:
		sf::Text text;
		sf::Text hintText; // Visible when child item is selected
		Orientation childrenOrientation = Orientation::Vertical;
		Alignment childrenAlignment = Alignment::Min;
		float childrenSpacing;

		sf::Color selectedColor = sf::Color::Yellow;
		sf::Color deselectedColor = sf::Color::White;

		bool isEnabled = true;
		std::vector<MenuItem*> children;

		MenuItem* parent = nullptr;
	};

	class Menu
	{
	public:
		MenuItem& GetRootItem() { return rootItem; }
		const MenuItem& GetRootItem() const { return rootItem; }

		MenuItem* GetSelectedItem() const { return selectedItem; }
		void SetSelectedItem(MenuItem* item) { selectedItem = item; }

		// Links children to parent
		void InitItem(MenuItem& menu);
		void SelectItem(MenuItem* item);
		bool SelectPreviousItem();
		bool SelectNextItem();
		bool ExpandSelectedItem();
		bool CollapseSelectedItem();

		MenuItem* GetContext();

		void Draw(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f origin);

	private:
		MenuItem rootItem;
		MenuItem* selectedItem = nullptr;
	};
}
