#pragma once
#include <cmath>
#include <SFML/Graphics.hpp>
#include <string>
#include "font.cpp"

struct Button {
    float x;
    float y;
    float dx;
    float dy;
    std::string label;
    bool IsPresent;

    Button() = default;

    Button(float x, float y, float dx, float dy, std::string label)
        : x(x)
        , y(y)
        , dx(dx)
        , dy(dy)
        , label(label)
        , IsPresent(true) {}

    void SetPosition(float curx, float cury) {
        x = curx;
        y = cury;
    }

    void Draw(sf::RenderWindow& window) {
        sf::RectangleShape rectangle(sf::Vector2f(dx, dy));
        rectangle.setPosition(x, y);
        rectangle.setFillColor(sf::Color::White);

        sf::Text text;
        text.setFont(NFont::font);
        text.setString(label);
        text.setCharacterSize(18);
        text.setCharacterSize(std::min(1.0, dx / text.getLocalBounds().width * 0.9) * 18.0);
        text.setFillColor(sf::Color::Black);
        float cx = x + dx * 0.5 - text.getLocalBounds().width * 0.5;
        float cy = y + dy * 0.5 - text.getLocalBounds().height * 0.5;
        text.setPosition(cx, cy);
        
        window.draw(rectangle);
        window.draw(text);
    }

    bool IsIn(float px, float py) {
        return x <= px && px <= x + dx && y <= py && py <= y + dy;
    }
};

