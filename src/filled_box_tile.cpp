#pragma once
#include <string>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "font.cpp"

struct FilledBoxTile {
    float x;
    float y;
    float dx;
    float dy;
    std::string boxName;
    sf::Texture boxTexture;
    std::vector<sf::Texture> itemTextures;
    std::vector<std::string> itemNames;
    bool IsPresent;

    FilledBoxTile() = default;

    FilledBoxTile(float curdx, float curdy, std::string curBoxName, const std::string boxImg, const std::vector<TItem>& items) {
        x = 0;
        y = 0;
        dx = curdx;
        dy = curdy;
        boxName = curBoxName;

        boxTexture.loadFromMemory(boxImg.c_str(), boxImg.size());

        itemTextures.resize(items.size());
        itemNames.resize(items.size());
        for (size_t i = 0; i < items.size(); i++) {
            itemTextures[i].loadFromMemory(items[i].GetImage().c_str(), items[i].GetImage().size());
            itemNames[i] = items[i].GetItemName();
        }

        IsPresent = false;
    }

    void SetPosition(float curx, float cury) {
        x = curx;
        y = cury;
    }

    void Draw(sf::RenderWindow& window) {
        sf::RectangleShape rectangle(sf::Vector2f(dx, dy));
        rectangle.setPosition(x, y);
        rectangle.setFillColor(sf::Color::White);
        window.draw(rectangle);

        sf::Text boxText;
        boxText.setFont(NFont::font);
        boxText.setString(boxName);
        boxText.setCharacterSize(18);
        boxText.setCharacterSize(std::min(1.0, dx / boxText.getLocalBounds().width * 0.9) * 18.0);
        boxText.setFillColor(sf::Color::Black);
        boxText.setPosition(x + 0.5 * dx - 0.5 * boxText.getLocalBounds().width, y + 5.f);

        sf::Sprite boxSprite;
        boxSprite.setTexture(boxTexture);
        float boxHeight = boxSprite.getLocalBounds().height;
        float boxWidth = boxSprite.getLocalBounds().width;
        float boxScale = std::min(0.4f * dy / boxHeight, dx / boxWidth);
        boxSprite.setPosition(x + 0.5 * dx - 0.5 * boxWidth * boxScale, y + 30.f);
        boxSprite.scale(boxScale, boxScale);

        window.draw(boxText);
        window.draw(boxSprite);

        for (size_t i = 0; i < itemNames.size(); i++) {
            const std::string& itemName = itemNames[i];
            const sf::Texture& itemTexture = itemTextures[i];

            sf::Text itemText;
            itemText.setFont(NFont::font);
            itemText.setString(itemName);
            itemText.setCharacterSize(18);
            itemText.setCharacterSize(std::min(1.0, (dx - 100.f) / itemText.getLocalBounds().width * 0.9) * 18.0);
            itemText.setFillColor(sf::Color::Black);
            itemText.setPosition(x + 100.f, y + 0.55 * dy + i * 90.f);

            sf::Sprite itemSprite;
            itemSprite.setTexture(itemTexture);
            float itemHeight = itemSprite.getLocalBounds().height;
            float itemWidth = itemSprite.getLocalBounds().width;
            float itemScale = std::min(80 / itemHeight, 80 / itemWidth);
            itemSprite.setPosition(x + 10.f, y + 0.55 * dy + i * 90.f);
            itemSprite.scale(itemScale, itemScale);

            window.draw(itemText);
            window.draw(itemSprite);
        }
    }
};

