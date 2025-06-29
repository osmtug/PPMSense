#pragma once

#include "IView.hpp"
#include "fonction.hpp" 
#include <string>
#include <imgui-SFML.h>
#include <imgui.h>

class DrawView : public IView {
public:
    DrawView(int largeur, int hauteur);
    void render() override;

    int brushRadius = 1;
    ImVec4 brushColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    ImVec2 lastPos = ImVec2(-1.0f, -1.0f);

private:
    Image image;
    unsigned char* pixelBuffer; // Pour afficher l’image avec ImGui::Image
    bool mouseDrawing = false;

    void updateTexture();
    void drawCircleOnImage(int cx, int cy);
    void drawLineOnImage(ImVec2 p1, ImVec2 p2);
    void onDraw(int mouseX, int mouseY);
    void renderSidebar();

    // Pour la texture SFML
    sf::Texture texture;
    sf::Sprite sprite;
};
