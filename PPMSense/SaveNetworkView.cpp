#include <imgui.h>
#include "SaveNetworkView.hpp"
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
        *dernierSlash = '\0'; 
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

    if (ImGui::Button("< Retour")) {
        viewManager->popView();
        ImGui::End();
        return;
    }

    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Text("Chemin du fichier :");

    ImGui::PushItemWidth(-150);
    ImGui::InputText("##filePath1", path, sizeof(path));
    ImGui::SameLine();

    if (ImGui::Button("Parcourir...")) {
        const char* filters[] = { "*.nn" };
        const char* selectedPath = tinyfd_saveFileDialog(
            "Choisir un fichier de sauvegarde",
            "reseau.nn", 
            1,
            filters,
            "Fichier réseau"
        );

        if (selectedPath) {
            strncpy_s(path, sizeof(path), selectedPath, _TRUNCATE);
        }
        couperDernierDossier(path);
    }

    ImGui::Spacing(); ImGui::Spacing();

    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Text("nom du fichier :");

    ImGui::PushItemWidth(-150); 
    ImGui::InputText("##filePath2", fileName, sizeof(fileName));
    ImGui::PopItemWidth();

    ImGui::Spacing(); ImGui::Spacing();


    float buttonWidth = 100.0f;
    float spacing = ImGui::GetStyle().ItemSpacing.x; 
    float totalWidth = buttonWidth * 2 + spacing;

    float avail = ImGui::GetContentRegionAvail().x;
    ImGui::SetCursorPosX((avail - totalWidth) * 0.5f);  

    if (ImGui::Button("Annuler", ImVec2(buttonWidth, 0))) {
        viewManager->popView();
        ImGui::End();
        return;
    }

    ImGui::SameLine();

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
