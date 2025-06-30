#pragma once

#include "IView.hpp"
#include <imgui.h>
#include "SaveNetworkView.hpp"
#include "ViewManager.hpp"
#include "tinyfiledialogs.h"
#include <string>
#include "NeuralNetwork.hpp"
#include <fstream>
#include <sstream>

class SaveNetworkView : public IView {
public:
    SaveNetworkView(const NeuralNetwork& neuralNetwork);
    void couperDernierDossier(char* path);
    void render() override;

private:
    char path[1024];
    char fileName[1024];
    NeuralNetwork trainedNN;
};