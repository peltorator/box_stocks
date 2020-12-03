#pragma once
#include <cmath>
#include <SFML/Graphics.hpp>
#include <string>
#include "font.cpp"

struct TextField {
    float x;
    float y;
    float dx;
    float dy;
    std::string name;
    std::string label;
    bool needPop;

    TextField() = default;

    TextField(float x, float y, float dx, float dy, std::string name)
        : x(x)
        , y(y)
        , dx(dx)
        , dy(dy)
        , name(name)
        , label("")
        , needPop(false) {}

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
        float cx = x + dx * 0.1;
        float cy = y + dy * 0.5 - text.getLocalBounds().height * 0.5;
        text.setPosition(cx, cy);

        sf::Text nameText;
        nameText.setFont(NFont::font);
        nameText.setString(name);
        nameText.setCharacterSize(18);
        nameText.setCharacterSize(std::min(1.0, dx / nameText.getLocalBounds().width * 0.9) * 18.0);
        nameText.setFillColor(sf::Color::White);
        float cx2 = x + dx * 0.5 - nameText.getLocalBounds().width * 0.5;
        float cy2 = y - dy * 0.2 - nameText.getLocalBounds().height;
        nameText.setPosition(cx2, cy2);
        
        window.draw(rectangle);
        window.draw(text);
        window.draw(nameText);
    }

    bool IsIn(float px, float py) {
        return x <= px && px <= x + dx && y <= py && py <= y + dy;
    }

    void AddChar(const char& c) {
        label.push_back(c);
    }

    void PopChar() {
        if (!label.empty() && needPop) {
            label.pop_back();
        }
        needPop = !needPop;
    }

    void Clear() {
        label.clear();
    }
};


