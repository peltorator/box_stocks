#pragma once

#include <cmath>
#include <SFML/Graphics.hpp>
#include <string>

#include "font.cpp"

struct TTextField {
    float X;
    float Y;
    float Dx;
    float Dy;
    std::string Name;
    std::string Label;
    bool NeedPop;

    TTextField() = default;

    TTextField(const float x, const float y, const float dx, const float dy, const std::string& name)
        : X(x)
        , Y(y)
        , Dx(dx)
        , Dy(dy)
        , Name(name)
        , Label("")
        , NeedPop(false) {}

    void Draw(sf::RenderWindow& window) {
        sf::RectangleShape rectangle(sf::Vector2f(Dx, Dy));
        rectangle.setPosition(X, Y);
        rectangle.setFillColor(sf::Color::White);

        sf::Text labelText;
        labelText.setFont(NFont::font);
        labelText.setString(Label);
        labelText.setCharacterSize(18);
        labelText.setCharacterSize(std::min(1.0, Dx / labelText.getLocalBounds().width * 0.9) * 18.0);
        labelText.setFillColor(sf::Color::Black);
        float cx = X + Dx * 0.1;
        float cy = Y + Dy * 0.5 - labelText.getLocalBounds().height * 0.5;
        labelText.setPosition(cx, cy);

        sf::Text nameText;
        nameText.setFont(NFont::font);
        nameText.setString(Name);
        nameText.setCharacterSize(18);
        nameText.setCharacterSize(std::min(1.0, Dx / nameText.getLocalBounds().width * 0.9) * 18.0);
        nameText.setFillColor(sf::Color::White);
        float cx2 = X + Dx * 0.5 - nameText.getLocalBounds().width * 0.5;
        float cy2 = Y - Dy * 0.2 - nameText.getLocalBounds().height;
        nameText.setPosition(cx2, cy2);
        
        window.draw(rectangle);
        window.draw(labelText);
        window.draw(nameText);
    }

    bool IsIn(float px, float py) {
        return X <= px && px <= X + Dx && Y <= py && py <= Y + Dy;
    }

    void AddChar(const char& c) {
        Label.push_back(c);
    }

    void PopChar() {
        if (!Label.empty() && NeedPop) {
            Label.pop_back();
        }
        NeedPop = !NeedPop;
    }

    void Clear() {
        Label.clear();
    }
};


