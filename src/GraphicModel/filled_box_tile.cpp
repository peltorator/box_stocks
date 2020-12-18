#pragma once
#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "font.cpp"
#include "../ShopModel/item.cpp"
#include "../ShopModel/box.cpp"
#include "../DataProvider/data_provider.cpp"

struct TFilledBoxTile {
    float X;
    float Y;
    float Dx;
    float Dy;
    std::string BoxName;
    uint64_t BoxCost;
    sf::Texture BoxTexture;
    std::vector<sf::Texture> ItemTextures;
    std::vector<std::string> ItemNames;
    std::vector<uint64_t> ItemCosts;
    uint64_t TotalItemsCost;
    bool IsPresent;

    TFilledBoxTile() = default;

    TFilledBoxTile(const float dx, const float dy, const uint64_t& boxID, const std::string& boxImg, const std::vector<uint64_t>& itemIDs) {
        X = 0;
        Y = 0;
        Dx = dx;
        Dy = dy;

        const TBox& box = NDataProvider::IdToBox[boxID];

        BoxName = box.BoxName;
        BoxTexture.loadFromMemory(boxImg.c_str(), boxImg.size());
        BoxCost = box.Cost;

        ItemTextures.resize(itemIDs.size());
        ItemNames.resize(itemIDs.size());
        ItemCosts.resize(itemIDs.size());
        TotalItemsCost = 0;
        for (size_t i = 0; i < itemIDs.size(); i++) {
            std::cout << itemIDs[i] << std::endl;
            assert(NDataProvider::IdToItem.count(itemIDs[i]));
            const TItem& item = NDataProvider::IdToItem[itemIDs[i]];
            ItemTextures[i].loadFromMemory(item.Image.c_str(), item.Image.size());
            ItemNames[i] = item.ItemName;
            ItemCosts[i] = item.Cost;
            TotalItemsCost += item.Cost;
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

        sf::Text totalCostText;
        totalCostText.setFont(NFont::font);
        totalCostText.setString("Total Cost: " + std::to_string(BoxCost + TotalItemsCost) + " Dollars");
        totalCostText.setCharacterSize(18);
        totalCostText.setFillColor(sf::Color::Black);
        totalCostText.setPosition(X + 10.f, Y + 0.9 * Dy);

        sf::Sprite boxSprite;
        boxSprite.setTexture(BoxTexture);
        float boxHeight = boxSprite.getLocalBounds().height;
        float boxWidth = boxSprite.getLocalBounds().width;
        float boxScale = std::min(0.4f * Dy / boxHeight, Dx / boxWidth);
        boxSprite.setPosition(X + 0.5 * Dx - 0.5 * boxWidth * boxScale, Y + 50.f);
        boxSprite.scale(boxScale, boxScale);

        window.draw(boxText);
        window.draw(totalCostText);
        window.draw(boxSprite);

        for (size_t i = 0; i < ItemNames.size(); i++) {
            if (i <= 1) {
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
                itemText.setPosition(X + 85.f, Y + 0.55 * Dy + i * 90.f + 15.f);

                sf::Text itemCostText;
                itemCostText.setFont(NFont::font);
                itemCostText.setString(std::to_string(ItemCosts[i]) + " Dollars");
                itemCostText.setCharacterSize(18);
                itemCostText.setFillColor(sf::Color::Black);
                sf::FloatRect itemCostTextRect = itemCostText.getLocalBounds();
                itemCostText.setOrigin(itemCostTextRect.left, itemCostTextRect.top + itemCostTextRect.height / 2.0);
                itemCostText.setPosition(X + 85.f, Y + 0.55 * Dy + i * 90.f + 45.f);

                sf::CircleShape itemShape(30);
                itemShape.setTexture(&itemTexture);
                float imageH = itemTexture.getSize().y;
                float imageW = itemTexture.getSize().x;
                float side = std::min(imageW, imageH);
                itemShape.setTextureRect(sf::IntRect((imageW - side) / 2, (imageH - side) / 2, side, side));
                itemShape.setPosition(X + 10.f, Y + 0.55 * Dy + i * 90.f);

                window.draw(itemText);
                window.draw(itemCostText);
                window.draw(itemShape);
            } else if (i == 2) {
                sf::Text dotsText;
                dotsText.setFont(NFont::font);
                dotsText.setString("...");
                dotsText.setCharacterSize(40);
                dotsText.setFillColor(sf::Color::Black);
                sf::FloatRect dotsTextRect = dotsText.getLocalBounds();
                dotsText.setOrigin(dotsTextRect.left + dotsTextRect.width / 2.0, dotsTextRect.top + dotsTextRect.height / 2.0);
                dotsText.setPosition(X + 0.5 * Dx, Y + 0.55 * Dy + i * 90.f);

                window.draw(dotsText);
            } else {
                break;
            }
        }
    }
};

