#include "App.hpp"
#include <imgui-SFML.h>
#include <imgui.h>
#include "HomeView.hpp"

App::App()
    : window(sf::VideoMode(1280, 720), "PPMSense - Editeur PPM") {
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    // Start with HomeView
    viewManager.pushView(std::make_unique<HomeView>());
}

void App::run() {
    sf::Clock deltaClock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        window.clear(sf::Color(30, 30, 30));

        if (auto currentView = viewManager.getCurrentView()) {
            currentView->render();
        }

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}
