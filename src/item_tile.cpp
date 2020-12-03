#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include "button.cpp"
#include "font.cpp"

struct ItemTile {
    float x;
    float y;
    float dx;
    float dy;
    uint64_t ItemID;
    std::string name;
    uint32_t cnt;
    uint32_t maxcnt;
    bool ShowCnt;
    Button minusButton;
    Button plusButton;
    sf::Texture pictureTexture;
    bool IsPresent;

    ItemTile() = default;

    ItemTile(float curdx, float curdy, uint64_t itemID, std::string curname, uint32_t curmaxcnt, const std::string img, bool showCnt) {
        x = 0;
        y = 0;
        dx = curdx;
        dy = curdy;
        ItemID = itemID;
        name = curname;
        cnt = 0;
        maxcnt = curmaxcnt;
        ShowCnt = showCnt;
        minusButton = Button(0, 0, 0.25 * dx, 0.2 * dy, "-");
        plusButton = Button(0, 0, 0.25 * dx, 0.2 * dy, "+");

        pictureTexture.loadFromMemory(img.c_str(), img.size());

        IsPresent = false;
    }

    void SetPosition(float curx, float cury) {
        x = curx;
        y = cury;
        minusButton.x = x + 0.1 * dx;
        minusButton.y = y + 0.7 * dy;
        plusButton.x = x + 0.65 * dx;
        plusButton.y = y + 0.7 * dy;
    }

    void Draw(sf::RenderWindow& window) {
        sf::RectangleShape rectangle(sf::Vector2f(dx, dy));
        rectangle.setPosition(x, y);
        rectangle.setFillColor(sf::Color::White);

        sf::Text text;
        text.setFont(NFont::font);
        text.setString(name);
        text.setCharacterSize(18);
        text.setCharacterSize(std::min(1.0, dx / text.getLocalBounds().width * 0.9) * 18.0);
        text.setFillColor(sf::Color::Black);
        text.setPosition(x + 0.5 * dx - 0.5 * text.getLocalBounds().width, y + 0.54 * dy);

        sf::Text cntText;
        cntText.setFont(NFont::font);
        cntText.setString((ShowCnt ? std::to_string(cnt) + " / " : "") + std::to_string(maxcnt));
        cntText.setCharacterSize(18);
        cntText.setCharacterSize(std::min(1.0, 0.25 * dx / cntText.getLocalBounds().width) * 18.0);
        cntText.setFillColor(sf::Color::Black);
        cntText.setPosition(x + 0.5 * dx - 0.5 * cntText.getLocalBounds().width, y + 0.8 * dy - 0.5 * cntText.getLocalBounds().height);

        sf::Sprite pictureSprite;
        pictureSprite.setTexture(pictureTexture);
        float pictureHeight = pictureSprite.getLocalBounds().height;
        float pictureWidth = pictureSprite.getLocalBounds().width;
        float scale = std::min(0.5f * dy / pictureHeight, dx / pictureWidth);
        pictureSprite.setPosition(x + 0.5 * dx - 0.5 * pictureWidth * scale, y + 5.f);
        pictureSprite.scale(scale, scale);
        
        window.draw(rectangle);
        window.draw(text);
        window.draw(cntText);
        window.draw(pictureSprite);
        minusButton.Draw(window);
        plusButton.Draw(window);
    }
};

