#include "DrawView.hpp"
#include "ViewManager.hpp"
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/OpenGL.hpp>



DrawView::DrawView(int largeur, int hauteur)
    : image(largeur, hauteur), pixelBuffer(nullptr) {
    updateTexture();
}

void DrawView::updateTexture() {
    int w = image.getLargeur();
    int h = image.getLongueur();

    sf::Image img;
    img.create(w, h);

    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x)
            img.setPixel(x, y, sf::Color(
                image._rouge[y][x],
                image._vert[y][x],
                image._bleu[y][x]
            ));

    texture.loadFromImage(img);
    sprite.setTexture(texture);
}



void DrawView::drawCircleOnImage(int cx, int cy) {
    int sqRadius = brushRadius * brushRadius;
    for (int y = -brushRadius; y <= brushRadius; ++y) {
        for (int x = -brushRadius; x <= brushRadius; ++x) {
            if (x * x + y * y <= sqRadius) {
                image.setPixel(cx + x, cy + y, brushColor);
            }
        }
    }
}

void DrawView::drawLineOnImage(ImVec2 p1, ImVec2 p2) {
    float dist = sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
    int steps = static_cast<int>(dist);

    for (int i = 0; i <= steps; ++i) {
        float t = i / (float)steps;
        int x = static_cast<int>(p1.x + t * (p2.x - p1.x));
        int y = static_cast<int>(p1.y + t * (p2.y - p1.y));
        drawCircleOnImage(x, y);  // dessine un cercle autour du point
    }
}

// Dans ta fonction de dessin principale :
void DrawView::onDraw(int mouseX, int mouseY) {
    if (lastPos.x < 0) {
        lastPos = ImVec2(mouseX, mouseY);
        drawCircleOnImage(mouseX, mouseY);
        return;
    }

    drawLineOnImage(lastPos, ImVec2(mouseX, mouseY));
    lastPos = ImVec2(mouseX, mouseY);
}

void DrawView::renderSidebar() {
    ImGui::Text("Outils");
    
    ImGui::Text("Options du pinceau");

    ImGui::ColorEdit4("Couleur", (float*)&brushColor, ImGuiColorEditFlags_NoInputs);

    // Slider rayon pinceau (exemple)
    ImGui::SliderInt("Rayon", &brushRadius, 1, 50);
}

void DrawView::render() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    ImGui::Begin("Dessin", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse);

    float widthLeft = ImGui::GetContentRegionAvail().x * 0.7f;

    // === Partie gauche : image + retour ===
    ImGui::BeginChild("ZoneGauche", ImVec2(widthLeft, 0), true);

    if (ImGui::Button("< Retour")) {
        viewManager->popView();
        ImGui::EndChild();
        ImGui::End();
        return;
    }

    ImGui::Separator();

    // === Affichage de l’image centrée et redimensionnée ===
    sf::Vector2u imgSize = texture.getSize();
    ImVec2 avail = ImGui::GetContentRegionAvail();

    // Calcul de l’échelle
    float scaleX = avail.x / imgSize.x;
    float scaleY = avail.y / imgSize.y;
    float scale = std::min(scaleX, scaleY);

    // Taille finale
    ImVec2 finalSize = ImVec2(imgSize.x * scale, imgSize.y * scale);

    // Centrage horizontal uniquement
    float centerX = (avail.x - finalSize.x) * 0.5f;
    if (centerX < 0) centerX = 0;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + centerX);

    // Affiche l’image
    GLuint textureID = texture.getNativeHandle();
    ImGui::Image((ImTextureID)(intptr_t)textureID, finalSize);

    // Dessin dans l’image
    ImVec2 topLeft = ImGui::GetItemRectMin();
    ImVec2 mouse = ImGui::GetIO().MousePos;

    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0)) {
        float relativeX = (mouse.x - topLeft.x) / scale;
        float relativeY = (mouse.y - topLeft.y) / scale;
        onDraw((int)relativeX, (int)relativeY);
        updateTexture();
    }
    else {
        // Bouton relâché ou souris pas sur l'image
        lastPos = ImVec2(-1, -1);
    }


    

    ImGui::EndChild();

    // === Partie droite : sidebar ===
    ImGui::SameLine();
    ImGui::BeginChild("ZoneDroite", ImVec2(0, 0), true);
    renderSidebar();
    ImGui::EndChild();

    ImGui::End();
}
