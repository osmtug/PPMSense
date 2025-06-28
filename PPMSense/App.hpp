#pragma once
#include <SFML/Graphics.hpp>
#include "ViewManager.hpp"

class App {
public:
    App();
    void run();

private:
    sf::RenderWindow window;
    ViewManager viewManager;
};
