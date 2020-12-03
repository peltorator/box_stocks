#pragma once
#include <cmath>
#include <SFML/Graphics.hpp>
#include "button.cpp"
#include "font.cpp"

struct BoxTile {
    float x;
    float y;
    float dx;
    float dy;
    uint64_t BoxID;
    std::string name;
    bool available;
    Button availableButton;
    sf::Texture pictureTexture;
    bool IsPresent;

    BoxTile() = default;

    BoxTile(float curdx, float curdy, uint64_t boxID, std::string curname, bool curavailable, const std::string img) {
        x = 0;
        y = 0;
        dx = curdx;
        dy = curdy;
        BoxID = boxID;
        name = curname;
        available = curavailable;
        availableButton = Button(0, 0, 0.8 * dx, 0.2 * dy, "");

        pictureTexture.loadFromMemory(img.c_str(), img.size());

        IsPresent = false;
    }

    void SetPosition(float curx, float cury) {
        x = curx;
        y = cury;
        
        availableButton.x = x + 0.1 * dx;
        availableButton.y = y + 0.7 * dy;
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

        sf::Sprite pictureSprite;
        pictureSprite.setTexture(pictureTexture);
        float pictureHeight = pictureSprite.getLocalBounds().height;
        float pictureWidth = pictureSprite.getLocalBounds().width;
        float scale = std::min(0.5f * dy / pictureHeight, dx / pictureWidth);
        pictureSprite.setPosition(x + 0.5 * dx - 0.5 * pictureWidth * scale, y + 5.f);
        pictureSprite.scale(scale, scale);

        availableButton.label = (available ? "Available" : "Unavailable");
        
        window.draw(rectangle);
        window.draw(text);
        window.draw(pictureSprite);
        availableButton.Draw(window);
    }
};

