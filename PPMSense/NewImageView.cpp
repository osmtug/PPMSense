#include "NewImageView.hpp"
#include "EditView.hpp"
#include "ViewManager.hpp"
#include <imgui.h>
#include "DrawView.hpp"

NewImageView::NewImageView() : largeur(256), hauteur(256) {}

void NewImageView::render() {
    ImGui::SetNextWindowSize(ImVec2(500, 250), ImGuiCond_Always);
    ImVec2 windowSize(500, 250);
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(ImVec2(center.x - windowSize.x / 2, center.y - windowSize.y / 2), ImGuiCond_Always);

    ImGui::Begin("Nouvelle Image", nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoTitleBar
    );

    if (ImGui::Button("< Retour")) {
        viewManager->popView();
        ImGui::End();
        return;
    }

    ImGui::Spacing(); ImGui::Spacing();

    ImGui::Text("Entrez les dimensions de l'image :");
    ImGui::InputInt("Largeur", &largeur);
    ImGui::InputInt("Hauteur", &hauteur);

    largeur = std::max(1, largeur);
    hauteur = std::max(1, hauteur);

    ImGui::Spacing();

    float buttonWidth = 120.0f;
    float avail = ImGui::GetContentRegionAvail().x;
    ImGui::SetCursorPosX((avail - buttonWidth) * 0.5f);

    if (ImGui::Button("Créer", ImVec2(buttonWidth, 0))) {
        // Crée une image noire (vecteurs 2D de zéros)
        //Image image(largeur, hauteur);  

        // Redirige vers EditView avec cette image
        viewManager->pushView(std::make_unique<DrawView>(largeur, hauteur));
    }

    ImGui::End();
}
