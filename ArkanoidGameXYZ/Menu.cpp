#include "Menu.h"
#include <assert.h>

namespace Arkanoid
{
    void Menu::InitItem(MenuItem& item)
    {
        for (auto child : item.GetChildren())
        {
            child->SetParent(&item);
            InitItem(*child);
        }
    }

    void Menu::SelectItem(MenuItem* item)
    {
        // It is definitely error to select root item
        assert(item != &rootItem);

        if (selectedItem == item)
        {
            return;
        }

        if (item && !item->IsEnabled())
        {
            // Don't allow to select disabled item
            return;
        }

        if (selectedItem)
        {
            selectedItem->GetText().setFillColor(selectedItem->GetDeselectedColor());
        }

        selectedItem = item;

        if (selectedItem)
        {
            selectedItem->GetText().setFillColor(selectedItem->GetSelectedColor());
        }
    }

    bool Menu::SelectPreviousItem()
    {
        if (selectedItem)
        {
            MenuItem* parent = selectedItem->GetParent();
            assert(parent); // There always should be parent
            auto it = std::find(parent->GetChildren().begin(), parent->GetChildren().end(), selectedItem);
            if (it != parent->GetChildren().begin())
            {
                SelectItem(*(--it));
                return true;
            }
        }

        return false;
    }

    bool Menu::SelectNextItem()
    {
        if (selectedItem)
        {
            MenuItem* parent = selectedItem->GetParent();
            assert(parent); // There always should be parent
            auto it = std::find(parent->GetChildren().begin(), parent->GetChildren().end(), selectedItem);
            if (it != parent->GetChildren().end() - 1)
            {
                SelectItem(*(++it));
                return true;
            }
        }
        return false;
    }

    bool Menu::ExpandSelectedItem()
    {
        if (selectedItem && selectedItem->GetChildren().size() > 0)
        {
            SelectItem(selectedItem->GetChildren().front());
            return true;
        }

        return false;
    }

    bool Menu::CollapseSelectedItem()
    {
        if (selectedItem && selectedItem->GetParent() && selectedItem->GetParent() != &rootItem)
        {
            SelectItem(selectedItem->GetParent());
            return true;
        }
        return false;
    }

    MenuItem* Menu::GetContext()
    {
        return selectedItem ? selectedItem->GetParent() : &rootItem;
    }

    void Menu::Draw(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f origin)
    {
        MenuItem* expandedItem = GetContext();

        std::vector<sf::Text*> texts;
        texts.reserve(expandedItem->GetChildren().size());
        for (auto& child : expandedItem->GetChildren())
        {
            if (child->IsEnabled())
            {
                texts.push_back(&child->GetText());
            }
        }

        DrawTextList(window, texts, expandedItem->GetChildrenSpacing(), expandedItem->GetChildrenOrientation(), expandedItem->GetChildrenAlignment(), position, origin);
    }
}