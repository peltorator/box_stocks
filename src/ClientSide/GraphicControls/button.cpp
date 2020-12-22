#pragma once

#include <cmath>
#include <SFML/Graphics.hpp>
#include <string>

#include "font.cpp"

struct TButton {
    float X;
    float Y;
    float Dx;
    float Dy;
    std::string Label;
    bool IsPresent;

    TButton() = default;

    TButton(const float x, const float y, const float dx, const float dy, const std::string& label, const bool isPresent = true)
        : X(x)
        , Y(y)
        , Dx(dx)
        , Dy(dy)
        , Label(label)
        , IsPresent(true) {}

    void SetPosition(const float x, const float y) {
        X = x;
        Y = y;
    }

    void Draw(sf::RenderWindow& window) {
        sf::RectangleShape rectangle(sf::Vector2f(Dx, Dy));
        rectangle.setPosition(X, Y);
        rectangle.setFillColor(sf::Color::White);

        sf::Text text;
        text.setFont(NFont::font);
        text.setString(Label);
        text.setCharacterSize(18);
        text.setCharacterSize(std::min(1.0, Dx / text.getLocalBounds().width * 0.9) * 18.0);
        text.setFillColor(sf::Color::Black);
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0, textRect.top + textRect.height / 2.0);
        text.setPosition(X + Dx * 0.5, Y + Dy * 0.5);
        
        window.draw(rectangle);
        window.draw(text);
    }

    bool IsIn(float px, float py) {
        return X <= px && px <= X + Dx && Y <= py && py <= Y + Dy;
    }
};

