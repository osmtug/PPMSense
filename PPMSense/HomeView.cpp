#include "HomeView.hpp"
#include <imgui.h>
#include "FilePickerView.hpp"
#include "EditView.hpp"
#include "ViewManager.hpp"
#include "NewImageView.hpp"
#include "TrainModelView.hpp"

void HomeView::render() {
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

    if (ImGui::Button("Dessiner", btnSize)) {
        viewManager->pushView(std::make_unique<NewImageView>());
    }

    ImGui::SetCursorPos(ImVec2((winSize.x - btnSize.x) / 2, (winSize.y - 150) / 2 + 80));
    if (ImGui::Button("Modifier", btnSize)) {
        viewManager->pushView(std::make_unique<FilePickerView>());
    }

    ImGui::SetCursorPos(ImVec2((winSize.x - btnSize.x) / 2, (winSize.y - 150) / 2 + 160));
    if (ImGui::Button("Entrainer", btnSize)) {
        viewManager->pushView(std::make_unique<TrainModelView>());
    }

    ImGui::End();
}
