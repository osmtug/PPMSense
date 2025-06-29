#include <imgui.h>
#include "SaveNetworkView.hpp"
#include <Eigen/Dense>
#include "EditView.hpp"
#include "ViewManager.hpp"
#include "tinyfiledialogs.h"
#include <string>
#include "NeuralNetwork.hpp"
#include <fstream>
#include <sstream>


SaveNetworkView::SaveNetworkView(const NeuralNetwork& neuralNetwork)
    : trainedNN(neuralNetwork)
{
    path[0] = '\0';
    fileName[0] = '\0';
}

void SaveNetworkView::couperDernierDossier(char* path) {
    char* dernierSlash = std::strrchr(path, '\\');
    if (dernierSlash) {
        *dernierSlash = '\0'; // Coupe la chaîne ici
    }
}

void SaveNetworkView::render() {
    ImGui::SetNextWindowSize(ImVec2(600, 200), ImGuiCond_Always);
    ImVec2 windowSize(600, 250);
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(center.x - windowSize.x / 2, center.y - windowSize.y / 2), ImGuiCond_Always);


    ImGui::Begin("Choisir un fichier .CSV MNIST (Avec le bon format)", nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoTitleBar
    );

    // Bouton retour (en haut à gauche)
    if (ImGui::Button("< Retour")) {
        viewManager->popView();
        ImGui::End();
        return;
    }

    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

    // Centrage horizontal
    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Text("Chemin du fichier :");

    ImGui::PushItemWidth(-150); // Laisser un peu de place pour le bouton à droite
    ImGui::InputText("##filePath1", path, sizeof(path));
    ImGui::PopItemWidth();
    ImGui::SameLine();

    if (ImGui::Button("Parcourir...")) {
        const char* filterPatterns[] = { "aucun" };
        const char* path = tinyfd_saveFileDialog(
            "Choisir un dossier où sauvegarder",
            "ne_pas_remplir", // Nom de fichier par défaut
            1,
            filterPatterns,
            "Fichiers"
        );
        char* dossierPath = path ? new char[1024] : nullptr; // Allouer un buffer pour le chemin du dossier
        if (path) {
            strncpy_s(dossierPath, sizeof(dossierPath), path, _TRUNCATE);
        }
        couperDernierDossier(dossierPath); // Coupe le nom du fichier pour ne garder que le dossier
    }

    ImGui::Spacing(); ImGui::Spacing();

    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

    // Centrage horizontal
    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Text("nom du fichier :");

    ImGui::PushItemWidth(-150); // Laisser un peu de place pour le bouton à droite
    ImGui::InputText("##filePath2", fileName, sizeof(fileName));
    ImGui::PopItemWidth();

    ImGui::Spacing(); ImGui::Spacing();


    float buttonWidth = 120.0f;
    float avail = ImGui::GetContentRegionAvail().x;
    ImGui::SetCursorPosX((avail - buttonWidth) * 0.5f);
    if (ImGui::Button("Annuler", ImVec2(buttonWidth, 0))) {
        viewManager->popView();
        ImGui::End();
        return;
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX((avail - buttonWidth) * 0.5f);
    if (ImGui::Button("Sauvegarder", ImVec2(buttonWidth, 0))) {
        if (strlen(fileName) > 0 && strlen(path) > 0) {
            std::string cheminComplet = std::string(path) + "/" + fileName;
            if (trainedNN.save(cheminComplet)) {
                tinyfd_messageBox("Sauvegarde réussie", ("Réseau sauvegardé sous : " + cheminComplet).c_str(), "info", "ok", 1);
                viewManager->popView();
            }
            else {
                tinyfd_messageBox("Erreur", "Échec de la sauvegarde du réseau", "ok", "error", 1);
            }
        }
        else {
            tinyfd_messageBox("Erreur", "Veuillez entrer un chemin et un nom de fichier valides", "ok", "error", 1);
        }
    }





    ImGui::End();
}
