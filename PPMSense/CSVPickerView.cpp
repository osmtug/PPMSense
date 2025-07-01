#include "CSVPickerView.hpp"
#include <Eigen/Dense>
#include "EditView.hpp"
#include "ViewManager.hpp"
#include <imgui.h>
#include "tinyfiledialogs.h"
#include <string>
#include "NeuralNetwork.hpp"
#include <fstream>
#include <sstream>
#include "SaveNetworkView.hpp"
#include "IView.hpp"

CSVPickerView::CSVPickerView() {
    filePathTrain[0] = '\0';
    filePathTest[0] = '\0';
}

void CSVPickerView::render() {
    ImGui::SetNextWindowSize(ImVec2(600, 200), ImGuiCond_Always);
    ImVec2 windowSize(800, 300);
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
    ImGui::Text("Chemin du fichier de training :");

    ImGui::PushItemWidth(-150); 
    ImGui::InputText("##filePath1", filePathTrain, sizeof(filePathTrain));
    ImGui::PopItemWidth();
    ImGui::SameLine();

    if (ImGui::Button("Parcourir train")) {
        const char* filterPatterns[] = { "*.csv" };
        const char* path = tinyfd_openFileDialog(
            "Choisir un fichier .csv",
            "",
            1,
            filterPatterns,
            "Fichiers csv",
            0
        );
        if (path) {
            strncpy_s(filePathTrain, sizeof(filePathTrain), path, _TRUNCATE);
        }
    }

    ImGui::Spacing(); ImGui::Spacing();

    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Text("Chemin du fichier de teste :");

    ImGui::PushItemWidth(-150); 
    ImGui::InputText("##filePath2", filePathTest, sizeof(filePathTest));
    ImGui::PopItemWidth();
    ImGui::SameLine();

    if (ImGui::Button("Parcourir teste")) {
        const char* filterPatterns[] = { "*.csv" };
        const char* path = tinyfd_openFileDialog(
            "Choisir un fichier .csv",
            "",
            1,
            filterPatterns,
            "Fichiers csv",
            0
        );
        if (path) {
            strncpy_s(filePathTest, sizeof(filePathTest), path, _TRUNCATE);
        }
    }

    ImGui::Spacing(); ImGui::Spacing();

    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Text("Chemin du fichier de model ia deja entrainer (optionnel) :");

    ImGui::PushItemWidth(-150); 
    ImGui::InputText("##filePath3", modelPath, sizeof(modelPath));
    ImGui::PopItemWidth();
    ImGui::SameLine();

    if (ImGui::Button("Parcourir model")) {
        const char* filterPatterns[] = { "*.nno" };
        const char* path = tinyfd_openFileDialog(
            "Choisir un fichier .nno",
            "",
            1,
            filterPatterns,
            "Fichiers nno",
            0
        );
        if (path) {
            strncpy_s(modelPath, sizeof(modelPath), path, _TRUNCATE);
        }
    }

    ImGui::Spacing(); ImGui::Spacing();

    float buttonWidth = 120.0f;
    float avail = ImGui::GetContentRegionAvail().x;
    ImGui::SetCursorPosX((avail - buttonWidth) * 0.5f);
    if (!training && ImGui::Button("Entrainer", ImVec2(buttonWidth, 0))) {
        if (strlen(filePathTest) > 0 && strlen(filePathTrain) > 0) {
            std::vector<Eigen::VectorXf> X_train, X_test;
            std::vector<int> y_train, y_test;

            if (!loadCSV_Eigen(filePathTrain, X_train, y_train) || !loadCSV_Eigen(filePathTest, X_test, y_test)) {
                tinyfd_messageBox("Erreur", "Échec de chargement des CSV", "ok", "error", 1);
                return;
            }

            training = true;
            trainProgress = 0.0f;

            trainingThread = std::thread([this, X_train, y_train, X_test, y_test]() {
                const char* extension = ".nno";
                size_t pathLen = strlen(modelPath);
                size_t extLen = strlen(extension);

                if (pathLen >= extLen && strcmp(modelPath + pathLen - extLen, extension) == 0) {
                    std::string pathStr = modelPath;
                    trainedNN.load(pathStr);
                }
                else {
                    trainedNN = NeuralNetwork({ 28 * 28, 128, 64, 10 }, 0.01f);
                }
                trainedNN.trainDataset(X_train, y_train, 10, &trainProgress);
                float acc = trainedNN.evaluate(X_test, y_test);

                std::string msg = "Précision : " + std::to_string(acc * 100.0f) + " %";
                tinyfd_messageBox("Résultat du test", msg.c_str(), "ok", "info", 1);

                training = false;
                viewManager->pushView(std::make_unique<SaveNetworkView>(trainedNN));
            });

            trainingThread.detach();  // pour ne pas bloquer
        }
    }

    

    if (training) {
        ImGui::Spacing();
        ImGui::Text("Entrainement...");
        ImGui::ProgressBar(trainProgress, ImVec2(-1.0f, 0));
    }

    ImGui::End();
}

bool CSVPickerView::loadCSV_Eigen(const char* filename, std::vector<Eigen::VectorXf>& data, std::vector<int>& labels) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::string line;
    std::getline(file, line); // ignore header

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string item;

        std::getline(ss, item, ','); // label
        int label = std::stoi(item);
        labels.push_back(label);

        Eigen::VectorXf vec(28 * 28); // 784
        int index = 0;

        while (std::getline(ss, item, ',') && index < 784) {
            vec[index++] = std::stof(item) / 255.0f; // normalisé
        }

        data.push_back(vec);
    }

    return true;
}
