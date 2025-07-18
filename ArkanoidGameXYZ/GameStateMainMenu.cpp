#include "GameStateMainMenu.h"
#include "Game.h"
#include <assert.h>

namespace Arkanoid
{
	void GameStateMainMenu::Init(Game& game)
	{
		assert(font.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"));

		menu.GetRootItem().GetHintText().setString("Snake Game");
		menu.GetRootItem().GetHintText().setFont(font);
		menu.GetRootItem().GetHintText().setCharacterSize(48);
		menu.GetRootItem().GetHintText().setFillColor(sf::Color::Red);
		menu.GetRootItem().SetChildrenOrientation(Orientation::Vertical);
		menu.GetRootItem().SetChildrenAlignment(Alignment::Middle);
		menu.GetRootItem().SetChildrenSpacing(10.f);
		menu.GetRootItem().AddChild(&startGameItem);
		menu.GetRootItem().AddChild(&optionsItem);
		menu.GetRootItem().AddChild(&recordsItem);
		menu.GetRootItem().AddChild(&exitGameItem);

		startGameItem.GetText().setString("Start Game");
		startGameItem.GetText().setFont(font);
		startGameItem.GetText().setCharacterSize(24);
		
		optionsItem.GetText().setString("Options");
		optionsItem.GetText().setFont(font);
		optionsItem.GetText().setCharacterSize(24);
		optionsItem.GetHintText().setString("Options");
		optionsItem.GetHintText().setFont(font);
		optionsItem.GetHintText().setCharacterSize(48);
		optionsItem.GetHintText().setFillColor(sf::Color::Red);
		optionsItem.SetChildrenOrientation(Orientation::Vertical);
		optionsItem.SetChildrenAlignment(Alignment::Middle);
		optionsItem.SetChildrenSpacing(10.f);
		optionsItem.AddChild(&optionsInfiniteApplesItem);
		optionsItem.AddChild(&optionsWithAccelerationItem);

		optionsInfiniteApplesItem.GetText().setString("Infinite Apples: On/Off");
		optionsInfiniteApplesItem.GetText().setFont(font);
		optionsInfiniteApplesItem.GetText().setCharacterSize(24);

		optionsWithAccelerationItem.GetText().setString("With Acceleration: On/Off");
		optionsWithAccelerationItem.GetText().setFont(font);
		optionsWithAccelerationItem.GetText().setCharacterSize(24);

		recordsItem.GetText().setString("Records");
		recordsItem.GetText().setFont(font);
		recordsItem.GetText().setCharacterSize(24);

		exitGameItem.GetText().setString("Exit Game");
		exitGameItem.GetText().setFont(font);
		exitGameItem.GetText().setCharacterSize(24);
		exitGameItem.GetHintText().setString("Are you sure?");
		exitGameItem.GetHintText().setFont(font);
		exitGameItem.GetHintText().setCharacterSize(48);
		exitGameItem.GetHintText().setFillColor(sf::Color::Red);
		exitGameItem.SetChildrenOrientation(Orientation::Horizontal);
		exitGameItem.SetChildrenAlignment(Alignment::Middle);
		exitGameItem.SetChildrenSpacing(10.f);
		exitGameItem.AddChild(&yesItem);
		exitGameItem.AddChild(&noItem);

		yesItem.GetText().setString("Yes");
		yesItem.GetText().setFont(font);
		yesItem.GetText().setCharacterSize(24);

		noItem.GetText().setString("No");
		noItem.GetText().setFont(font);
		noItem.GetText().setCharacterSize(24);

		menu.InitItem(menu.GetRootItem());
		menu.SelectItem(&startGameItem);
	}

	void GameStateMainMenu::Shutdown(Game& game)
	{
		// No need to do anything here
	}

	void GameStateMainMenu::HandleWindowEvent(Game& game, const sf::Event& event)
	{
		if (!menu.GetSelectedItem())
		{
			return;
		}

		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				menu.CollapseSelectedItem();
			}
			else if (event.key.code == sf::Keyboard::Enter)
			{
				if (menu.GetSelectedItem() == &startGameItem)
				{
					game.SwitchGameState(GameStateType::Playing);
				}
				else if (menu.GetSelectedItem() == &optionsItem)
				{
					menu.ExpandSelectedItem();
				}
				else if (menu.GetSelectedItem() == &optionsInfiniteApplesItem)
				{
					game.SetOptions((GameOptions)((std::uint8_t)game.GetOptions() ^ (std::uint8_t)GameOptions::InfiniteApples));
				}
				else if (menu.GetSelectedItem() == &optionsWithAccelerationItem)
				{
					game.SetOptions((GameOptions)((std::uint8_t)game.GetOptions() ^ (std::uint8_t)GameOptions::WithAcceleration));
				}
				else if (menu.GetSelectedItem() == &recordsItem)
				{
					game.PushGameState(GameStateType::Records, true);
				}
				else if (menu.GetSelectedItem() == &exitGameItem)
				{
					menu.ExpandSelectedItem();
				}
				else if (menu.GetSelectedItem() == &yesItem)
				{
					game.SwitchGameState(GameStateType::None);
				}
				else if (menu.GetSelectedItem() == &noItem)
				{
					menu.CollapseSelectedItem();
				}
				else
				{
					menu.ExpandSelectedItem();
				}
			}
			
			Orientation orientation = menu.GetSelectedItem()->GetParent()->GetChildrenOrientation();
			if (orientation == Orientation::Vertical && event.key.code == sf::Keyboard::Up ||
				orientation == Orientation::Horizontal && event.key.code == sf::Keyboard::Left)
			{
				menu.SelectPreviousItem();
			}
			else if (orientation == Orientation::Vertical && event.key.code == sf::Keyboard::Down ||
						orientation == Orientation::Horizontal && event.key.code == sf::Keyboard::Right)
			{
				menu.SelectNextItem();
			}
		}
	}

	void GameStateMainMenu::Update(Game& game, float timeDelta)
	{
		bool isInfiniteApples = ((std::uint8_t)game.GetOptions() & (std::uint8_t)GameOptions::InfiniteApples) != (std::uint8_t)GameOptions::Empty;
		optionsInfiniteApplesItem.GetText().setString("Infinite Apples: " + std::string(isInfiniteApples ? "On" : "Off"));

		bool isWithAcceleration = ((std::uint8_t)game.GetOptions() & (std::uint8_t)GameOptions::WithAcceleration) != (std::uint8_t)GameOptions::Empty;
		optionsWithAccelerationItem.GetText().setString("With Acceleration: " + std::string(isWithAcceleration ? "On" : "Off"));
	}

	void GameStateMainMenu::Draw(Game& game, sf::RenderWindow& window)
	{
		sf::Vector2f viewSize = (sf::Vector2f)window.getView().getSize();

		sf::Text* hintText = &menu.GetContext()->GetHintText();
		hintText->setOrigin(GetTextOrigin(*hintText, { 0.5f, 0.f }));
		hintText->setPosition(viewSize.x / 2.f, 150.f);
		window.draw(*hintText);

		menu.Draw(window, viewSize / 2.f, { 0.5f, 0.f });
	}

}
