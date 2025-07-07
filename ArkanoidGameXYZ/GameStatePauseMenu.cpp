#include "GameStatePauseMenu.h"
#include "Game.h"
#include <assert.h>

namespace SnakeGame
{
	void GameStatePauseMenu::Init(Game& game)
	{
		assert(font.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"));

		background.setFillColor(sf::Color(0, 0, 0, 128)); // Semi-transparent black

		titleText.setString("Pause");
		titleText.setFont(font);
		titleText.setCharacterSize(48);
		titleText.setFillColor(sf::Color::Red);

		menu.GetRootItem().SetChildrenOrientation(Orientation::Vertical);
		menu.GetRootItem().SetChildrenAlignment(Alignment::Middle);
		menu.GetRootItem().AddChild(&resumeItem);
		menu.GetRootItem().AddChild(&exitItem);
		
		resumeItem.GetText().setString("Return to game");
		resumeItem.GetText().setFont(font);
		resumeItem.GetText().setCharacterSize(24);

		exitItem.GetText().setString("Exit to main menu");
		exitItem.GetText().setFont(font);
		exitItem.GetText().setCharacterSize(24);

		menu.InitItem(menu.GetRootItem());
		menu.SelectItem(&resumeItem);
	}

	void GameStatePauseMenu::Shutdown(Game& game)
	{
		// We dont need to free resources here, because they will be freed automatically
	}

	void GameStatePauseMenu::HandleWindowEvent(Game& game, const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				game.PopGameState();
			}

			if ( menu.GetSelectedItem() == nullptr)
			{
				return;
			}

			if (event.key.code == sf::Keyboard::Enter)
			{
				if (menu.GetSelectedItem() == &resumeItem)
				{
					game.PopGameState();
				}
				else if (menu.GetSelectedItem() == &exitItem)
				{
					game.SwitchGameState(GameStateType::MainMenu);
				}
			}

			Orientation orientation = menu.GetSelectedItem()->GetParent()->GetChildrenOrientation();
			if (event.key.code == sf::Keyboard::Up)
			{
				menu.SelectPreviousItem();
			}
			else if (event.key.code == sf::Keyboard::Down)
			{
				menu.SelectNextItem();
			}
		}
	}

	void GameStatePauseMenu::Update(Game& game, float timeDelta)
	{

	}

	void GameStatePauseMenu::Draw(Game& game, sf::RenderWindow& window)
	{
		sf::Vector2f viewSize = (sf::Vector2f)window.getView().getSize();
		
		background.setSize(viewSize);
		window.draw(background);

		titleText.setOrigin(GetTextOrigin(titleText, { 0.5f, 0.f }));
		titleText.setPosition(viewSize.x / 2.f, 100);
		window.draw(titleText);

		menu.Draw(window, window.getView().getCenter(), { 0.5f, 0.f });
	}

}
