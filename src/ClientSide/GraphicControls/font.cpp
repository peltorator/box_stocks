#pragma once
#include <SFML/Graphics.hpp>

namespace NFont {
    sf::Font font;

    void LoadFont() {
        font.loadFromFile("../../../assets/fonts/arial.ttf");
    }
};
