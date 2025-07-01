#include "FilePickerView.hpp"
#include "EditView.hpp"
#include "ViewManager.hpp"
#include <imgui.h>
#include "tinyfiledialogs.h"
#include <string>

FilePickerView::FilePickerView() {
    filePath[0] = '\0'; 
}

void FilePickerView::render() {
    ImGui::SetNextWindowSize(ImVec2(600, 200), ImGuiCond_Always);
    ImVec2 windowSize(600, 200);
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(center.x - windowSize.x / 2, center.y - windowSize.y / 2), ImGuiCond_Always);


    ImGui::Begin("Choisir un fichier .ppm", nullptr,
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
    ImGui::InputText("##filePath", filePath, sizeof(filePath));
    ImGui::PopItemWidth();
    ImGui::SameLine();

    if (ImGui::Button("Parcourir...")) {
        const char* filterPatterns[] = { "*.ppm" };
        const char* path = tinyfd_openFileDialog(
            "Choisir un fichier .ppm",
            "",
            1,
            filterPatterns,
            "Fichiers PPM",
            0
        );
        if (path) {
            strncpy_s(filePath, sizeof(filePath), path, _TRUNCATE);
        }
    }

    ImGui::Spacing(); ImGui::Spacing();

    float buttonWidth = 120.0f;
    float avail = ImGui::GetContentRegionAvail().x;
    ImGui::SetCursorPosX((avail - buttonWidth) * 0.5f);
    if (ImGui::Button("Modifier", ImVec2(buttonWidth, 0))) {
        if (strlen(filePath) > 0) {
            viewManager->pushView(std::make_unique<EditView>(std::string(filePath)));
        }
    }

    ImGui::End();
}
