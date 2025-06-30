#include "TrainModelView.hpp"
#include <imgui.h>
#include "ViewManager.hpp"
#include "CSVPickerView.hpp"


void TrainModelView::render() {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("Accueil", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse);

    ImVec2 winSize = ImGui::GetWindowSize();
    ImVec2 btnSize(300, 60);
    ImGui::SetCursorPos(ImVec2((winSize.x - btnSize.x) / 2, (winSize.y - 150) / 2));

    if (ImGui::Button("classifieur d’images manuscrites", btnSize)) {
        viewManager->pushView(std::make_unique<CSVPickerView>());
    }

    ImGui::SetCursorPos(ImVec2((winSize.x - btnSize.x) / 2, (winSize.y - 150) / 2 + 80));
    if (ImGui::Button("générateur d’images", btnSize)) {
		// Passe à la vue entrainement generateur d'image
        //viewManager->pushView(std::make_unique< TODO >());
    }

    ImGui::SetCursorPos(ImVec2((winSize.x - btnSize.x) / 2, (winSize.y - 150) / 2 + 160));
    if (ImGui::Button("Retour", btnSize)) {
        viewManager->popView();
    }

    ImGui::End();
}