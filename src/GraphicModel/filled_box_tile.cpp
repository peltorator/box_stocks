#pragma once
#include <string>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "font.cpp"

    struct TFilledBoxTile {
        float X;
        float Y;
        float Dx;
        float Dy;
        std::string BoxName;
        sf::Texture BoxTexture;
        std::vector<sf::Texture> ItemTextures;
        std::vector<std::string> ItemNames;
        bool IsPresent;

        TFilledBoxTile() = default;

        TFilledBoxTile(const float dx, const float dy, const std::string &boxName, const std::string& boxImg, const std::vector<TItem>& items) {
            X = 0;
            Y = 0;
            Dx = dx;
            Dy = dy;
            BoxName = boxName;

            BoxTexture.loadFromMemory(boxImg.c_str(), boxImg.size());

            ItemTextures.resize(items.size());
            ItemNames.resize(items.size());
            for (size_t i = 0; i < items.size(); i++) {
                ItemTextures[i].loadFromMemory(items[i].Image.c_str(), items[i].Image.size());
                ItemNames[i] = items[i].ItemName;
            }

            IsPresent = false;
        }

        void SetPosition(float x, float y) {
            X = x;
            Y = y;
        }

        void Draw(sf::RenderWindow& window) {
            sf::RectangleShape rectangle(sf::Vector2f(Dx, Dy));
            rectangle.setPosition(X, Y);
            rectangle.setFillColor(sf::Color::White);
            window.draw(rectangle);

            sf::Text boxText;
            boxText.setFont(NFont::font);
            boxText.setString(BoxName);
            boxText.setCharacterSize(18);
            boxText.setCharacterSize(std::min(1.0, Dx / boxText.getLocalBounds().width * 0.9) * 18.0);
            boxText.setFillColor(sf::Color::Black);
            sf::FloatRect boxTextRect = boxText.getLocalBounds();
            boxText.setOrigin(boxTextRect.left + boxTextRect.width / 2.0, boxTextRect.top);
            boxText.setPosition(X + 0.5 * Dx, Y + 5.f);

            sf::Sprite boxSprite;
            boxSprite.setTexture(BoxTexture);
            float boxHeight = boxSprite.getLocalBounds().height;
            float boxWidth = boxSprite.getLocalBounds().width;
            float boxScale = std::min(0.4f * Dy / boxHeight, Dx / boxWidth);
            boxSprite.setPosition(X + 0.5 * Dx - 0.5 * boxWidth * boxScale, Y + 50.f);
            boxSprite.scale(boxScale, boxScale);

            window.draw(boxText);
            window.draw(boxSprite);

            for (size_t i = 0; i < ItemNames.size(); i++) {
                const std::string& itemName = ItemNames[i];
                const sf::Texture& itemTexture = ItemTextures[i];

                sf::Text itemText;
                itemText.setFont(NFont::font);
                itemText.setString(itemName);
                itemText.setCharacterSize(18);
                itemText.setCharacterSize(std::min(1.0, (Dx - 100.f) / itemText.getLocalBounds().width * 0.9) * 18.0);
                itemText.setFillColor(sf::Color::Black);
                sf::FloatRect itemTextRect = itemText.getLocalBounds();
                itemText.setOrigin(itemTextRect.left, itemTextRect.top + itemTextRect.height / 2.0);
                itemText.setPosition(X + 85.f, Y + 0.55 * Dy + i * 90.f + 30.f);

                sf::CircleShape itemShape(30);
                itemShape.setTexture(&itemTexture);
                float imageH = itemTexture.getSize().y;
                float imageW = itemTexture.getSize().x;
                float side = std::min(imageW, imageH);
                itemShape.setTextureRect(sf::IntRect((imageW - side) / 2, (imageH - side) / 2, side, side));
                itemShape.setPosition(X + 10.f, Y + 0.55 * Dy + i * 90.f);

                window.draw(itemText);
                window.draw(itemShape);
            }
    }
};
